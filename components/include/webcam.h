#pragma once

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"

#include "registeredComponent.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

namespace StreamFlow {

class WebcamProducer : public RegisteredComponent<WebcamProducer> {
public:
  WebcamProducer()
      : RegisteredComponent("WebcamProducer", "This is an image producer") {
    exposeIO(out);
  }

  void init() override {
    cap.open(0, CAP_V4L2);
      cout << "open camera" << endl;
    if (!cap.isOpened()) {
      cout << "cannot open camera"<< endl;
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
    cout << "image " << counter++  << " " << image.cols << " " << image.rows << endl;
  }

  StreamFlow::Output<std::unique_ptr<Mat>> out{
      "out", "produce incrementing int every X microseconds"};

  int counter = 0;
};

} // namespace StreamFlow
