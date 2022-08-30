#ifndef BUFFER_H
#define BUFFER_H

#include "I0_Base.h"

namespace StreamFlow {

template <typename T, IOBehavior behavior>
class IO : public IO_base {
 public:
  IO() = delete;
  IO(std::string name, std::string description) {
    setName(name);
    setDocString(description);
    info.behavior = behavior;
    info.dataType = typeid(T).name();
    info.dataTypeHashCode = typeid(T).hash_code();
  }

  T read() const {
    if (isUsable())
      return std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)->read();
    else
      throw std::runtime_error("error: port not usable in " + std::string(__FUNCTION__) + " buffer \"" + doc() + "\": not connected");
  }

  void write(T &item) {
    if (isUsable()) std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)->write(item);
    // else
    //   throw std::runtime_error("error: port not usable in " + std::string(__FUNCTION__) + " buffer \"" + doc() + "\": not connected");
  }

  void write(T &&item) {
    if (isUsable()) std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)->write(item);
    // else
    //   throw std::runtime_error("error: port not usable in " + std::string(__FUNCTION__) + " buffer \"" + doc() + "\": not connected");
  }

 protected:
  void allocateBufferPtr(std::string name) {
    buffer_ptr = std::static_pointer_cast<Queue_Base>(std::make_shared<ComponentExchangeQueue<T>>(name));
    info.buffer_ptr = buffer_ptr.get();
  }
};

template <typename T>
class Input : public IO<T, IOBehavior::input> {
 public:
  Input() {}
  Input(std::string name, std::string description) : IO<T, IOBehavior::input>(name, description) { DocumentedObject::setName(name); }
};

template <typename T>
class Output : public IO<T, IOBehavior::output> {
 public:
  Output() = delete;
  Output(std::string name, std::string description) : IO<T, IOBehavior::output>(name, description) {
    DocumentedObject::setName(name);
    IO<T, IOBehavior::output>::allocateBufferPtr(name);
  }
};

// TODO : this needs to be rewritten; probably via partial specialization of IO
template <typename T>
class Parameter : public IO<T, IOBehavior::parameter> {
 public:
  Parameter() = delete;
  Parameter(std::string name, std::string description) : IO<T, IOBehavior::parameter>(parameter, name, description) {
    DocumentedObject::setName(name);
    IO<T, IOBehavior::parameter>::allocateBufferPtr(name);
  }
};
}  // namespace StreamFlow

#endif  // BUFFER_H
