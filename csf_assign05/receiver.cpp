#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  try {
    conn.connect(server_hostname, server_port);
  } catch (const std::runtime_error &e) {
    std::cerr << "Connection error: " << e.what() << "\n";
    return 1;
  }
  //check if successfully opened
  if (!conn.is_open()) {
    std::cerr << "Failed to connect to server\n";
    return 1;
  }
  
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin_msg(TAG_RLOGIN, username);
  if(!conn.send(rlogin_msg)) {
    std::cerr << "Failed to send rlogin message\n";
    return 1;
  }
  //wait for the login resposne

  Message rlogin_resp;
  if(!conn.receive(rlogin_resp)) {
    std::cerr << "Failed to recieve rlogin response\n";
    return 1;
  }

  if (rlogin_resp.tag == TAG_ERR) {
    std::cerr << rlogin_resp.data << "\n";
    return 1;  
  }
  Message join_msg(TAG_JOIN, room_name);
  if(!conn.send(join_msg)) {
    std::cerr << rlogin_resp.data << "\n";
    return 1;
  }

  Message join_resp;
  if(!conn.receive(join_resp)) {
    std::cerr << "Failed to recieve join response\n";
    return 1;
  }

  if(join_resp.tag == TAG_ERR) {
    std::cerr << join_resp.data << std::endl;
    return 1;
  }
  while(true) {
    Message delivery_msg;
    if(!conn.receive(delivery_msg)) {
      break;
    }
    if(delivery_msg.tag == TAG_OK) continue;
    if (delivery_msg.tag == TAG_ERR) {
      std::cerr << delivery_msg.data << "\n";
      return 1;
    }
    if(delivery_msg.tag == TAG_DELIVERY) {
      std::string payload = delivery_msg.data;

    
    size_t first_colon = payload.find(':');
    if(first_colon == std::string::npos) continue;
    
    size_t second_colon = payload.find(':', first_colon + 1);
    if(second_colon == std::string::npos) continue;

    std::string sender = payload.substr(first_colon + 1, second_colon - first_colon - 1);
    std::string message = payload.substr(second_colon +1);

    std::cout << sender << ": " << message << "\n";
    }
  }
  /*
  if (join_resp.get_tag() != TAG_JOIN_SUCCESS) {
    std::cerr << "JOIN failed: " << join_resp.get_text() << std::endl;
    return 1;
  }
  */

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}
