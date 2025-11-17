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
    std::cerr << "Connection error: " << e.what() << std::endl;
    return 1;
  }


  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin_msg(TAG_RLOGIN);
  rlogin_msg.set_username(username);
  conn.send_message(rlogin_msg);
  Message rlogin_resp = conn.receive_message();
  if (rlogin_resp.get_tag() != TAG_RLOGIN_SUCCESS) {
    std::cerr << "RLOGIN failed: " << rlogin_resp.get_text() << std::endl;
    return 1;
  }
  Message join_msg(TAG_JOIN);
  join_msg.set_room_name(room_name);
  conn.send_message(join_msg);
  Message join_resp = conn.receive_message();
  if (join_resp.get_tag() != TAG_JOIN_SUCCESS) {
    std::cerr << "JOIN failed: " << join_resp.get_text() << std::endl;
    return 1;
  }
  

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}
