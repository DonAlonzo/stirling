#pragma once

#include "entity.h"

namespace stirling {

    struct Component {
        virtual void update(Entity* entity, float delta_seconds) = 0;
    };

}