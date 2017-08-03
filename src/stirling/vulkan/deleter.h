#pragma once

#include "vulkan/vulkan.h"

#include <algorithm>
#include <functional>
#include <iostream>

namespace stirling {
    namespace vulkan {

        template <typename T>
        struct Deleter {
            Deleter() :
                m_object          (VK_NULL_HANDLE), 
                m_delete_function ([]() {}) {
            }

            Deleter(T object, std::function<void(T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object), 
                m_delete_function ([=]() {
                    delete_function(object, nullptr);
                }) {
            }

            Deleter(T object, VkInstance instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object),
                m_delete_function ([=]() {
                    delete_function(instance, object, nullptr);
                }) {
            }

            Deleter(T object, VkDevice device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> delete_function) :
                m_object          (object),
                m_delete_function ([=]() {
                    delete_function(device, object, nullptr);
                }) {
            }

            Deleter(const Deleter&) = delete;
            Deleter& operator=(const Deleter&) = delete;

            Deleter(Deleter&& rhs) :
                m_object          (std::move(rhs.m_object)),
                m_delete_function (std::move(rhs.m_delete_function)) {
                rhs.m_object = VK_NULL_HANDLE;
            }

            Deleter& operator=(Deleter&& rhs) {
                if (m_object != VK_NULL_HANDLE) m_delete_function();

                m_object          = std::move(rhs.m_object);
                m_delete_function = std::move(rhs.m_delete_function);
                
                rhs.m_object = VK_NULL_HANDLE;

                return *this;
            }

            ~Deleter() {
				if (m_object != VK_NULL_HANDLE) {
					std::cout << "Releasing " << typeid(T).name() << std::endl;
					m_delete_function();
				}
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