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

class OrientationAndMagnitudeImage : public FactoryRegisteredComponent<OrientationAndMagnitudeImage> {
 public:
  OrientationAndMagnitudeImage()
      : FactoryRegisteredComponent<OrientationAndMagnitudeImage>("OrientationAndMagnitudeImage", "dx, dy image to orientation and magnitude") {}

  void init() override {}
  void step() override { transformInputToOutput(); }

 private:
  VideoCapture cap;

  void transformInputToOutput() {
    Mat dxIn = abs(*(dx.read()));
    Mat dyIn = abs(*(dy.read()));
    static constexpr double THRESHOLD_MAGNITUDE = 100.;

    // calculate magnitude of gradient and truncate
    Mat temp;
    magnitude(dxIn, dyIn, temp);
    threshold(temp, temp, THRESHOLD_MAGNITUDE, 1, THRESH_BINARY);
    cv::multiply(dxIn, temp, dxIn);
    cv::multiply(dyIn, temp, dyIn);

    // dyIn = dyIn * temp;

    Mat mask;
    temp.convertTo(mask, CV_8U, 1, 0);
    dxIn.convertTo(dxIn, CV_8U, 1. / 8, 0);
    dyIn.convertTo(dyIn, CV_8U, 1. / 8, 0);

    // cv::multiply(similarity, mask, similarity);

    Mat similarity = cv::Mat::zeros(dxIn.size(), CV_8U);

    for (int i : {4, 8, 16}) similarity += computeLocalSimilarity(dxIn, dyIn, mask, i);

    threshold(similarity, similarity, 1, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);
    findContours(similarity, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(similarity.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++) {
      Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
      drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
    }
    imshow("Contours", drawing);

    // similarity *= (255 / 4);

    imshow("dxIn", dxIn);
    imshow("dyIn", dyIn);
    imshow("simx", similarity);

    magnitudeOut.write(std::make_unique<Mat>(mask.clone()));

    // cout << "image " << counter++  << " " << image.cols << " " << image.rows
    // << endl;
  }

  float similarityScore(int x1, int y1, int x2, int y2) {
    // x1 -= 128;
    // x2 -= 128;
    // y1 -= 128;
    // y2 -= 128;

    // x1 = abs(x1);
    // x2 = abs(x2);
    // y1 = abs(y1);
    // y2 = abs(y2);

    auto n1 = sqrt(x1 * x1 + y1 * y1);
    auto n2 = sqrt(x2 * x2 + y2 * y2);
    auto cosineS = (x1 * x2 + y1 * y2) / (n1 * n2);

    return (cosineS);  // cosine similarity
  }

  Mat computeLocalSimilarity(Mat dxIn, Mat dyIn, Mat mask, int sizeDivider = 8) {
    Mat dxSmall;
    resize(dxIn, dxSmall, Size(dxIn.cols / sizeDivider, dxIn.rows / sizeDivider), 0, 0, INTER_LINEAR);

    Mat dySmall;
    resize(dyIn, dySmall, Size(dyIn.cols / sizeDivider, dyIn.rows / sizeDivider), 0, 0, INTER_LINEAR);

    Mat maskSmall;
    resize(mask, maskSmall, Size(dyIn.cols / sizeDivider, dyIn.rows / sizeDivider));

    Mat similarity = cv::Mat::zeros(dySmall.size(), CV_8U);

    float similarityCosine = 0.f;

    for (int row = 1; row < dxSmall.rows - 1; ++row) {
      uchar* maskp = maskSmall.ptr(row);
      uchar* sim = similarity.ptr(row);

      uchar* abovex = dxSmall.ptr(row - 1);
      uchar* centerx = dxSmall.ptr(row);
      uchar* underx = dxSmall.ptr(row + 1);

      uchar nox = *abovex++;
      uchar nx = *abovex++;
      uchar nex = *abovex++;

      uchar ox = *centerx++;
      uchar cx = *centerx++;
      uchar ex = *centerx++;

      uchar sox = *underx++;
      uchar sx = *underx++;
      uchar sex = *underx++;

      uchar* abovey = dySmall.ptr(row - 1);
      uchar* centery = dySmall.ptr(row);
      uchar* undery = dySmall.ptr(row + 1);

      uchar noy = *abovey++;
      uchar ny = *abovey++;
      uchar ney = *abovey++;

      uchar oy = *centery++;
      uchar cy = *centery++;
      uchar ey = *centery++;

      uchar soy = *undery++;
      uchar sy = *undery++;
      uchar sey = *undery++;

      for (int col = 1; col < dxSmall.cols - 1; ++col) {
        if ((*maskp++) > 0) {
          // *sim = (255 - abs((cx) - ((nox + nx + nex + ox + ex + sox + sx + sex) / 8))) / 8;
          // *sim += (255 - abs((cy) - ((noy + ny + ney + oy + ey + soy + sy + sey) / 8))) / 8;
          similarityCosine = 0;
          similarityCosine += similarityScore(cx, cy, ox, oy);
          similarityCosine += similarityScore(cx, cy, ex, ey);
          similarityCosine += similarityScore(cx, cy, nx, ny);
          similarityCosine += similarityScore(cx, cy, sx, sy);
          similarityCosine += similarityScore(cx, cy, sex, sey);
          similarityCosine += similarityScore(cx, cy, sox, soy);
          similarityCosine += similarityScore(cx, cy, nex, ney);
          similarityCosine += similarityScore(cx, cy, nox, noy);

          if (similarityCosine > 8 * 0.6) *sim = 1;
          //  if (*sim < 7 * 10) *sim = 0;

          // *sim += similarityScore(cx, cy, sex, sey);
          // *sim += similarityScore(cx, cy, sox, soy);
        }
        sim++;
        // shift window
        nox = nx;
        nx = nex;
        nex = *abovex++;

        ox = cx;
        cx = ex;
        ex = *centerx++;

        sox = sx;
        sx = sex;
        sex = *underx++;

        noy = ny;
        ny = ney;
        ney = *abovey++;

        oy = cy;
        cy = ey;
        ey = *centery++;

        soy = sy;
        sy = sey;
        sey = *undery++;
      }
    }

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(similarity, similarity, element);
    dilate(similarity, similarity, element);

    resize(similarity, similarity, Size(dyIn.cols, dyIn.rows), 0, 0, cv::INTER_NEAREST);

    return similarity;
  }

  StreamFlow::Input<std::unique_ptr<Mat>> dx = ComponentBase::createInput<std::unique_ptr<Mat>>(*this, "dx", "x gradient image");
  StreamFlow::Input<std::unique_ptr<Mat>> dy = ComponentBase::createInput<std::unique_ptr<Mat>>(*this, "dy", "y gradient image");

  StreamFlow::Output<std::unique_ptr<Mat>> orientationOut = ComponentBase::createOutput<std::unique_ptr<Mat>>(*this, "orientation", "orientation image");
  StreamFlow::Output<std::unique_ptr<Mat>> magnitudeOut = ComponentBase::createOutput<std::unique_ptr<Mat>>(*this, "magnitude", "magnitude image");
};

}  // namespace StreamFlow
