#include "ImageTransform.h"

#include <opencv2/imgproc.hpp>

namespace StreamFlow {

Mat imageTransformer(Mat& imageIn) { return imageIn; };

REGISTER_IN_FACTORY_WITH_NAME(ImageTransform<imageTransformer>, identity);

Mat grayscale(Mat& imageIn) {
  cv::Mat greyMat;
  cv::cvtColor(imageIn, greyMat, cv::COLOR_BGR2GRAY);
  return greyMat;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransform<grayscale>, grayscale);

Mat sobelX(Mat& imageIn) {
  cv::Mat imageOut;
  cv::Sobel(imageIn, imageOut, CV_8U, 1, 0);
  return imageOut;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransform<sobelX>, sobelX);

Mat sobelY(Mat& imageIn) {
  cv::Mat imageOut;
  cv::Sobel(imageIn, imageOut, CV_8U, 0, 1);
  return imageOut;
};

REGISTER_IN_FACTORY_WITH_NAME(ImageTransform<sobelY>, sobelY);

}  // namespace StreamFlow
