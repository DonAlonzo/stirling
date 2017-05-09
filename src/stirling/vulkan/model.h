#pragma once

#include <glm/glm.hpp>

namespace stirling { namespace vulkan { class Device; } }
#include "index_buffer.h"
#include "texture.h"
#include "vertex.h"
#include "vertex_buffer.h"

#include <string>
#include <vector>

namespace stirling {
    namespace vulkan {
        class Model {
        public:
            VertexBuffer vertex_buffer;
            IndexBuffer  index_buffer;
            Texture      texture;

            Model(VertexBuffer&& vertex_buffer, IndexBuffer&& index_buffer, Texture&& texture);

            static Model loadFromFile(const Device& device, const std::string& model_path, const std::string& texture_path);
        };
    }
}