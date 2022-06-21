#include "consumer.h"

namespace StreamFlow {

// template<> FactoryRegistrar<ConsumerComponent>
// ConsumerComponent::RegisteredComponent<ConsumerComponent>::registrar =
//    FactoryRegistrar<ConsumerComponent>("ConsumerComponent");

REGISTER_IN_FACTORY(ConsumerComponent);
REGISTER_IN_FACTORY(ConsumerComponentINT)

REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<float>, consumerFloat);
REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<double>, consumerDouble);
REGISTER_IN_FACTORY_WITH_NAME(ConsumerComponentTemplate<short>, consumerShort);

}  // namespace StreamFlow
