#include <opencv2/core/hal/interface.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ImageTransformer.h"

namespace StreamFlow {

Mat imageTransformer(Mat& imageIn) { return imageIn; };

REGISTER_IN_FACTORY_WITH_NAME(ImageTransformer<imageTransformer>, identity);

Mat grayscale(Mat& imageIn) {
  cv::Mat greyMat;
  cv::cvtColor(imageIn, greyMat, cv::COLOR_BGR2GRAY);
  return greyMat;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransformer<grayscale>, grayscale);

Mat sobelX(Mat& imageIn) {
  cv::Mat imageOut;
  cv::Scharr(imageIn, imageOut, CV_32F, 1, 0);
  return imageOut;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransformer<sobelX>, sobelX);

Mat sobelY(Mat& imageIn) {
  cv::Mat imageOut;
  cv::Scharr(imageIn, imageOut, CV_32F, 0, 1);
  return imageOut;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransformer<sobelY>, sobelY);

Mat blur(Mat& imageIn) {
  cv::Mat imageOut = imageIn;
  // cv::GaussianBlur(imageIn, imageOut, Size(3, 3), 0);
  // cv::medianBlur(imageIn, imageOut, 3);
  imshow("blurred", imageOut);
  return imageOut;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransformer<blur>, blur);

}  // namespace StreamFlow
