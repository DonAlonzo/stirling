#pragma once

#include "vulkan/vulkan.h"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <array>

namespace stirling {
    namespace vulkan {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec2 texture_coordinate;

            bool operator==(const Vertex& rhs) const;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
        };
    }
}

namespace std {
    template<> struct hash<stirling::vulkan::Vertex> {
        size_t operator()(stirling::vulkan::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^
                    (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                    (hash<glm::vec2>()(vertex.texture_coordinate) << 1);
        }
    };
}