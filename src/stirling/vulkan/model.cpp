#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

#include <algorithm>
#include <unordered_map>

namespace stirling {
    namespace vulkan {

        Model Model::loadFromFile(const std::string& path) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string err;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
                throw std::runtime_error(err);
            }

            std::unordered_map<vulkan::Vertex, uint32_t> unique_vertices = {};

            Model model = {};

            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    vulkan::Vertex vertex = {};

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
                        unique_vertices[vertex] = model.vertices.size();
                        model.vertices.push_back(vertex);
                    }

                    model.indices.push_back(unique_vertices[vertex]);
                }
            }

            return model;
        }

    }
}