#ifndef BUFFER_H
#define BUFFER_H

#include <condition_variable>
#include <cstdlib>
#include <cxxabi.h>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <typeinfo>

#include "ComponentExchangeQueue.h"
#include "DocumentedObject.h"

namespace StreamFlow {

enum IOBehavior { input, output, parameter, undefined };

static std::string behaviorNames[] = {"input", "output", "parameter",
                                      "undefined"};

static std::string demangle(const char *name) {
  int status = -4; // some arbitrary value to eliminate the compiler warning
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

  return (status == 0) ? res.get() : name;
}

struct IOInformation {
  IOBehavior behavior{undefined};
  bool connected{false};
  bool exposed{false};
  void *buffer_ptr = nullptr;
  std::string dataType = "";
  std::string name = "";
  size_t dataTypeHashCode;
};

class IO_base : public DocumentedObject {
public:
  // IO_base() = delete;

  bool isConnected() const { return info.connected; }
  bool isBufferValid() const { return buffer_ptr != nullptr; }
  bool isExposed() const { return info.exposed; }
  bool isUsable() const {
    return isConnected() && isBufferValid() && isExposed();
  }

  std::string doc() const override final {
    std::ostringstream oss;
    oss << DocumentedObject::doc() << " ";
    oss << "type=" << demangle(info.dataType.c_str())
        << ", dir=" << behaviorNames[info.behavior]
        << ", connected=" << std::to_string(info.connected)
        << ", ptr=" << info.buffer_ptr;
    return oss.str();
  }

  void markExposed(bool status = true) { info.exposed = status; }

  void connect(IO_base &other) {
    if (info.behavior == other.info.behavior && info.behavior != parameter)
      throw std::runtime_error(
          "error in " + std::string(__FUNCTION__) +
          " cannot connect (input,input) nor (output,ouput)");

    if (info.dataTypeHashCode != other.info.dataTypeHashCode)
      throw std::runtime_error("error in " + std::string(__FUNCTION__) +
                               " cannot connect " +
                               demangle(info.dataType.c_str()) + " with " +
                               demangle(other.info.dataType.c_str()));

    if (buffer_ptr != nullptr || other.buffer_ptr != nullptr) {
      if (other.buffer_ptr == nullptr)
        other.buffer_ptr = buffer_ptr;
      else
        buffer_ptr = other.buffer_ptr;

      info.connected = true;
      other.info.connected = true;
      info.buffer_ptr = buffer_ptr.get();
      other.info.buffer_ptr = buffer_ptr.get();
    } else {
      throw std::runtime_error("error in " + std::string(__FUNCTION__) +
                               " buffer \"" + doc() + " fails to connect to " +
                               other.doc());
    }
  }

  void operator&(IO_base &other) { connect(other); }

  void operator>>(IO_base &other) { connect(other); }

  void operator<<(IO_base &other) { connect(other); }

  void operator+(IO_base &other) { connect(other); }

protected:
  IOInformation info;
  std::shared_ptr<Queue_Base> buffer_ptr = nullptr;
};

template <typename T> class IO : public IO_base {
public:
  IO() = delete;
  IO(IOBehavior behavior, std::string name, std::string description) {
    setName(name);
    setDescription(description);
    info.behavior = behavior;
    info.dataType = typeid(T).name();
    info.dataTypeHashCode = typeid(T).hash_code();
  }

  T read() const {
    if (isUsable())
      return std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)
          ->read();
    else
      throw std::runtime_error("error: port not usable in " +
                               std::string(__FUNCTION__) + " buffer \"" +
                               name() + "\": not connected");
  }

  void write(T &item) {
    if (isUsable())
      std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)
          ->write(item);
    else
      throw std::runtime_error("error: port not usable in " +
                               std::string(__FUNCTION__) + " buffer \"" +
                               name() + "\": not connected");
  }

  void write(T &&item) {
    if (isUsable())
      std::static_pointer_cast<ComponentExchangeQueue<T>>(buffer_ptr)
          ->write(item);
    else
      throw std::runtime_error("error: port not usable in " +
                               std::string(__FUNCTION__) + " buffer \"" +
                               name() + "\": not connected");
  }

  //    std::string describe() const override
  //    {
  //        return name() + ":dir="+ std::to_string(info.direction) + ",usable="
  //        + std::to_string(isUsable());
  //    }

protected:
  void allocateBufferPtr(std::string name) {
    buffer_ptr = std::static_pointer_cast<Queue_Base>(
        std::make_shared<ComponentExchangeQueue<T>>(name));
    info.buffer_ptr = buffer_ptr.get();
  }
};

template <typename T> class Input : public IO<T> {
public:
  Input() {}
  Input(std::string name, std::string description)
      : IO<T>(input, name, description) {
    DocumentedObject::setName(name);
  }
};

template <typename T> class Output : public IO<T> {
public:
  Output() = delete;
  Output(std::string name, std::string description)
      : IO<T>(output, name, description) {
    DocumentedObject::setName(name);
    IO<T>::allocateBufferPtr(name);
  }
};

template <typename T> class Parameter : public IO<T> {
public:
  Parameter() = delete;
  Parameter(std::string name, std::string description)
      : IO<T>(parameter, name, description) {
    DocumentedObject::setName(name);
    IO<T>::allocateBufferPtr(name);
  }
};
} // namespace StreamFlow

#endif // BUFFER_H
