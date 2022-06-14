#include "webcam.h"

namespace StreamFlow {

// template<> FactoryRegistrar<WebcamProducer>
// WebcamProducer::RegisteredComponent<WebcamProducer>::registrar =
//     FactoryRegistrar<WebcamProducer>("WebcamProducer");

REGISTER_IN_FACTORY(WebcamProducer);

auto WebcamProducer2 = WebcamProducer::registerInFactory({"WebcamProducer2"});

int WebcamProducer::ComponentBase::reg =
    WebcamProducer::registerInFactory("WebcamProducer3");

} // namespace StreamFlow
