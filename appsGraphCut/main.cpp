#include <cassert>
#include <iostream>
#include <string>

#include "Application.h"
#include "ComponentFactory.h"
#include "Ports.h"

int main() {
  using namespace StreamFlow;

  // std::cout << StreamFlow::Factory::describe() << std::endl;

  Application app("AutoGraphCut");

  // add Component (or node) in the app with alias
  // app.addComponentAs("ImageDisplay", "ImageDisplay2");

  // connect app's components together
  app["VideoProducer"]["out"] >> app["AutoGraphCut"]["bgrImage"];

  // run the application, enjoy pipeline and parallelism;
  app.start();

  return 0;
}
