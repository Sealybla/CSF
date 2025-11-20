#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
bool running = true;

void deliveries(Connection &conn) {
 
  while (running) {
    Message incoming;
    if(!conn.receive(incoming)){
      running = false;
      return;
    }
    if(incoming.tag == TAG_DELIVERY) {
      //parse
      std::string payload = incoming.data;
      size_t c1 = payload.find(':');
      size_t c2 = payload.find(':', c1+1);
      if(c1 == std::string::npos || c2 == std::string::npos) continue;
      std::string sender = payload.substr(c1 +1, c2 - c1 -1);
      std::string text = payload.substr(c2 + 1);
      std::cout << sender << ": "<< text<< std::endl;
      //new message
    }
    
    
  } 
  
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
  try {
    conn.connect(server_hostname, server_port);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: Unable to connect to server\n";
    return 1;
  }
  
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
  Message slogin_resp;
  if(!conn.receive(slogin_resp)) {
    std::cerr << "Error: Failed to recieve slogin reponse\n";
    return 1;
  }
  if (slogin_resp.tag != TAG_OK) {
    std::cerr << slogin_resp.data <<"\n";
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
    
    if (command == "/join"){
      std::string room_name;
      iss >> room_name; 
      Message msg(TAG_JOIN, room_name); 
      if(!conn.send(msg)){
        std::cerr << "Error: Failed to send join message\n";
        continue;
      } 
      Message resp;
      if(!conn.receive(resp)) {
        std::cerr << "Error: Failed to receive join response\n";
        continue;
      }
      if(resp.tag == TAG_ERR) {
        std::cerr << resp.data << "\n";
        continue;
      }
 
    } else if (command == "/leave") {
      Message msg(TAG_LEAVE, ""); 
      if(!conn.send(msg)) {
        std::cerr << "Error: Failed to send leave message\n";
        continue;
      }
      Message resp;
      if(!conn.receive(resp)) {
        std::cerr << "Error: Failed to receive leave response\n";
        continue;
      }
      if(resp.tag == TAG_ERR) {
        std::cerr << resp.data << "\n";
        continue;
      }
    } else if (command == "/sendall") { 
      std::string message_text;
      std::getline(iss, message_text); 
      //remove leading space if any
      if (!message_text.empty() && message_text[0] == ' ') {
        message_text.erase(0, 1);
      }
      Message msg(TAG_SENDALL, message_text); 
      if(!conn.send(msg)) {
        std::cerr << "Error: Failed to send sendall message\n";
        continue;
      }
      
      Message resp;
      if(!conn.receive(resp)) {
        std::cerr << "Error: Failed to receive leave response\n";
        continue;
      }
      if(resp.tag == TAG_ERR) {
        std::cerr << resp.data << "\n";
        continue;
      }
      
    } else if (command == "/quit") {
      Message msg(TAG_QUIT, ""); 
      if(!conn.send(msg)) {
        std::cerr << "Error: Failed to receive quit response\n";
        return 1;
      }
      Message resp;
      if(!conn.receive(resp)) {
        std::cerr << "Error: Failed to receive leave response\n";
        return 1;
      }
      if(resp.tag == TAG_ERR) {
        std::cerr << resp.data << "\n";
      }
     
      break;
    } else{
      std::cerr << "Error: Unknown command '" << command << "'\n";
      continue;
    }
  }
  return 0;

}

