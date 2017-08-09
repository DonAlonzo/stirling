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
                object          (VK_NULL_HANDLE), 
                delete_function ([]() {}) {
            }

            Deleter(T object, std::function<void(T, VkAllocationCallbacks*)> delete_function) :
                object          (object), 
                delete_function ([=]() {
                    delete_function(object, nullptr);
                }) {
            }

            Deleter(T object, VkInstance instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> delete_function) :
                object          (object),
                delete_function ([=]() {
                    delete_function(instance, object, nullptr);
                }) {
            }

            Deleter(T object, VkDevice device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> delete_function) :
                object          (object),
                delete_function ([=]() {
                    delete_function(device, object, nullptr);
                }) {
            }

            Deleter(const Deleter&) = delete;
            Deleter& operator=(const Deleter&) = delete;

            Deleter(Deleter&& rhs) :
                object          (std::move(rhs.object)),
                delete_function (std::move(rhs.delete_function)) {
                rhs.object = VK_NULL_HANDLE;
            }

            Deleter& operator=(Deleter&& rhs) {
                if (object != VK_NULL_HANDLE) delete_function();

                object          = std::move(rhs.object);
                delete_function = std::move(rhs.delete_function);
                
                rhs.object = VK_NULL_HANDLE;

                return *this;
            }

            ~Deleter() {
				if (object != VK_NULL_HANDLE) {
					std::cout << "Releasing " << typeid(T).name() << std::endl;
					delete_function();
				}
            }

            operator T() const {
                return object;
            }

        private:
            T object;
            std::function<void()> delete_function;
        };

    }
}