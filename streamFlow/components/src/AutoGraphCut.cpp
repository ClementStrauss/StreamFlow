#include "AutoGraphCut.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

void StreamFlow::AutoGraphCut::process() {
  Mat imageInputBGR = *(bgrImage.read());

  // bgr = imread("/home/cstrauss/Pictures/2022-08-30-100851.jpg");

  imshow("source", imageInputBGR);

  // Mat colorSpaced = bgr;
  // cvtColor(bgr, colorSpaced, COLOR_BGR2YCrCb);

  // // Extract the channels
  // std::vector<cv::Mat> YUV_planes(3);
  // cv::split(colorSpaced, YUV_planes);

  // // apply the CLAHE algorithm to the L channel
  // cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
  // clahe->setClipLimit(2);
  // cv::Mat dst0, dst1, dst2;
  // clahe->apply(YUV_planes[0], dst0);
  // clahe->apply(YUV_planes[1], dst1);
  // clahe->apply(YUV_planes[2], dst2);

  // // Merge the the color planes back into an Yxx image
  // dst0.copyTo(YUV_planes[0]);
  // dst1.copyTo(YUV_planes[1]);
  // dst2.copyTo(YUV_planes[2]);

  // cv::merge(YUV_planes, colorSpaced);

  // // convert back to RGB
  // cv::Mat image_clahe;
  // // bgr = colorSpaced;
  // cv::cvtColor(colorSpaced, bgr, COLOR_YCrCb2BGR);

  // imshow("bgrFiltered", bgrFiltered);

  Mat bgrSmall;
  Mat bgrSmaller;
  int pyramideScale1 = 8;
  int pyramideScale2 = 2;

  resize(imageInputBGR, bgrSmall, {imageInputBGR.cols / pyramideScale1, imageInputBGR.rows / pyramideScale1}, 0, 0, INTER_LINEAR);
  // blur(bgrSmall, bgrSmall, {3, 3});
  resize(bgrSmall, bgrSmaller, {bgrSmall.cols / pyramideScale2, bgrSmall.rows / pyramideScale2}, 0, 0, INTER_LINEAR);
  // medianBlur(bgrSmaller, bgrSmaller, 3);
  // blur(bgrSmaller, bgrSmaller, {3, 3});

  Mat bgrFiltered;

  Mat bgdModel, fgdModel;
  Mat res;

  // take first 33% as sky region init
  // Drawing the Rectangle
  cv::Mat mask = cv::Mat::zeros(cv::Size(bgrSmaller.cols, bgrSmaller.rows), CV_8UC1);
  mask = GC_PR_BGD;
  Rect rect(0, 0, bgrSmaller.cols, bgrSmaller.rows / 2);
  rectangle(mask, rect, GC_PR_FGD, -1);

  grabCut(bgrSmaller, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);

  imshow("bgrSmall", bgrSmall);
  imshow("mask", mask * 64);
  int erosion_size = 5;
  Mat element = getStructuringElement(MORPH_RECT, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
  erode(mask, mask, element);
  imshow("mask erode", mask * 64);

  resize(mask, mask, {bgrSmall.cols, bgrSmall.rows}, 0, 0, INTER_NEAREST);

  grabCut(bgrSmall, mask, rect, bgdModel, fgdModel, 2, GC_EVAL);
  mask = mask & 1;
  resize(mask, mask, {imageInputBGR.cols, imageInputBGR.rows}, 0, 0, INTER_LANCZOS4);

  morphologyEx(mask, mask, cv::MORPH_OPEN, element);

  // imshow("maskResized step1", mask * 64);

  // imshow("maskResized eroded", mask * 64);

  // // border = bgr.cols / 12;
  // // Rect rect2(border, border, bgr.cols - border * 2, bgr.rows - border * 2);
  // // grabCut(bgr, mask, rect2, bgdModel, fgdModel, 1, GC_EVAL);

  // // feathered mask
  // blur(black, black, {11, 11});
  // multiply(bgr, black, bgr, 1. / 255);
  // imshow("res2", bgr);

  blur(mask, mask, {5, 5});

  Mat darkenImage(mask.rows, mask.cols, CV_8UC3, cv::Scalar(255, 255, 255));
  darkenImage.setTo(cv::Scalar(0, 0, 0), mask);

  Mat coloredMask(mask.rows, mask.cols, CV_8UC3, cv::Scalar(0, 0, 0));
  coloredMask.setTo(cv::Scalar(0, 255, 0), mask);

  // blur(coloredMask, coloredMask, {15, 15});
  //  medianBlur(coloredMask, coloredMask, 5);

  // blur(black, black, {11, 11});

  // multiply(imageInputBGR, coloredMask, imageInputBGR, 1. / 255);
  //   imshow("bgr", bgr);

  addWeighted(coloredMask, 0.8, imageInputBGR, 0.7, 0.0, res);

  // imshow("mask", mask * 64);
  imshow("res", res);

  // // feathered mask
  // blur(black, black, {11, 11});
  // multiply(bgr, black, bgr, 1. / 255);
  // imshow("res2", bgr);
}

REGISTER_IN_FACTORY_WITH_NAME(StreamFlow::AutoGraphCut, AutoGraphCut);
