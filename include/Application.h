#pragma once

#include <map>
#include <pthread.h>
#include <sstream>
#include <string>
#include <thread>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "DocumentedObject.h"

namespace StreamFlow {

class Application final : public DocumentedObject {
public:
  Application(std::string name = "application",
              std::string desc = "empty description") {
    DocumentedObject::setName(name);
    DocumentedObject::setDescription(desc);
#ifdef __linux__
    pthread_setname_np(pthread_self(),
                       name.c_str()); // set the name, visible in htop
#endif
  }

  std::string doc() const override { return DocumentedObject::doc(); }

  void addComponent(std::string factoryKey, std::string instanceName) {
    if (instanciated_components_map.count(instanceName) != 0)
      throw std::runtime_error(
          factoryKey + " already created with name \"" + instanceName +
          "\" : cannot create two components with the same name");

    instanciated_components_map[instanceName] =
        StreamFlow::Factory::create(factoryKey);
    instanciated_components_map[instanceName]->setName(instanceName);
  }

  void addNode(std::string factoryKey) { addComponent(factoryKey, factoryKey); }

  StreamFlow::ComponentBase &operator[](std::string key) {
    if (instanciated_components_map.count(key) == 0)
      throw std::runtime_error(key + " component does not exit");

    return *(instanciated_components_map[key]);
  }

  void run() {
    for (auto &component : instanciated_components_map) {
      threads[component.first] = std::thread([&]() {
        component.second.get()->status = componentStatus::running;
        while (component.second.get()->status == componentStatus::running)
          component.second.get()->step();

        std::cout << "end of " << component.first << std::endl;
        component.second.~unique_ptr();
      });
#ifdef __linux__
      pthread_setname_np(threads[component.first].native_handle(),
                         component.second->name().c_str());
#endif
    }
  }

  void join_all() {
    for (auto &thread : threads) {
      thread.second.join();
    }
  }

  ~Application() {
    join_all();
    instanciated_components_map.clear();
    threads.clear();
  }

private:
  std::map<std::string, std::unique_ptr<StreamFlow::ComponentBase>>
      instanciated_components_map;
  std::map<std::string, std::thread> threads;
};

} // namespace StreamFlow
