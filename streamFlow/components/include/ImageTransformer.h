#pragma once

#include <iostream>
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

template <Mat Transform(Mat&)>
class ImageTransformer : public FactoryRegisteredComponent<ImageTransformer<Transform>> {
 public:
  ImageTransformer() : FactoryRegisteredComponent<ImageTransformer<Transform>>("ImageTransformTemplate", "This is an image transform in -> out template") {
    // ComponentBase::exposeIO(out);
    // ComponentBase::exposeIO(in);
  }

  void init() override {}
  void step() override { transformInputToOutput(); }

 private:
  VideoCapture cap;

  void transformInputToOutput() {
    Mat imageIn = *(in.read());
    Mat imageOut;
    imageOut = Transform(imageIn);
    auto ptr = std::make_unique<Mat>(imageOut.clone());
    out.write(ptr);
    // cout << "image " << counter++  << " " << image.cols << " " << image.rows
    // << endl;
  }

  StreamFlow::Input<std::unique_ptr<Mat>> in = ComponentBase::createInput<std::unique_ptr<Mat>>(*this, "in", "input image");
  StreamFlow::Output<std::unique_ptr<Mat>> out = ComponentBase::createOutput<std::unique_ptr<Mat>>(*this, "out", "input image");
};

}  // namespace StreamFlow
