#include "entity.h"
#include "model_component.h"

#include <iostream>

namespace stirling {

    ModelComponent::ModelComponent(vulkan::Model&& model, VkDescriptorSet descriptor_set) :
        model          (std::move(model)),
        descriptor_set (descriptor_set) {
    }

    ModelComponent::operator const vulkan::Model&() const {
        return model;
    }

    void ModelComponent::update(Entity* entity, float delta_seconds) {
    }

}