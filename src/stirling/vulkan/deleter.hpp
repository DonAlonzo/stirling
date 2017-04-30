#pragma once

#include "vulkan/vulkan.h"

#include <functional>

namespace stirling {
    namespace vulkan {
        template <typename T>
        class Deleter {
        public:
            Deleter(T object, std::function<void(T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object), 
                m_delete_function ([=]() {
                    delete_function(object, nullptr);
                }) {
            }

            Deleter(T object, VkInstance instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object),
                m_delete_function ([=]() {
                    delete_function(instance, obj, nullptr);
                }) {
            }

            Deleter(T object, VkDevice device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object),
                m_delete_function ([=]() {
                    delete_function(device, object, nullptr);
                }) {
            }

            ~Deleter() {
                m_delete_function();
            }

            operator const T&() const {
                return m_object;
            }

        private:
            T m_object;
            std::function<void()> m_delete_function;
        };
    }
}