#pragma once

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "registeredComponent.h"

namespace StreamFlow {

class ConsumerComponent : public FactoryRegisteredComponent<ConsumerComponent> {
 public:
  ConsumerComponent() : FactoryRegisteredComponent("consumer", "This is a consumer") { exposeIO(in); }

  void init() override {}
  void step() override { consume(); }

 private:
  void consume() {
    auto value = in.read();
    std::cout << name() + " reads " << value.operator*() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    if (*value > 80) status = componentStatus::done;
  }

  StreamFlow::Input<std::unique_ptr<int>> in{"in", "consume int every 0.1 second and cout value"};
};

class ConsumerComponentINT : public FactoryRegisteredComponent<ConsumerComponentINT> {
 public:
  ConsumerComponentINT() : FactoryRegisteredComponent("consumerINT", "This is a consumer") { exposeIO(in); }

  void init() override {}
  void step() override { consume(); }

 private:
  void consume() {
    auto value = in.read();
    std::cout << name() + " reads " << value << std::endl;
    //  std::this_thread::sleep_for(std::chrono::microseconds(5000));
    //          if(value > 8)
    //              break;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    if (value > 80) status = componentStatus::done;
  }

  StreamFlow::Input<int> in{"in", "consume int every 0.1 second and cout value"};
};

// REGISTER_IN_FACTORY(ConsumerComponentINT)

template <typename T>
class ConsumerComponentTemplate : public FactoryRegisteredComponent<ConsumerComponentTemplate<T>> {
 public:
  ConsumerComponentTemplate() : FactoryRegisteredComponent<ConsumerComponentTemplate<T>>("consumer", "This is a consumer") { ComponentBase::exposeIO(in); }

  ConsumerComponentTemplate(std::string name) : FactoryRegisteredComponent<ConsumerComponentTemplate<T>>(name, "This is a consumer") {
    DocumentedObject::setName(name);
    ComponentBase::setName(name);
    ComponentBase::exposeIO(in);
  }

  void init() override {}
  void step() override { consume(); }

 private:
  void consume() {
    auto value = in.read();
    std::cout << ComponentBase::name() + " reads " << value << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    //          if(value > 8)
    //              break;
  }

  StreamFlow::Input<T> in{"in", "consume int every 0.1 second and cout value"};
};

}  // namespace StreamFlow
