#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace stirling {
	namespace vulkan {
		class Window {
		public:
			Window(int width, int height);
			
			~Window();

			bool isRunning();
		};
	}
}