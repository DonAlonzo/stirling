#pragma once

#include "entity.h"
#include "vulkan/model.h"

namespace stirling {
    class ModelEntity : public Entity {
    public:
        ModelEntity(vulkan::Model&&);

        const vulkan::Model& model() const;

        void update(float delta_seconds) override;

    private:
        vulkan::Model m_model;
    };
}