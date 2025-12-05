#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "message.h"
#include "guard.h"

MessageQueue::MessageQueue() {
  // Initialize the mutex
  pthread_mutex_init(&m_lock, nullptr);
  // Initialize the semaphore to 0 (no messages available)
  sem_init(&m_avail, 0, 0);
}

MessageQueue::~MessageQueue() {
  // Empty the queue and free all messages
  while (!m_messages.empty()) {
    Message *msg = m_messages.front();
    m_messages.pop_front();
    delete msg;
  }
  // Destroy the mutex
  pthread_mutex_destroy(&m_lock);
  // Destroy the semaphore
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // Acquire the lock to ensure thread-safe access to the queue
  Guard guard(m_lock);
  
  // Add the message to the queue
  m_messages.push_back(msg);
  
  // Lock is released here (Guard goes out of scope)
  // Notify any waiting thread that a message is available
  // NOTE: sem_post is called outside the critical section to allow
  // waiting receiver threads to wake up and acquire the lock
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  // Wait up to 1 second for a message to be available
  // If sem_timedwait returns -1, it timed out (ETIMEDOUT) and no message is available
  // NOTE: sem_timedwait is called OUTSIDE the critical section to avoid holding
  // the lock while waiting, which would prevent other threads from enqueuing messages
  if (sem_timedwait(&m_avail, &ts) == -1) {
    // Timeout occurred, no message available
    return nullptr;
  }

  // Acquire the lock to safely remove the message from the queue
  Guard guard(m_lock);
  
  // Remove the next message from the queue
  Message *msg = nullptr;
  if (!m_messages.empty()) {
    msg = m_messages.front();
    m_messages.pop_front();
  }
  
  // Lock is released here (Guard goes out of scope)
  return msg;
}
