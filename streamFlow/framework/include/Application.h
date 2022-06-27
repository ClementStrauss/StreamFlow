#pragma once

#include <pthread.h>

#include <map>
#include <sstream>
#include <string>
#include <thread>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "DocumentedObject.h"
#include "GraphGenerator.h"
#include "GraphViz.h"

namespace StreamFlow {
/**
 * @brief Application class, holding the component graph
 *
 */
class Application final : public DocumentedObject {
 public:
  Application(std::string name = "application", std::string desc = "empty description") {
    DocumentedObject::setName(name);
    DocumentedObject::setDocString(desc);

#ifdef __linux__
    pthread_setname_np(pthread_self(),
                       name.c_str());  // set the name, visible in htop
#endif
  }

  /**
   * @brief Add a component into the application, create component with alias
   *
   * @param factoryKey identifier of the component to add
   * @param instanceAlias alias name used to acces the instancianted component
   */
  void addComponentAs(std::string factoryKey, std::string instanceAlias) {
    if (instanciated_components_map.count(instanceAlias) != 0)
      throw std::runtime_error(factoryKey + " already created with name \"" + instanceAlias + "\" : cannot create two components with the same name");

    instanciated_components_map[instanceAlias] = StreamFlow::Factory::create(factoryKey);
    instanciated_components_map[instanceAlias]->setName(instanceAlias);
    GraphGenerator::instance().addNode(instanceAlias, instanciated_components_map[instanceAlias]->doc());
  }

  /**
   * @brief Add a component into the application
   *
   * @param factoryKey : identifier of the component, used also to acces the instancianted component
   */
  void addComponent(std::string factoryKey) { addComponentAs(factoryKey, factoryKey); }

  /**
   * @brief Acces a component within the app. Usually to build the application's graph
   * If the component does not exist yet it is auto-added to the application
   *
   * @param key : identifier key of the component
   * @return StreamFlow::ComponentBase& : the instanciated component within the application
   */
  StreamFlow::ComponentBase &operator[](std::string key) {
    if (instanciated_components_map.count(key) == 0) {
      if (StreamFlow::Factory::canBeCreated(key)) {
        addComponent(key);
      } else {
        throw std::runtime_error(key + " component does not exit");
      }
    }

    std::cout << "acces " + key << std::endl;
    (instanciated_components_map[key])->setName(key);
    return *(instanciated_components_map[key]);
  }

  /**
   * @brief Start the application by starting all compoenents of the application graph
   *
   */
  void start() {
    GraphGenerator::instance().draw();
    for (auto &component : instanciated_components_map) {
      std::cout << "init of " << component.first << std::endl;
      component.second.get()->init();
    }

    for (auto &component : instanciated_components_map) {
      threads[component.first] = std::thread([&]() {
        component.second.get()->status = componentStatus::running;
        while (component.second.get()->status == componentStatus::running) component.second.get()->step();

        std::cout << "end of " << component.first << std::endl;
        component.second.~unique_ptr();
      });
#ifdef __linux__
      pthread_setname_np(threads[component.first].native_handle(), component.second->name().c_str());
#endif
    }
  }

  ~Application() {
    join_all();
    instanciated_components_map.clear();
    threads.clear();
  }

 private:
  void join_all() {
    for (auto &thread : threads) {
      thread.second.join();
    }
  }

  std::map<std::string, std::unique_ptr<StreamFlow::ComponentBase>> instanciated_components_map;
  std::map<std::string, std::thread> threads;
  // initializes instace of a graphviz graph
};

}  // namespace StreamFlow
