#include <cassert>
#include <iostream>
#include <string>

#include "Application.h"
#include "ComponentFactory.h"
#include "Ports.h"

int main() {
  using namespace StreamFlow;

  // std::cout << StreamFlow::Factory::describe() << std::endl;

  Application app("put application name here");
  // add Component (or node) in the app
  app.addNode("WebcamProducer");
  app.addNode("MatDuplicator");
  app.addNode("sobelX");
  app.addNode("sobelY");

  // app.addNode("sobelY");
  app.addNode("grayscale");

  app.addNode("ImageDisplay");
  app.addComponent("ImageDisplay", "display2");

  // connect app's components together
  app["WebcamProducer"]["out"] >> app["grayscale"]["in"];
  app["grayscale"]["out"] >> app["MatDuplicator"]["in"];

  app["MatDuplicator"]["out1"] >> app["sobelX"]["in"];
  app["MatDuplicator"]["out2"] >> app["sobelY"]["in"];

  app["sobelX"]["out"] >> app["ImageDisplay"]["in"];
  app["sobelY"]["out"] >> app["display2"]["in"];

  std::cout << "Component documentation " << std::endl;
  std::cout << StreamFlow::Factory::create("ProducerComponentINT")->doc() << std::endl;

  ////  app.addNode("ProducerComponent");
  ////  app.addNode("ConsumerComponent");
  ////  app["ProducerComponent"]["out"] >> app["ConsumerComponent"]["in"];

  //  // ask for component port documentation
  //  std::cout << app["WebcamProducer"]["out"].doc() << std::endl;
  //  std::cout << app["ImageDisplay"]["in"].doc() << std::endl;

  //  // describe the application
  //  std::cout << app.doc() << std::endl;

  //  // describe a component
  // app["WebcamProducer"].doc() << std::endl;

  // run the application, enjoy pipeline and parallelism;
  app.run();

  return 0;
}
