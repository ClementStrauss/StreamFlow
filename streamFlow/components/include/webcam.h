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

class WebcamProducer : public FactoryRegisteredComponent<WebcamProducer> {
 public:
  WebcamProducer() : FactoryRegisteredComponent("WebcamProducer", "This is an image producer") { exposeIO(*this, out); }

  void init() override {
    cap.open(0, CAP_V4L2);
    cout << "open camera" << endl;
    if (!cap.isOpened()) {
      cerr << "cannot open camera" << endl;
      throw;
    }
  }
  void step() override { produce(); }

 private:
  VideoCapture cap;

  void produce() {
    Mat image;
    cap.read(image);
    auto ptr = std::make_unique<Mat>(image.clone());
    out.write(ptr);

    waitKey(1);

    // cout << "image " << counter++  << " " << image.cols << " " << image.rows
    // << endl;
  }

  StreamFlow::Output<std::unique_ptr<Mat>> out{"out", "produce image from webcam"};

  int counter = 0;
};

}  // namespace StreamFlow
