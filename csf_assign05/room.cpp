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
  // Hold the room lock while enqueuing messages to ensure that
  // a member will not be removed and deleted while we are notifying
  // them. This is a conservative approach that trades some
  // concurrency for correctness and simplicity.
  Guard guard(lock);
  for (User *user : members) {
    // Create a heap-allocated message for this recipient and enqueue it.
    Message *msg = new Message(TAG_DELIVERY, room_name + ":" + sender_username + ":" + message_text);
    user->mqueue.enqueue(msg);
  }
}
