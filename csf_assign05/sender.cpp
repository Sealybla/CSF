#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int server_responses(Connection &conn, Message &msg) {
  if (!conn.send(msg)) {
    std::cerr << "Error: Failed to send message\n";
    return 1; 
  }
  Message server_resp;
  if (!conn.receive(server_resp)) {
    std::cerr<< "Error: No response from server\n";
    return 1; 
  }
  if (server_resp.tag == TAG_ERR) {
    std::cerr << server_resp.data << "\n";
  }
  return 0; 

}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  //check if successfully opened
  if (!conn.is_open()) {
    std::cerr << "Failed to connect to server\n";
    return 1;
  }

  // TODO: send slogin message
  Message slogin_msg(TAG_SLOGIN, username);
  if (!conn.send(slogin_msg)) {
    std::cerr << "Error: Failed to send slogin message\n";
    return 1;
  }
  //response from server 
  Message server_resp; 
  if (!conn.receive(server_resp)) {
    std::cerr << "No server response. Login failed \n";
    return 1;
  }

  if (server_resp.tag == TAG_ERR) {
    std::cerr << server_resp.data << "\n";
    return 1;
  }



  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  // loop part of diagram 
  std::string line;
  while (true) {
    // std::cout << "> ";
    if (!std::getline(std::cin, line)) {
      break; // EOF or error
    }
    line = trim(line);
    if (line.empty()) {
      continue; //skip empty lines
    }
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    
    if (command == "/join"){
      std::string room_name;
      iss >> room_name; 
      Message msg(TAG_JOIN, room_name); 
      if(server_responses(conn, msg)) {
        return 1;
      }
    } else if (command == "/leave") {
      Message msg(TAG_LEAVE, ""); 
      if(server_responses(conn, msg)) {
        return 1;
      }      
    } else if (command == "/quit") {
      Message msg(TAG_QUIT, ""); 
      if(server_responses(conn, msg)) {
        return 1;
      }
      break;
    } else {
      Message msg(TAG_SENDALL, line); 
      if(server_responses(conn, msg)) {
        return 1;
      }
    }
  }
  return 0;
}

