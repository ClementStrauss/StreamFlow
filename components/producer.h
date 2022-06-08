#pragma once

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"

namespace StreamFlow {

class ProducerComponent : public ComponentBase {
public:
  ProducerComponent() : ComponentBase("producer", "This is a producer") {
    exposeIO(out);
  }

  void init() override {}
  void step() override { produce(); }

  void produce() {
    if (counter < 1000) {
      auto ptr = std::make_unique<int>(counter);
      out.write(ptr);
      counter++;
      //  std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }
  StreamFlow::Output<std::unique_ptr<int>> out{
      "out", "produce incrementing int every X microseconds"};
  int counter = 0;
  std::string composentName;
};

REGISTER_IN_FACTORY(ProducerComponent);

class ProducerComponentINT : public ComponentBase {
public:
  ProducerComponentINT() : ComponentBase("producerINT", "This is a producer") {
    exposeIO(out);
  }

  void init() override {}
  void step() override { produce(); }

  void produce() {
    if (counter < 1000) {
      out.write(counter);
      counter++;
      //  std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }
  StreamFlow::Output<int> out{"out",
                              "produce incrementing int every X microseconds"};
  int counter = 0;
  std::string composentName;
};

REGISTER_IN_FACTORY(ProducerComponentINT);
} // namespace StreamFlow
