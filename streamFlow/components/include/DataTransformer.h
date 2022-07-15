#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "registeredComponent.h"

using namespace std;
using namespace cv;

namespace StreamFlow {

template <typename I, typename O, O Transform(I &), const char *const str>
class DataTransformer : public FactoryRegisteredComponent<DataTransformer<I, O, Transform, str>> {
 public:
  DataTransformer() : FactoryRegisteredComponent<DataTransformer<I, O, Transform, str>>("DataTransformer", str) {}
  auto operator<=>(const DataTransformer &) const = default;

  void init() override {}
  void step() override { transformInputToOutput(); }

 private:
  void transformInputToOutput() {
    I inputData = *(in.read());
    O outputData = Transform(inputData);
    auto ptr = std::make_unique<O>(outputData);
    out.write(ptr);
  }

  StreamFlow::Input<std::unique_ptr<I>> in = ComponentBase::createInput<std::unique_ptr<I>>(*this, "in", "input image");
  StreamFlow::Output<std::unique_ptr<O>> out = ComponentBase::createOutput<std::unique_ptr<O>>(*this, "out", "output image");
};

}  // namespace StreamFlow
