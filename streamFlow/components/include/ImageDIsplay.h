#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include "ComponentBase.h"
#include "ComponentFactory.h"
#include "Ports.h"
#include "opencv2/objdetect.hpp"
#include "registeredComponent.h"

using namespace std;
using namespace cv;

namespace StreamFlow {

class ImageDisplay : public FactoryRegisteredComponent<ImageDisplay> {
 public:
  ImageDisplay() : FactoryRegisteredComponent("ImageDisplay", "This is an image display for OpenCV") { exposeIO(*this, in); }

  void init() override { namedWindow(name()); }
  void step() override { display(); }

 private:
  VideoCapture cap;

  void display() {
    //    cout << in.getElementNumber() << endl;
    Mat image = *(in.read());

    imshow(name(), image);
    std::this_thread::sleep_for(std::chrono::microseconds(20));
  }

  StreamFlow::Input<std::unique_ptr<Mat>> in{"in", "produce incrementing int every X microseconds"};
};

}  // namespace StreamFlow

#endif  // IMAGEDISPLAY_H
