#include "stirling/vulkan/instance.h"
#include "stirling/window.h"

#include <iostream>

class StirlingDemo {
public:
    StirlingDemo() :
        m_window (800, 600) {
    }

    void run() {
        while (m_window.isRunning()) {
            m_window.update();
            m_window.render();
        }
    }

private:
    stirling::Window m_window;
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