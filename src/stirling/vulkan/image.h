#pragma once

#include "vulkan/vulkan.h"

namespace stirling { namespace vulkan {
    struct Device;
}}
#include "deleter.h"

#include <string>

namespace stirling {
    namespace vulkan {

        struct Image : Deleter<VkImage>{
			Deleter<VkDeviceMemory> memory;

            Image(const Device& device, const VkImageCreateInfo& create_info, const VkMemoryPropertyFlags& properties);

			static Image loadFromFile(const Device& device, const std::string& file_name);

        private:
            VkImage                 init(const Device& device, const VkImageCreateInfo& create_info) const;
            Deleter<VkDeviceMemory> allocateMemory(const Device& device, const VkMemoryPropertyFlags& properties) const;
            uint32_t                findMemoryType(const Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        };

    }
}