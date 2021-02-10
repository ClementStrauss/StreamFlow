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

  std::cout << StreamFlow::Factory::describe() << std::endl;

   Application app("put application name here");
  // add Component (or node) in the app
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

  return 0;
}
