#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <cassert>
#include <memory>

#include "ComponentBase.h"
#include "DocumentedObject.h"

namespace StreamFlow {

using Key = std::string;
using Object = std::unique_ptr<StreamFlow::ComponentBase>;
using Creator = std::function<Object()>;

class Factory {
 public:
  void static registerNewCreator(Key const &key, Creator const &creator);

  Object static create(Key const &key);

  boolean static canBeCreated(Key const &key);

  std::string static describe();

 private:
  static std::unordered_map<Key, Creator> &m_creators();
};

template <typename T>
class FactoryRegistrar {
 public:
  FactoryRegistrar(std::string key) {
    factoryKey = key;
    Factory::registerNewCreator(key, []() { return std::make_unique<T>(); });
  }
  std::string factoryKey;
};

template <typename T>
class FactoryRegistrerWithName {
 public:
  FactoryRegistrerWithName(std::string key) {
    Factory::registerNewCreator(key, [key]() { return std::make_unique<T>(key); });
  }
};

#define ALIGNOF(...) alignof(__VA_ARGS__)

#define REGISTER_IN_FACTORY(C) \
  template <>                  \
  FactoryRegistrar<C> C::FactoryRegisteredComponent<C>::registrar = FactoryRegistrar<C>(#C);

#define REGISTER_IN_FACTORY_WITH_NAME(C, _NAME) \
  template <>                                   \
  FactoryRegistrar<C> C::FactoryRegisteredComponent<C>::registrar = FactoryRegistrar<C>(#_NAME);

}  // namespace StreamFlow
#endif  // COMPONENTFACTORY_H
