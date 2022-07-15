#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <graphviz/types.h>

#include <cstddef>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "DocumentedObject.h"
#include "Ports.h"

namespace StreamFlow {

// forward declaration

enum componentStatus { running, done, paused, not_started };

class ComponentBase : public DocumentedObject {
 public:
  ComponentBase() = delete;
  ComponentBase(std::string name, std::string description) {
    setName(name);
    setDocString(description);
  }
  virtual ~ComponentBase() override {
    portsMap.clear();
    std::cout << "ComponentBase: deleted " << name() << std::endl;
  }

  virtual void init() = 0;
  virtual void step() = 0;

  void exposeIO(ComponentBase &component, StreamFlow::IO_base &io_ptr) {
    io_ptr.markExposed();
    portsMap[io_ptr.name()] = &io_ptr;
    io_ptr.info.componentName = name();
  }

  template <typename T>
  StreamFlow::Input<T> createInput(ComponentBase &component, std::string aname, std::string desc) {
    StreamFlow::Input<T> port{aname, desc};
    exposeIO(component, port);
    return port;
  }

  template <typename T>
  StreamFlow::Output<T> createOutput(ComponentBase &component, std::string aname, std::string desc) {
    StreamFlow::Output<T> port{aname, desc};
    exposeIO(component, port);
    return port;
  }

  std::string doc(bool verbose = false) const override {
    std::ostringstream oss;
    oss << "<" << name() << ">" << std::endl;
    oss << DocumentedObject::doc() << std::endl;
    oss << "Exposed ports :" << std::endl;
    for (auto &p : portsMap) {
      oss << p.second->doc() << std::endl;
    }
    oss << "</" << name() << ">" << std::endl;
    return oss.str();
  }

  StreamFlow::IO_base *getFromMap(std::string key) {
    auto val_iter = portsMap.find(key);
    if (val_iter != portsMap.end())
      return portsMap[key];
    else
      throw std::out_of_range("port " + key + " unknown (check port name, and call to expose(port))");
  }

  void setName(std::string aName) noexcept {
    DocumentedObject::setName(aName);
    for (auto &port : portsMap) {
      port.second->info.componentName = aName;
    }
  }

  StreamFlow::IO_base &operator[](std::string key) { return *getFromMap(key); }

  componentStatus status{not_started};

  static int reg;

 private:
  std::map<std::string, StreamFlow::IO_base *> portsMap;
};
}  // namespace StreamFlow

#endif  // COMPONENTBASE_H
