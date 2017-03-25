#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
		class Instance;
	}
}

#include <functional>

namespace stirling {
	namespace vulkan {
		template <typename T>
		class Deleter {
		public:
			Deleter() : Deleter([](T, VkAllocationCallbacks*) {}) {
			}

			Deleter(std::function<void(T, VkAllocationCallbacks*)> delete_function) :{
				m_delete_function = [=](T object) {
					delete_function(object, nullptr);
				};
			}

			Deleter(const Instance& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> delete_function) {
				m_delete_function = [&instance, delete_function](T object) {
					delete_function(instance, object, nullptr);
				};
			}

			Deleter(const Device& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> delete_function) {
				m_delete_function = [&device, delete_function](T object) {
					delete_function(device, object, nullptr);
				};
			}

			T& operator=(T rhs) {
				if (rhs != object) {
					cleanup();
					object = rhs;
				}
				return *this;
			}

			~Deleter() {
				cleanup();
			}

			const T* operator &() const {
				return &object;
			}

			T* replace() {
				cleanup();
				return &object;
			}

			operator T() const {
				return object;
			}

			template<typename V>
			bool operator==(V rhs) {
				return object == T(rhs);
			}

		private:
			T object{VK_NULL_HANDLE};
			std::function<void(T)> m_delete_function;

			void cleanup() {
				if (object != VK_NULL_HANDLE) {
					m_delete_function(object);
				}
				object = VK_NULL_HANDLE;
			}
		};
	}
}