#pragma once
#include <type_traits>
#include "componentManager.h"
#include "entity.h"

namespace Heimdall {

struct ComponentCounter {
    static int familyCounter;
};

template <typename ComponentType>
struct Component {
    static inline int family()
    {
        static int family = ComponentCounter::familyCounter++;
        return family;
    }
};

template <typename C>
static int getComponentFamily()
{
    return Component<typename std::remove_const<C>::type>::family();
}

}