#ifndef COMPONENTEXCHANGEQUEUE_H
#define COMPONENTEXCHANGEQUEUE_H

#include "DocumentedObject.h"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

namespace StreamFlow {

class Queue_Base : public DocumentedObject {
public:
  virtual size_t size() = 0;
};

template <typename T, int maxSize = 8>
class ComponentExchangeQueue : public Queue_Base {
public:
  T read() {
    std::unique_lock<std::mutex> mlock(mutex_);
    // always check if readable and wait for signal if necessary
    readableCondition_.wait(mlock, [&] { return !queue_.empty(); });

    auto val = std::move(queue_.front());
    queue_.pop();

    if (queue_.size() + 1 == maxSize) {
      mlock.unlock();
      writableCondition_.notify_all();
    }
    return val;
  }

  void write(T &item) {
    std::unique_lock<std::mutex> mlock(mutex_);
    // always check if writable and wait for signal if necessary
    writableCondition_.wait(mlock, [&] { return queue_.size() < maxSize; });
    queue_.push(std::move(item));
    mlock.unlock();
    readableCondition_.notify_all();
  }

  size_t size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

  ComponentExchangeQueue(std::string name) { setName(name); }
  ComponentExchangeQueue(const ComponentExchangeQueue &) =
      delete; // disable copying
  ComponentExchangeQueue &
  operator=(const ComponentExchangeQueue &) = delete; // disable assignment
  ~ComponentExchangeQueue() {
    std::cout << "delete buffer" << this->name() << std::endl;
  }

private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
  std::condition_variable readableCondition_;
  std::condition_variable writableCondition_;
};

// class decorator with extended logs
template <typename T, int maxSize = 8>
class ComponentExchangeQueueDebug : public ComponentExchangeQueue<T, maxSize> {
public:
  using Base = ComponentExchangeQueue<T, maxSize>;
  ComponentExchangeQueueDebug() {
    std::cout << "create queue " << this << std::endl;
    Base();
  }
  ~ComponentExchangeQueueDebug() {
    std::cout << "delete queue " << this << std::endl;
  }

  T read() {
    std::cout << "try read queue " << this << " occupation(before)= " << size()
              << std::endl;
    return Base::read();
    std::cout << "succes read queue " << this
              << " occupation(after)= " << Base::size() << std::endl;
  }

  void write(const T &item) {
    std::cout << "try write queue " << this
              << " occupation(before)= " << Base::size() << std::endl;
    Base::write(item);
    std::cout << "succes write queue " << this
              << " occupation(after)= " << Base::size() << std::endl;
  }

  size_t size() { return Base::size(); }
};
} // namespace StreamFlow

#endif // COMPONENTEXCHANGEQUEUE_H
