#include "Duplicator.h"

#include <opencv2/imgproc.hpp>

namespace StreamFlow {

char intDuplicatorDesc[] = {"duplicator"};  // TODO : see C++20 string literral template, not working for now...
typedef Duplicator<int, intDuplicatorDesc> intDuplicator;

REGISTER_IN_FACTORY_WITH_NAME(intDuplicator, intDuplicator);

typedef Clonator<Mat, intDuplicatorDesc> MatDuplicator;

REGISTER_IN_FACTORY_WITH_NAME(MatDuplicator, MatDuplicator);

}  // namespace StreamFlow
