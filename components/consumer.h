#pragma once

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"

namespace StreamFlow
{

class ConsumerComponent : public ComponentBase
{
public:
  ConsumerComponent()
    : ComponentBase("consumer", "This is a consumer")
  {
    exposeIO(in);
  }

  void init() override {}
  void step() override { consume(); }

  void consume()
  {

    auto value = in.read();
    std::cout << name() + " reads " << value.operator*() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    if (*value > 80)
      status = componentStatus::done;
  }

  StreamFlow::Input<std::unique_ptr<int>> in{ "in", "consume int every 0.1 second and cout value" };
};

REGISTER_IN_FACTORY(ConsumerComponent);

class ConsumerComponentINT : public ComponentBase
{
public:
  ConsumerComponentINT()
    : ComponentBase("consumerINT", "This is a consumer")
  {
    exposeIO(in);
  }

  void init() override {}
  void step() override { consume(); }

  void consume()
  {

    auto value = in.read();
    std::cout << name() + " reads " << value << std::endl;
    //  std::this_thread::sleep_for(std::chrono::microseconds(5000));
    //          if(value > 8)
    //              break;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    if (value > 80)
      status = componentStatus::done;
  }

  StreamFlow::Input<int> in{ "in", "consume int every 0.1 second and cout value" };
};

REGISTER_IN_FACTORY(ConsumerComponentINT);

template<typename T>
class ConsumerComponentTemplate : public ComponentBase
{
public:
  ConsumerComponentTemplate()
    : ComponentBase("consumer", "This is a consumer")
  {
    exposeIO(&in);
  }

  ConsumerComponentTemplate(std::string name)
    : ComponentBase(name, "This is a consumer")
  {
    setName(name);
    exposeIO(in);
  }

  void init() override {}
  void step() override { consume(); }

  void consume()
  {

    auto value = in.read();
    std::cout << name() + " reads " << value << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    //          if(value > 8)
    //              break;
  }

  StreamFlow::Input<T> in{ "in", "consume int every 0.1 second and cout value" };
};

REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<float>, consumerFloat);
REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<double>, consumerDouble);
REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<short>, consumerShort);
}
