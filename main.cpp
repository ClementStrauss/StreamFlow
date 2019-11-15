#include <cassert>
#include <iostream>
#include <string>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "allComponentIncludes.h"

#include "Application.h"

int
main()
{
  using namespace StreamFlow;

  //    auto producer_ptr = Factory::make_ptr("producer");
  //    auto consumer_ptr = Factory::make_ptr("consumer");

  //    //connect component together
  //    (*producer_ptr)["out"].connect((*consumer_ptr)["in"]);

  //    //connect component together with operator
  //    (*producer_ptr)["out"]+(*consumer_ptr)["in"];

  //    // spawn new thread
  //    std::thread firstp (&RLNM::ComponentBase::loop, producer_ptr.get());
  //    std::thread secondc (&RLNM::ComponentBase::loop, consumer_ptr.get());
  //    firstp.join();
  //    secondc.join();

  std::cout << StreamFlow::Factory::describe() << std::endl;

  //   return 1;

  Application app("appName");
  app.add_component("producer");
  app.add_component("consumer");
  app["producer"]["out"] >> app["consumer"]["in"];

  app.add_component("producerINT");
  app.add_component("consumerINT");
  app["producerINT"]["out"] >> app["consumerINT"]["in"];

  std::cout << app["producer"]["out"].describe() << std::endl;
  std::cout << app["consumer"]["in"].describe() << std::endl;
  std::cout << app.describe() << std::endl;
  std::cout << app["producer"].describe() << std::endl;

  //    return 1;
  app.run();

  //    RLNM::producerComponent producerComponent;
  //    RLNM::consumerComponent consumerComponent;

  //    producerComponent["out"]+(consumerComponent["in"]);

  //    std::cout << producerComponent.describe() << std::endl;
  //    std::cout << consumerComponent.describe() << std::endl;

  //    producerComponent.out.write(10000);
  //    auto value = consumerComponent.in.read();
  //    std::cout << "read value " << value << std::endl;

  //    std::thread first (&RLNM::producerComponent::loop, &producerComponent);
  //    std::thread second (&RLNM::consumerComponent::loop, &consumerComponent);     // spawn new thread that calls foo()
  //    second.join();
  //    first.join();

  return 0;
}
