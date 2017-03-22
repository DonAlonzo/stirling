#pragma once

typedef struct GLFWwindow GLFWwindow;

namespace stirling {
	namespace vulkan {
		class Window {
		public:
			Window(int width, int height);
			
			~Window();

			Window(const Window& rhs) = delete;
			Window& operator=(const Window& rhs) = delete;

			Window(Window&& rhs) = default;
			Window& operator=(Window&& rhs) = default;

			bool isRunning();
			void update();

		private:
			GLFWwindow* m_window;
		};
	}
}