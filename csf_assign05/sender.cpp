#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

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
  if (!conn.is_open()) {
    std::cerr << "Error: Unable to connect to server\n";
    return 1;
  }

  // TODO: send slogin message
  Message slogin_msg(TAG_SLOGIN, username);
  if (!conn.send(slogin_msg)) {
    std::cerr << "Error: Failed to send slogin message\n";
    return 1;
  }
  Message server_resp; 
  if (!conn.receive(server_resp)|| server_resp.get_tag() != TAG_OK) {
    std::cerr << "Login failed \n";
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string line;
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, line)) {
      break; // EOF or error
    }
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    
    if (command == "join"){
      std::string room_name;
      iss >> room_name; 
      Message msg(TAG_JOIN, room_name); 
      conn.send(msg); 
    } else if (command == "leave") {
      std::string room_name;
      iss >> room_name; 
      Message msg(TAG_LEAVE, room_name); 
      conn.send(msg);
    } else if (command == "sendall") {
      std::string room_name;
      iss >> room_name; 
      std::string message_text;
      std::getline(iss, message_text); 
      //remove leading space if any
      if (!message_text.empty() && message_text[0] == ' ') {
        message_text.erase(0, 1);
      }
      Message msg(TAG_SENDALL, room_name + ":" + message_text); 
      conn.send(msg);
    } else if (command == "quit") {
      Message msg(TAG_QUIT, ""); 
      conn.send(msg); 
      break;
    } else{
      std::cerr << "Error: Unknown command '" << command << "'\n";
      continue;
    }
  return 0;
}
