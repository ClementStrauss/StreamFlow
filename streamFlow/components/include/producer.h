#pragma once

#include "ComponentFactory.h"
#include "Ports.h"
#include "registeredComponent.h"

namespace StreamFlow {

class ProducerComponent : public FactoryRegisteredComponent<ProducerComponent> {
 public:
  ProducerComponent() : FactoryRegisteredComponent("producer", "This is a producer") { exposeIO(*this, out); }

  void init() override {}
  void step() override { produce(); }

 private:
  void produce() {
    if (counter < 1000) {
      auto ptr = std::make_unique<int>(counter);
      out.write(ptr);
      counter++;
      //  std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }
  StreamFlow::Output<std::unique_ptr<int>> out{"out", "produce incrementing int every X microseconds"};
  int counter = 0;
};

class ProducerComponentINT : public FactoryRegisteredComponent<ProducerComponentINT> {
 public:
  ProducerComponentINT() : FactoryRegisteredComponent("producerINT", "This is a producer") { exposeIO(*this, out); }

  void init() override {}
  void step() override { produce(); }

 private:
  void produce() {
    if (counter < 1000) {
      out.write(counter);
      counter++;
      //  std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }
  StreamFlow::Output<int> out{"out", "produce incrementing int every X microseconds"};
  int counter = 0;

  StreamFlow::Input<int> out2 = createInput<int>(*this, "out2", "desc out2");
};

}  // namespace StreamFlow
