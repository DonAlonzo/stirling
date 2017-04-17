#pragma once

#include "component.h"

namespace stirling {
    class PhysicsComponent : public Component {
    public:
        PhysicsComponent();

        void update(Entity* entity, float delta_seconds) override;
    };
}