#include "instance.h"

#include <stdexcept>

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> g_validation_layers = {
    "VK_LAYER_LUNARG_standard_validation"
};

namespace stirling {
    namespace vulkan {

        Instance::Instance(const std::vector<const char*>& extensions) :
            Deleter<VkInstance>(init(extensions), vkDestroyInstance),
            validator        (initValidator()),
            layer_properties (getLayerProperties()),
            physical_devices (getPhysicalDevices()) {
        }

        VkInstance Instance::init(std::vector<const char*> extensions) const {
            if (ENABLE_VALIDATION_LAYERS) {
                if (!checkValidationLayerSupport()) {
                    throw std::runtime_error("Validation layers requested, but not available.");
                }
                extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            }

            VkApplicationInfo application_info = {};
            application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            application_info.pApplicationName   = "Stirling Engine Demo";
            application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
            application_info.pEngineName        = "Stirling Engine";
            application_info.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
            application_info.apiVersion         = VK_API_VERSION_1_0;

            VkInstanceCreateInfo create_info = {};
            create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            create_info.pApplicationInfo        = &application_info;
            create_info.enabledExtensionCount   = extensions.size();
            create_info.ppEnabledExtensionNames = extensions.data();
            create_info.enabledLayerCount       = ENABLE_VALIDATION_LAYERS ? g_validation_layers.size() : 0;
            create_info.ppEnabledLayerNames     = ENABLE_VALIDATION_LAYERS ? g_validation_layers.data() : nullptr;

            VkInstance instance;
            if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create instance.");
            }
            return instance;
        }

        Validator Instance::initValidator() const {
            return ENABLE_VALIDATION_LAYERS ? Validator{*this} : Validator{};
        }

        std::vector<VkLayerProperties> Instance::getLayerProperties() const {
            uint32_t instance_layer_count;
            vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
            std::vector<VkLayerProperties> instance_layers{instance_layer_count};
            vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers.data());
            return instance_layers;
        }

        std::vector<VkPhysicalDevice> Instance::getPhysicalDevices() const {
            uint32_t physical_device_count = 0;
            vkEnumeratePhysicalDevices(*this, &physical_device_count, nullptr);
            std::vector<VkPhysicalDevice> physical_devices{physical_device_count};
            vkEnumeratePhysicalDevices(*this, &physical_device_count, physical_devices.data());
            return physical_devices;
        }

        bool Instance::checkValidationLayerSupport() const {
            auto available_layers = getLayerProperties();

            for (const auto& validation_layer : g_validation_layers) {
                bool found = false;

                for (const auto& available_layer : available_layers) {
                    if (strcmp(validation_layer, available_layer.layerName) == 0) {
                        found = true;
                        break;
                    }
                }

                if (!found) return false;
            }

            return true;
        }

    }
}