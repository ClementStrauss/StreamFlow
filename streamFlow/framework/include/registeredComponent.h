#ifndef REGISTEREDCOMPONENT_H
#define REGISTEREDCOMPONENT_H

#include "ComponentBase.h"
#include "ComponentFactory.h"
namespace StreamFlow {
template <class T>
class FactoryRegisteredComponent : public ComponentBase {
 public:
  FactoryRegisteredComponent() = delete;
  FactoryRegisteredComponent(std::string name, std::string description) : ComponentBase(name, description) {}

 public:
  static FactoryRegistrar<T> registrar;

  static int registerInFactory(std::string name) {
    registrar = FactoryRegistrar<T>(name);
    return 0;
  }

  std::string name() const noexcept override { return ComponentBase::name() + "/" + registrar.factoryKey; }
};
}  // namespace StreamFlow
#endif  // REGISTEREDCOMPONENT_H
