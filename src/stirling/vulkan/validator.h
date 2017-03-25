#pragma once

// Vulkan
#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {

		class Validator {
		public:
			Validator(VkInstance instance);
			~Validator();
			Validator(Validator&&) = default;
			Validator(const Validator&) = delete;
			Validator& operator=(Validator&&) = delete;
			Validator& operator=(const Validator&) = delete;

			static Validator nullValidator();

		private:
			VkInstance               m_instance;
			VkDebugReportCallbackEXT m_debug_callback;

			Validator();

			VkDebugReportCallbackEXT initDebugCallback() const;
		};

	}
}