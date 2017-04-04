#include "model.h"
#include "device.h"
#include "image.h"
#include "texture.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

#include <algorithm>
#include <unordered_map>

namespace stirling {
    namespace vulkan {

        Model Model::loadFromFile(const Device& device, const std::string& model_path, const std::string& texture_path) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string err;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_path.c_str())) {
                throw std::runtime_error(err);
            }


            std::unordered_map<Vertex, int> unique_vertices = {};
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    Vertex vertex = {};

                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.texture_coordinate = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };

                    vertex.color = { 1.0f, 1.0f, 1.0f };

                    if (unique_vertices.count(vertex) == 0) {
                        unique_vertices[vertex] = vertices.size();
                        vertices.push_back(vertex);
                    }

                    indices.push_back(unique_vertices[vertex]);
                }
            }

            return Model(VertexBuffer(device, vertices), IndexBuffer(device, indices), Texture(device, Image::loadFromFile(device, texture_path)));
        }

        Model::Model(VertexBuffer&& vertex_buffer, IndexBuffer&& index_buffer, Texture&& texture) :
            m_vertex_buffer (std::move(vertex_buffer)),
            m_index_buffer  (std::move(index_buffer)),
            m_texture       (std::move(texture)) {
        }

        const VertexBuffer& Model::getVertexBuffer() const {
            return m_vertex_buffer;
        }

        const IndexBuffer& Model::getIndexBuffer() const {
            return m_index_buffer;
        }

        const Texture& Model::getTexture() const {
            return m_texture;
        }

    }
}