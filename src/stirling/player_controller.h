#pragma once

namespace stirling {
    class PlayerController {
        PlayerController(vulkan::Transform& transform);
        
        void moveForward(float f);
        void moveRight(float f);
        
    private:
        vulkan::Transform& m_transform;
    };
}