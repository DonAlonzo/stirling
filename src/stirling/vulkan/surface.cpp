#include "surface.h"
#include "instance.h"

namespace stirling {
    namespace vulkan {

        Surface::Surface(vulkan::Deleter<VkSurfaceKHR> surface) :
            surface (std::move(surface)) {
        }

        Surface::operator const VkSurfaceKHR&() const {
            return surface;
        }

    }
}