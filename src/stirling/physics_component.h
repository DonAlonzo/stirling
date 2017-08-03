#pragma once

#include "component.h"

namespace stirling {

    struct PhysicsComponent : Component {
        PhysicsComponent();

        void update(Entity* entity, float delta_seconds) override;
    };

}