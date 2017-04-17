#include "entity.h"
#include "model_component.h"

#include <iostream>

namespace stirling {

    ModelComponent::ModelComponent(vulkan::Model&& model) :
        m_model (std::move(model)) {
    }

    ModelComponent::operator const vulkan::Model&() const {
        return m_model;
    }

    const vulkan::Model& ModelComponent::model() const {
        return m_model;
    }

    void ModelComponent::update(Entity* entity, float delta_seconds) {
    }

}