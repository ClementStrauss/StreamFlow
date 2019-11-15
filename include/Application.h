#pragma once

#include <map>
#include <pthread.h>
#include <sstream>
#include <string>
#include <thread>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "ObjectBase.h"

namespace StreamFlow
{

class Application final : public ObjectBase
{
public:
  Application(std::string name = "application", std::string desc = "empty description")
  {
    ObjectBase::setName(name);
    ObjectBase::setDescription(desc);
#ifdef __linux__
    pthread_setname_np(pthread_self(), name.c_str()); // set the name, visible in htop
#endif
  }

  std::string describe() const override { return ObjectBase::describe(); }

  void add_component(std::string factoryKey) { add_component(factoryKey, factoryKey); }

  void add_component(std::string factoryKey, std::string name)
  {
    if (instanciated_components_map.count(name) != 0)
      throw std::runtime_error(factoryKey + " already created with name \"" + name + "\" : cannot create two components with the same name");

    instanciated_components_map[name] = StreamFlow::Factory::make_ptr(factoryKey);
    instanciated_components_map[name]->setName(name);
  }

  StreamFlow::ComponentBase& operator[](std::string key)
  {
    if (instanciated_components_map.count(key) == 0)
      throw std::runtime_error(key + " component does not exit");

    return *(instanciated_components_map[key]);
  }

  void run()
  {
    for (auto& component : instanciated_components_map)
    {
      threads[component.first] = std::thread([&]() {
        component.second.get()->status = componentStatus::running;
        while (component.second.get()->status == componentStatus::running)
          component.second.get()->step();
      });
#ifdef __linux__
      pthread_setname_np(threads[component.first].native_handle(), component.second->name().c_str());
#endif
    }
  }

  void join_all()
  {
    for (auto& thread : threads)
    {
      thread.second.join();
    }
  }

  ~Application()
  {
    join_all();
    instanciated_components_map.clear();
    threads.clear();
  }

private:
  std::map<std::string, std::unique_ptr<StreamFlow::ComponentBase>> instanciated_components_map;
  std::map<std::string, std::thread> threads;
};

} // namespace StreamFlow
