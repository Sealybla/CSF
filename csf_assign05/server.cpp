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

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

// Helper function to handle a sender client
void chat_with_sender(Connection *conn, Server *server, User *user) {
  Message msg;
  // track rooms this sender has joined
  std::set<std::string> joined_rooms;
  
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
      Room *room = server->find_or_create_room(msg.data);
      joined_rooms.insert(msg.data);
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_LEAVE) {
      // Sender is leaving a room
      if (joined_rooms.find(msg.data) == joined_rooms.end()) {
        // Sender not in that room
        if (!conn->send(Message(TAG_ERR, "Not in room"))) {
          break;
        }
        continue;
      }
      joined_rooms.erase(msg.data);
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_SENDALL) {
      // Sender is broadcasting a message to all receivers in a room
      // Parse the message: room_name:message_text
      size_t colon_pos = msg.data.find(':');
      if (colon_pos != std::string::npos) {
        std::string room_name = msg.data.substr(0, colon_pos);
        std::string message_text = msg.data.substr(colon_pos + 1);
        // Ensure sender has joined the room before sending
        if (joined_rooms.find(room_name) == joined_rooms.end()) {
          if (!conn->send(Message(TAG_ERR, "Sender not in room"))) {
            break;
          }
          continue;
        }
        Room *room = server->find_or_create_room(room_name);
        room->broadcast_message(user->username, message_text);
      }
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
void chat_with_receiver(Connection *conn, Server *server, User *user) {
  Message msg;
  
  while (true) {
    // Check if there are any pending messages for this user
    Message *pending = user->mqueue.dequeue();
    if (pending != nullptr) {
      // Deliver pending message
      if (!conn->send(*pending)) {
        // Failed to send message - terminate and clean up
        delete pending;
        break;
      }
      delete pending;
    } else {
      // Send empty message to indicate no messages available
      if (!conn->send(Message(TAG_EMPTY, ""))) {
        // Failed to send empty message - terminate
        break;
      }
    }
    
    // Receive a message from the receiver (should be join/leave/quit)
    if (!conn->receive(msg)) {
      // Connection lost or error receiving message - terminate
      break;
    }
    if (conn->get_last_result() == Connection::INVALID_MSG) {
      if (!conn->send(Message(TAG_ERR, "Invalid message format"))) {
        break;
      }
      continue;
    }
    
    if (msg.tag == TAG_JOIN) {
      // Receiver is joining a room
      Room *room = server->find_or_create_room(msg.data);
      room->add_member(user);
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        room->remove_member(user);
        break;
      }
    } else if (msg.tag == TAG_LEAVE) {
      // Receiver is leaving a room
      // msg.data should contain the room name
      Room *room = server->find_or_create_room(msg.data);
      room->remove_member(user);
      // Send OK response
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - terminate
        break;
      }
    } else if (msg.tag == TAG_QUIT) {
      // Receiver is quitting
      if (!conn->send(Message(TAG_OK, ""))) {
        // Failed to send response - but still exit
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

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // Convert arg from void* to ClientContext*
  ClientContext *ctx = static_cast<ClientContext *>(arg);
  Connection *conn = ctx->conn;
  Server *server = ctx->server;
  
  // Read login message (should be tagged either with TAG_SLOGIN or TAG_RLOGIN)
  Message login_msg;
  // keep trying until we get a valid login or the connection dies
  while (true) {
    if (!conn->receive(login_msg)) {
      // Failed to receive login message (EOF/error)
      conn->close();
      delete conn;
      delete ctx;
      return nullptr;
    }
    if (conn->get_last_result() == Connection::INVALID_MSG) {
      // protocol error: notify client and keep waiting for a proper login
      if (!conn->send(Message(TAG_ERR, "Invalid login message format"))) {
        // couldn't reply; treat as disconnected
        conn->close();
        delete conn;
        delete ctx;
        return nullptr;
      }
      continue;
    }
    break;
  }

  // Create a User object to track pending messages
  User *user = nullptr;
  bool is_sender = false;
  
  if (login_msg.tag == TAG_SLOGIN) {
    // Client is logging in as a sender
    is_sender = true;
    user = new User(login_msg.data);
    if (!conn->send(Message(TAG_OK, ""))) {
      // Failed to send login response - clean up and exit
      delete user;
      conn->close();
      delete conn;
      delete ctx;
      return nullptr;
    }
  } else if (login_msg.tag == TAG_RLOGIN) {
    // Client is logging in as a receiver
    is_sender = false;
    user = new User(login_msg.data);
    if (!conn->send(Message(TAG_OK, ""))) {
      // Failed to send login response - clean up and exit
      delete user;
      conn->close();
      delete conn;
      delete ctx;
      return nullptr;
    }
  } else {
    // Invalid login tag - notify client and close
    conn->send(Message(TAG_ERR, "Invalid login message"));
    conn->close();
    delete conn;
    delete ctx;
    return nullptr;
  }
  
  // Depending on whether the client logged in as a sender or receiver,
  // communicate with the client
  if (is_sender) {
    chat_with_sender(conn, server, user);
  } else {
    chat_with_receiver(conn, server, user);
  }
  
  // Clean up
  conn->close();
  delete conn;
  delete user;
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
    int clientfd = Accept(m_ssock, nullptr, nullptr);
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
  
  RoomMap::iterator it = m_rooms.find(room_name);
  if (it != m_rooms.end()) {
    return it->second;
  }
  
  // Create a new room and add it to the map
  Room *new_room = new Room(room_name);
  m_rooms[room_name] = new_room;
  return new_room;
}
