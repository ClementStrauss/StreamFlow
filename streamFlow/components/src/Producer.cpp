#include "producer.h"

namespace StreamFlow {

// template<> FactoryRegistrar<ConsumerComponent>
// ConsumerComponent::RegisteredComponent<ConsumerComponent>::registrar =
//    FactoryRegistrar<ConsumerComponent>("ConsumerComponent");

REGISTER_IN_FACTORY(ProducerComponent);
REGISTER_IN_FACTORY(ProducerComponentINT)

}  // namespace StreamFlow
