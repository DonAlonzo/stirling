#pragma once

#include "entity.h"

#include <memory>
#include <vector>

namespace stirling {
    class World {
    public:
        void addEntity(std::shared_ptr<Entity> entity);

        void update(float delta_seconds);

    private:
        std::vector<std::shared_ptr<Entity>> m_entities;
    };
}