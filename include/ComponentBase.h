#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "ObjectBase.h"
#include "Ports.h"
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace StreamFlow
{

template<typename T>
class SmartPointer : public std::unique_ptr<T>
{
public:
  T& get_value() const { return *(std::unique_ptr<T>::get()); }
};

enum componentStatus
{
  running,
  done,
  paused,
  not_started

};

class ComponentBase : public ObjectBase
{
public:
  ComponentBase() = delete;
  ComponentBase(std::string name, std::string description)
  {
    setName(name);
    setDescription(description);
  }
  virtual ~ComponentBase();

  virtual void init() = 0;
  virtual void step() = 0;

  void exposeIO(StreamFlow::IO_base& io_ptr)
  {
    io_ptr.markExposed();
    portsMap[io_ptr.name()] = &io_ptr;
  }

  template<typename T>
  StreamFlow::Input<T> createInput(std::string desc)
  {
    StreamFlow::Input<T> port{ "", desc };
    exposeIO(port);
    return port;
  }

  std::string describe() const override
  {

    std::ostringstream oss;
    oss << ">>>" << std::endl;
    oss << ObjectBase::describe() << std::endl;
    oss << "Exposed ports :" << std::endl;
    for (auto& p : portsMap)
    {
      oss << p.second->describe() << std::endl;
    }
    oss << "<<<" << std::endl;
    return oss.str();
  }

  StreamFlow::IO_base* getFromMap(std::string key)
  {
    auto val_iter = portsMap.find(key);
    if (val_iter != portsMap.end())
      return portsMap[key];
    else
      throw std::out_of_range("port " + key + " unknown (check port name, and call to expose(port))");
  }

  StreamFlow::IO_base& operator[](std::string key) { return *getFromMap(key); }

  componentStatus status{ not_started };

private:
  std::map<std::string, StreamFlow::IO_base*> portsMap;
};

ComponentBase::~ComponentBase() {}
}

#endif // COMPONENTBASE_H
