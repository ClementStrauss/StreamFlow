#ifndef REGISTEREDCOMPONENT_H
#define REGISTEREDCOMPONENT_H

#include "ComponentBase.h"
#include "ComponentFactory.h"
namespace StreamFlow {
template <class T> class RegisteredComponent : public ComponentBase {
public:
  RegisteredComponent() = delete;
  RegisteredComponent(std::string name, std::string description)
      : ComponentBase(name, description) {}

public:
  static FactoryRegistrar<T> registrar;

  static int registerInFactory(std::string name) {
    registrar = FactoryRegistrar<T>(name);
    return 0;
  }
};
} // namespace StreamFlow
#endif // REGISTEREDCOMPONENT_H
