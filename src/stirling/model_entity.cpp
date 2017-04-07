#include "model_entity.h"

#include <iostream>

namespace stirling {

    ModelEntity::ModelEntity(vulkan::Model&& model) :
        m_model (std::move(model)) {
    }

    const vulkan::Model& ModelEntity::model() const {
        return m_model;
    }

    void ModelEntity::update(float delta_seconds) {
        m_transform.rotate(delta_seconds * 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
    }

}