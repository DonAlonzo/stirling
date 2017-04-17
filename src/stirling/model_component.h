#pragma once

#include "component.h"
#include "vulkan/model.h"

namespace stirling {
    class ModelComponent : public Component {
    public:
        ModelComponent(vulkan::Model&&);

        operator const vulkan::Model&() const;

        const vulkan::Model& model() const;

        void update(Entity* entity, float delta_seconds) override;

    private:
        vulkan::Model m_model;
    };
}