#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "ComponentBase.h"
#include "DocumentedObject.h"
#include <cassert>

namespace StreamFlow
{

using Key = std::string;
using Object = std::unique_ptr<StreamFlow::ComponentBase>;
using Creator = std::function<Object()>;

class Factory
{
public:
  void static registerNewCreator(Key const& key, Creator const& creator)
  {
    std::cout << "register " << key << std::endl;
    assert(m_creators.count(key) == 0);
    assert(creator);
    m_creators.emplace(key, creator);
  }

  Object static create(Key const& key)
  {
    assert(m_creators.count(key) == 1);
    assert(m_creators[key]);
    return m_creators[key]();
  }

  static std::string describe()
  {
    std::ostringstream oss;
    oss << "+++++++++++++++++++++++++++FACTORY++++++++++++++++++++++++++++" << std::endl;
    for (auto componnentCreator : m_creators)
    {
      oss << "registered with key: \"" + componnentCreator.first + "\" :" << std::endl;
      auto comp = componnentCreator.second();
      oss << comp->describe();
    }
    oss << "+++++++++++++++++++++++++END FACTORY+++++++++++++++++++++++++++" << std::endl;

    return oss.str();
  }

private:
  static std::unordered_map<Key, Creator> m_creators;
};

std::unordered_map<Key, Creator> Factory::m_creators;

template<typename T>
class FactoryRegistrer
{
public:
  FactoryRegistrer(std::string key)
  {
    Factory::registerNewCreator(key, []() { return std::make_unique<T>(); });
  }
};

template<typename T>
class FactoryRegistrerWithName
{
public:
  FactoryRegistrerWithName(std::string key)
  {
    Factory::registerNewCreator(key, [key]() { return std::make_unique<T>(key); });
  }
};

#define REGISTER_IN_FACTORY(C) static FactoryRegistrer<C> C##FACTORY(C().name());
#define REGISTER_IN_FACTORY_WITH_NAME(C, _NAME) static FactoryRegistrerWithName<C> _NAME##FACTORY_WITH_NAME(#_NAME);
}
#endif // COMPONENTFACTORY_H
