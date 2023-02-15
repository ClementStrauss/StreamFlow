#include "Video.h"

namespace StreamFlow {

// template<> FactoryRegistrar<WebcamProducer>
// WebcamProducer::RegisteredComponent<WebcamProducer>::registrar =
//     FactoryRegistrar<WebcamProducer>("WebcamProducer");

REGISTER_IN_FACTORY(VideoProducer);

}  // namespace StreamFlow
