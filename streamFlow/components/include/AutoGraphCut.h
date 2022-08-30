#pragma once

#include <opencv2/core/hal/interface.h>

#include <iostream>
#include <new>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "registeredComponent.h"

using namespace std;
using namespace cv;

namespace StreamFlow {

class AutoGraphCut : public FactoryRegisteredComponent<AutoGraphCut> {
 public:
  AutoGraphCut() : FactoryRegisteredComponent<AutoGraphCut>("AutoGraphCut", "auto graphcut") {}

  void init() override {}
  void step() override { process(); }

 private:
  VideoCapture cap;

  void process();

  StreamFlow::Input<std::unique_ptr<Mat>> bgrImage = ComponentBase::createInput<std::unique_ptr<Mat>>(*this, "bgrImage", "rgb image from source");

  // StreamFlow::Output<std::unique_ptr<Mat>> imageOut = ComponentBase::createOutput<std::unique_ptr<Mat>>(*this, "imageOut", "imageOut");
};

}  // namespace StreamFlow
