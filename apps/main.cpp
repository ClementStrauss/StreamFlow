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

  // add Component (or node) in the app with alias
  // app.addComponentAs("ImageDisplay", "ImageDisplay2");

  // connect app's components together
  app["WebcamProducer"]["out"] >> app["grayscale"]["in"];
  app["grayscale"]["out"] >> app["MatDuplicator"]["in"];

  app["MatDuplicator"]["out1"] >> app["sobelX"]["in"];
  app["MatDuplicator"]["out2"] >> app["sobelY"]["in"];

  // app["sobelX"]["out"] >> app["ImageDisplay"]["in"];
  // app["sobelY"]["out"] >> app["ImageDisplay2"]["in"];

  app["sobelX"]["out"] >> app["OrientationAndMagnitudeImage"]["dx"];
  app["sobelY"]["out"] >> app["OrientationAndMagnitudeImage"]["dy"];

  // Create a component outside to application to request its documentation
  std::cout << "Component documentation" << std::endl;
  std::cout << StreamFlow::Factory::create("ProducerComponentINT")->doc() << std::endl;

  // illustrate parallel idenpendant graph
  app["ProducerComponent"]["out"] >> app["ConsumerComponent"]["in"];

  // run the application, enjoy pipeline and parallelism;
  app.start();

  return 0;
}
