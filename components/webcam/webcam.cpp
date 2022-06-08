#include "webcam.h"

namespace StreamFlow {

FactoryRegistrer<WebcamProducer> WebcamProducer::registrar =
    FactoryRegistrer<WebcamProducer>("WebcamProducer");
// REGISTER_IN_FACTORY(WebcamProducer)

} // namespace StreamFlow
