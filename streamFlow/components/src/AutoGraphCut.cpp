#include "AutoGraphCut.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

void StreamFlow::AutoGraphCut::process() {
  Mat bgr = *(bgrImage.read());

  // bgr = imread("/home/cstrauss/Pictures/2022-08-30-100851.jpg");

  imshow("inputBefore", bgr);

  Mat colorSpaced = bgr;
  cvtColor(bgr, colorSpaced, COLOR_BGR2YCrCb);

  // Extract the channels
  std::vector<cv::Mat> YUV_planes(3);
  cv::split(colorSpaced, YUV_planes);

  // apply the CLAHE algorithm to the L channel
  cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
  clahe->setClipLimit(2);
  cv::Mat dst0, dst1, dst2;
  clahe->apply(YUV_planes[0], dst0);
  clahe->apply(YUV_planes[1], dst1);
  clahe->apply(YUV_planes[2], dst2);

  // Merge the the color planes back into an Lab image
  dst0.copyTo(YUV_planes[0]);
  dst1.copyTo(YUV_planes[1]);
  dst2.copyTo(YUV_planes[2]);

  cv::merge(YUV_planes, colorSpaced);

  // convert back to RGB
  cv::Mat image_clahe;
  // bgr = colorSpaced;
  cv::cvtColor(colorSpaced, bgr, COLOR_YCrCb2BGR);

  Mat bgrFiltered;
  medianBlur(bgr, bgrFiltered, 11);

  imshow("bgrFiltered", bgrFiltered);

  Mat bgrSmall;
  resize(bgrFiltered, bgrSmall, {bgr.cols / 4, bgr.rows / 4}, 0, 0, INTER_NEAREST);

  Mat bgdModel, fgdModel;
  Mat mask;
  Mat res;

  int border = bgrSmall.cols / 16;
  Rect rect(border, border * 2, bgrSmall.cols - border * 2, bgrSmall.rows - border * 2);
  grabCut(bgrSmall, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

  resize(mask, mask, {bgr.cols, bgr.rows}, 0, 0, INTER_NEAREST);
  imshow("maskResized step1", mask * 64);

  int erosion_size = 15;
  Mat element = getStructuringElement(MORPH_RECT, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
  erode(mask, mask, element);
  imshow("maskResized eroded", mask * 64);

  border = bgr.cols / 12;
  Rect rect2(border, border, bgr.cols - border * 2, bgr.rows - border * 2);
  grabCut(bgr, mask, rect2, bgdModel, fgdModel, 1, GC_EVAL);

  Mat black(mask.rows, mask.cols, CV_8UC3, cv::Scalar(0, 0, 0));
  black.setTo(Scalar::all(255), mask & 1);
  addWeighted(black, 0.5, bgr, 0.5, 0.0, res);

  imshow("mask", mask * 64);
  imshow("res", res);

  // feathered mask
  blur(black, black, {11, 11});
  multiply(bgr, black, bgr, 1. / 255);
  imshow("res2", bgr);
}

REGISTER_IN_FACTORY_WITH_NAME(StreamFlow::AutoGraphCut, AutoGraphCut);
