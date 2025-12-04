#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // Initialize the mutex
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // Destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // Acquire the lock to ensure thread-safe modification of the member set
  Guard guard(lock);
  
  // Add the user to the room
  members.insert(user);
}

void Room::remove_member(User *user) {
  // Acquire the lock to ensure thread-safe modification of the member set
  Guard guard(lock);
  
  // Remove the user from the room
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // Acquire the lock to ensure thread-safe iteration over the member set
  // This prevents join/leave operations from occurring during broadcast
  Guard guard(lock);
  
  // Send a message to every receiver User in the room
  for (UserSet::iterator it = members.begin(); it != members.end(); ++it) {
    User *user = *it;
    // Create a heap-allocated message to send to the receiver
    // Format: "sender_username:message_text"
    Message *msg = new Message(TAG_DELIVERY, sender_username + ":" + message_text);
    // Enqueue the message to the user's message queue
    // NOTE: enqueue() has its own synchronization for the queue
    user->mqueue.enqueue(msg);
  }
  
  // Lock is released here (Guard goes out of scope)
}
