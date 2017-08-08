#pragma once

#include "component.h"

namespace stirling {

    struct PhysicsComponent : Component {
        void update(Entity* entity, float delta_seconds) override;
    };

}