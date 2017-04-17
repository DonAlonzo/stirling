#pragma once

namespace stirling { class Entity; }

namespace stirling {
    class Component {
    public:
        virtual void update(Entity* entity, float delta_seconds) = 0;
    };
}