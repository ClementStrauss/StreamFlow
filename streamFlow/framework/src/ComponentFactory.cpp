#include "ComponentFactory.h"

#include "allComponentIncludes.h"

namespace StreamFlow {

std::unordered_map<Key, Creator> &Factory::m_creators() {
  static std::unordered_map<Key, Creator> Impl;
  return Impl;
}

void Factory::registerNewCreator(Key const &key, Creator const &creator) {
  std::cout << "Registers " << key << std::endl;
  assert(m_creators().count(key) == 0);
  assert(creator);
  m_creators().emplace(key, creator);
}

Object Factory::create(Key const &key) {
  std::cout << "create " << key << std::endl;
  assert(m_creators().count(key) == 1);
  assert(m_creators()[key]);
  return m_creators()[key]();
}

std::string Factory::describe() {
  std::ostringstream oss;
  oss << "+++++++++++++++++++++++++++FACTORY++++++++++++++++++++++++++++" << std::endl;
  for (auto &componnentCreator : m_creators()) {
    oss << "registered with key: \"" + componnentCreator.first + "\" :" << std::endl;
    auto comp = componnentCreator.second();
    oss << comp->doc();
  }
  oss << "+++++++++++++++++++++++++END FACTORY+++++++++++++++++++++++++++" << std::endl;

  return oss.str();
}

}  // namespace StreamFlow
