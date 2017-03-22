#include "stirling/vulkan/window.h"

#include <iostream>

class StirlingDemo {
public:
	StirlingDemo() :
		m_window (initWindow()) {
	}

	void run() {
		while (m_window.isRunning()) {
		}
	}

private:
	stirling::vulkan::Window m_window;

	stirling::vulkan::Window initWindow() {
		return stirling::vulkan::Window(800, 600);
	}
};

int main() {
	try {
		StirlingDemo().run();
		return 0;
	} catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}