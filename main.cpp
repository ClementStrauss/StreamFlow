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

  Application app("put application name here");
  // create Component (or node) in the app
  app.addNode("producer");
  app.addNode("consumer");

  // connect app's components together
  app["producer"]["out"] >> app["consumer"]["in"];

  app.addNode("producerINT");
  app.addNode("consumerINT");
  app["producerINT"]["out"] >> app["consumerINT"]["in"];

  // ask for component port documentation
  std::cout << app["producer"]["out"].doc() << std::endl;
  std::cout << app["consumer"]["in"].doc() << std::endl;

  // describe the application
  std::cout << app.doc() << std::endl;

  // describe a component
  std::cout << app["producer"].doc() << std::endl;

  // run the application, enjoy pipeline and parallelism;
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
