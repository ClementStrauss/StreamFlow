#include "DataTransformer.h"

#include <opencv2/imgproc.hpp>

namespace StreamFlow {

int identityFunc(int& in) { return in; };
typedef DataTransformer<int, int, identityFunc> identityDataTransformer;

REGISTER_IN_FACTORY_WITH_NAME(identityDataTransformer, identityDataTransformer);

}  // namespace StreamFlow
