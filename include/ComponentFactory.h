#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "ComponentBase.h"
#include "DocumentedObject.h"
#include <cassert>
#include <memory>

namespace StreamFlow {

using Key = std::string;
using Object = std::unique_ptr<StreamFlow::ComponentBase>;
using Creator = std::function<Object()>;

class Factory {
public:
  void static registerNewCreator(Key const &key, Creator const &creator);
  Object static create(Key const &key);
  std::string static describe();

private:
  static std::unordered_map<Key, Creator> &m_creators();
};

// std::unordered_map<Key, Creator> Factory::m_creators;

template <typename T> class FactoryRegistrar {
public:
  FactoryRegistrar(std::string key) {
    Factory::registerNewCreator(key, []() { return std::make_unique<T>(); });
  }
};

template <typename T> class FactoryRegistrerWithName {
public:
  FactoryRegistrerWithName(std::string key) {
    Factory::registerNewCreator(key,
                                [key]() { return std::make_unique<T>(key); });
  }
};

#define REGISTER_IN_FACTORY_OLD(C)                                             \
  static FactoryRegistrar<C> C##FACTORY(C().name());
#define REGISTER_IN_FACTORY_WITH_NAME_OLD(C, _NAME)                            \
  static FactoryRegistrerWithName<C> _NAME##FACTORY_WITH_NAME(#_NAME);

#define REGISTER_IN_FACTORY(C)                                                 \
  template <>                                                                  \
  FactoryRegistrar<C> C::RegisteredComponent<C>::registrar =                   \
      FactoryRegistrar<C>(#C);

#define REGISTER_IN_FACTORY_WITH_NAME(C, _NAME)                                \
  template <>                                                                  \
  FactoryRegistrar<C> C::RegisteredComponent<C>::registrar =                   \
      FactoryRegistrar<C>(#_NAME);

} // namespace StreamFlow
#endif // COMPONENTFACTORY_H
