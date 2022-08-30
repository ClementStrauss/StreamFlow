#pragma once

#include <cxxabi.h>

#include <condition_variable>
#include <cstdlib>
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
#include "GraphGenerator.h"

namespace StreamFlow {

class ComponentBase;

enum IOBehavior { input, output, always_overwrite_output, parameter, undefined };

static std::string behaviorNames[] = {"input", "output", "always_overwrite_output", "parameter", "undefined"};

static std::string demangle(const char *name) {
  int status = -4;  // some arbitrary value to eliminate the compiler warning
  std::unique_ptr<char, void (*)(void *)> res{abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

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
  std::string componentName = "";
};

class IO_base : public DocumentedObject {
 public:
  // IO_base() = delete;

  bool isConnected() const { return info.connected; }
  bool isBufferValid() const { return buffer_ptr != nullptr; }
  bool isExposed() const { return info.exposed; }
  bool isUsable() const { return isConnected() && isBufferValid() && isExposed(); }
  size_t getElementNumber() { return buffer_ptr->size(); }

  std::string doc(bool verbose = false) const override final {
    std::ostringstream oss;
    oss << behaviorNames[info.behavior] << ": " << DocumentedObject::doc() << "\n";
    if (verbose) oss << "type=" << demangle(info.dataType.c_str()) << ", connected=" << std::to_string(info.connected) << ", ptr=" << info.buffer_ptr;

    return oss.str();
  }

  void markExposed(bool status = true) { info.exposed = status; }

  void connect(IO_base &other) {
    if (info.behavior == other.info.behavior && info.behavior != parameter)
      throw std::runtime_error("error in " + std::string(__FUNCTION__) + " cannot connect (input,input) nor (output,ouput)");

    if (info.dataTypeHashCode != other.info.dataTypeHashCode)
      throw std::runtime_error("error in " + std::string(__FUNCTION__) + " cannot connect " + demangle(info.dataType.c_str()) + " with " +
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
      throw std::runtime_error("error in " + std::string(__FUNCTION__) + " buffer \"" + doc() + " fails to connect to " + other.doc());
    }

    grapvizConnect(other);
  }

  void grapvizConnect(IO_base &other) { GraphGenerator::instance().addEdge(this->info.componentName, other.info.componentName, doc()); };

  void operator&(IO_base &other) { connect(other); }

  void operator>>(IO_base &other) { connect(other); }

  void operator<<(IO_base &other) { connect(other); }

  void operator+(IO_base &other) { connect(other); }

  IOInformation info;

 protected:
  std::shared_ptr<Queue_Base> buffer_ptr = nullptr;
};

}  // namespace StreamFlow