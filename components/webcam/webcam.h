#pragma once

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"

#include "opencv2/objdetect.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

namespace StreamFlow {

class WebcamProducer : public ComponentBase {
public:
  WebcamProducer()
      : ComponentBase("WebcamProducer", "This is an image producer") {
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

  static FactoryRegistrer<WebcamProducer> registrar;
};

} // namespace StreamFlow
