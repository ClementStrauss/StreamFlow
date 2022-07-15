#pragma once

#include <algorithm>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <type_traits>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "registeredComponent.h"

using namespace std;
using namespace cv;

namespace StreamFlow {

template <typename T, const char *const str>
class Duplicator : public FactoryRegisteredComponent<Duplicator<T, str>> {
 public:
  Duplicator() : FactoryRegisteredComponent<Duplicator<T, str>>("Duplicator", str) { static_assert(is_trivially_copyable<T>()); }
  void init() override {}
  void step() override { transformInputToOutput(); }

 private:
  void transformInputToOutput() {
    T inputData = *(in.read());
    out1.write(std::make_unique<T>(inputData));
    out2.write(std::make_unique<T>(inputData));

    // cout << "image " << counter++  << " " << image.cols << " " << image.rows
    // << endl;
  }

  StreamFlow::Input<std::unique_ptr<T>> in = ComponentBase::createInput<std::unique_ptr<T>>(*this, "in", "input");
  StreamFlow::Output<std::unique_ptr<T>> out1 = ComponentBase::createOutput<std::unique_ptr<T>>(*this, "out1", "output duplicate 1");
  StreamFlow::Output<std::unique_ptr<T>> out2 = ComponentBase::createOutput<std::unique_ptr<T>>(*this, "out2", "output duplicate 2");
};

template <typename T, const char *const str>
class Clonator : public FactoryRegisteredComponent<Clonator<T, str>> {
 public:
  Clonator() : FactoryRegisteredComponent<Clonator<T, str>>("Duplicator", str) {}
  void init() override {}
  void step() override { transformInputToOutput(); }

 private:
  void transformInputToOutput() {
    T inputData = *(in.read());
    out1.write(std::make_unique<T>(inputData.clone()));
    out2.write(std::make_unique<T>(inputData.clone()));

    // cout << "image " << counter++  << " " << image.cols << " " << image.rows
    // << endl;
  }

  StreamFlow::Input<std::unique_ptr<T>> in = ComponentBase::createInput<std::unique_ptr<T>>(*this, "in", "input");
  StreamFlow::Output<std::unique_ptr<T>> out1 = ComponentBase::createOutput<std::unique_ptr<T>>(*this, "out1", "input duplicate 1");
  StreamFlow::Output<std::unique_ptr<T>> out2 = ComponentBase::createOutput<std::unique_ptr<T>>(*this, "out2", "input duplicate 2");
};

}  // namespace StreamFlow