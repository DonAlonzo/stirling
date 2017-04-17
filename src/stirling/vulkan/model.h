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
            Model(VertexBuffer&& vertex_buffer, IndexBuffer&& index_buffer, Texture&& texture);

            const VertexBuffer& getVertexBuffer() const;
            const IndexBuffer& getIndexBuffer() const;
            const Texture& getTexture() const;

            static Model loadFromFile(const Device& device, const std::string& model_path, const std::string& texture_path);

        private:
            VertexBuffer m_vertex_buffer;
            IndexBuffer  m_index_buffer;
            Texture      m_texture;
        };
    }
}