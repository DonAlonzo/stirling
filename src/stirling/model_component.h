#pragma once

#include "component.h"
#include "vulkan/model.h"

namespace stirling {
    class ModelComponent : public Component {
    public:
        ModelComponent(vulkan::Model&&, VkDescriptorSet descriptor_set);

        operator const vulkan::Model&() const;

        void update(Entity* entity, float delta_seconds) override;

        const vulkan::Model   model;
        const VkDescriptorSet descriptor_set;
    };
}