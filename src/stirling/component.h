#pragma once

#include "entity.h"

namespace stirling {
    class Component {
    public:
        virtual void update(Entity* entity, float delta_seconds) = 0;
    };
}