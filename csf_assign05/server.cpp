#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// Structure to pass data to the client thread
struct ClientContext {
  Connection *conn;
  Server *server;
  
  ClientContext(Connection *conn, Server *server)
    : conn(conn), server(server) { }
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

// Helper function to handle a sender client
void chat_with_sender(Connection *conn, Server *server, const std::string &username) {
  Message msg;
  Room *current_room = nullptr; // Track which room the sender is currently in
  
  while (true) {
    // Receive a message from the sender
    if (!conn->receive(msg)) {
      // Connection lost or error receiving message
      break;
    }
    if (conn->get_last_result() == Connection::INVALID_MSG) {
      // send protocol error but do not disconnect
      if (!conn->send(Message(TAG_ERR, "Invalid message format"))) {
        break; // can't respond, treat as disconnect
      }
      continue;
    }
    
    if (msg.tag == TAG_JOIN) {
      // Sender is joining a room
      current_room = server->find_or_create_room(msg.data);
      // Send OK response (sender does NOT get added to room member list)
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_LEAVE) {
      // Sender is leaving a room
      if (!current_room) {
        // Sender not in any room
        if (!conn->send(Message(TAG_ERR, "Not in room"))) {
          break;
        }
        continue;
      }
      current_room = nullptr;
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_SENDALL) {
      // Sender is broadcasting a message to all receivers in their current room
      if (!current_room) {
        // Sender not in any room - cannot broadcast
        if (!conn->send(Message(TAG_ERR, "Not in room"))) {
          break;
        }
        continue;
      }
      current_room->broadcast_message(username, msg.data);
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_QUIT) {
      // Sender is quitting
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - but still break to exit
        break;
      }
      break;
    } else {
      // Unknown message tag - send error response
      if (!conn->send(Message(TAG_ERR, "Unknown message type"))) {
        // Failed to send error response - terminate
        break;
      }
    }
  }
}

// Helper function to handle a receiver client
void chat_with_receiver(Connection *conn, Server *server, const std::string &username) {
  Message msg;
  
  // First, wait for TAG_JOIN to determine which room the receiver wants to join
  Room *room = nullptr;
  while (room == nullptr) {
    if (!conn->receive(msg)) {
      // Connection lost or error receiving message
      return;
    }
    if (conn->get_last_result() == Connection::INVALID_MSG) {
      // send protocol error but do not disconnect
      if (!conn->send(Message(TAG_ERR, "Invalid message format"))) {
        return; // can't respond, treat as disconnect
      }
      continue;
    }
    
    if (msg.tag == TAG_JOIN) {
      // Receiver is joining a room
      room = server->find_or_create_room(msg.data);
      
      // Create User and add to room
      User *user = new User(username);
      room->add_member(user);
      
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - remove from room and terminate
        room->remove_member(user);
        delete user;
        return;
      }
      
      // Now poll for messages to deliver
      while (true) {
        Message *pending = user->mqueue.dequeue();
        if (pending != nullptr) {
          // Deliver pending message
          if (!conn->send(*pending)) {
            // Failed to send message - terminate and clean up
            delete pending;
            room->remove_member(user);
            delete user;
            return;
          }
          delete pending;
        } else {
          // No message available, poll again
          // (dequeue timeout returns nullptr after 1 second)
          // Just loop back to dequeue again
        }
      }
    } else {
      // Receiver sent something other than JOIN - error
      if (!conn->send(Message(TAG_ERR, "Expected JOIN message"))) {
        return;
      }
    }
  }
}

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // Convert arg from void* to ClientContext*
  ClientContext *ctx = static_cast<ClientContext *>(arg);
  Connection *conn = ctx->conn;
  Server *server = ctx->server;
  
  // Read login message (should be tagged either with TAG_SLOGIN or TAG_RLOGIN)
  Message login_msg;
  if (!conn->receive(login_msg)) {
    // Failed to receive login message (EOF/error)
    conn->close();
    delete conn;
    delete ctx;
    return nullptr;
  }
  if (conn->get_last_result() == Connection::INVALID_MSG) {
    // protocol error on login
    conn->send(Message(TAG_ERR, "Invalid login message format"));
    conn->close();
    delete conn;
    delete ctx;
    return nullptr;
  }

  // Check login tag and proceed
  if (login_msg.tag == TAG_SLOGIN) {
    // Client is logging in as a sender
    if (!conn->send(Message(TAG_OK, ""))) {
      // Failed to send login response - clean up and exit
      conn->close();
      delete conn;
      delete ctx;
      return nullptr;
    }
    chat_with_sender(conn, server, login_msg.data);
  } else if (login_msg.tag == TAG_RLOGIN) {
    // Client is logging in as a receiver
    if (!conn->send(Message(TAG_OK, ""))) {
      // Failed to send login response - clean up and exit
      conn->close();
      delete conn;
      delete ctx;
      return nullptr;
    }
    chat_with_receiver(conn, server, login_msg.data);
  } else {
    // Invalid login tag - notify client and close
    conn->send(Message(TAG_ERR, "Invalid login message"));
    conn->close();
    delete conn;
    delete ctx;
    return nullptr;
  }
  
  // Clean up
  conn->close();
  delete conn;
  delete ctx;

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // Initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  // Destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // Create the server socket using open_listenfd
  m_ssock = open_listenfd(std::to_string(m_port).c_str());
  if (m_ssock < 0) {
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // Infinite loop calling accept and starting a new pthread for each connected client
  while (true) {
    // Accept a client connection
    struct sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    int clientfd = Accept(m_ssock, (struct sockaddr *)&clientaddr, &clientlen);
    if (clientfd < 0) {
      continue; // Skip on error
    }
    
    // Create a Connection object for this client
    Connection *conn = new Connection(clientfd);
    
    // Create a ClientContext struct to pass to the worker thread
    ClientContext *ctx = new ClientContext(conn, this);
    
    // Create a new thread to handle this client
    pthread_t tid;
    if (pthread_create(&tid, nullptr, worker, ctx) != 0) {
      // If thread creation fails, clean up
      delete conn;
      delete ctx;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // Lock the mutex to ensure thread-safe access to the room map
  Guard guard(m_lock);
  
  auto it = m_rooms.find(room_name);
  if (it != m_rooms.end()) {
    return it->second;
  }
  
  // Create a new room and add it to the map
  Room *new_room = new Room(room_name);
  m_rooms[room_name] = new_room;
  return new_room;
}
