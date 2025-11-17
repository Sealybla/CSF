#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
  
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  if (m_fd < 0) {
    m_last_result = EOF_OR_ERROR;
    throw std::runtime_error("Failed to connect to server");
    return;
  }
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
  m_last_result = SUCCESS;

}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open()) {
    ::close(m_fd);
    m_fd = -1; 
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  if (m_fd != -1) {
    return true;
  } else {
    return false;
  }
}

void Connection::close() {
  // TODO: close the connection if it is open
  
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
