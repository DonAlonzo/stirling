#include "input_handler.h"

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace stirling {

    InputHandler& InputHandler::getInstance() {
        static InputHandler instance;
        return instance;
    }

    InputHandler::InputHandler() {
        m_key_bindings[GLFW_KEY_W]            = Action::MOVE_FORWARD;
        m_key_bindings[GLFW_KEY_S]            = Action::MOVE_BACKWARD;
        m_key_bindings[GLFW_KEY_A]            = Action::STRAFE_LEFT;
        m_key_bindings[GLFW_KEY_D]            = Action::STRAFE_RIGHT;
        m_key_bindings[GLFW_KEY_SPACE]        = Action::JUMP;
        m_key_bindings[GLFW_KEY_LEFT_CONTROL] = Action::CROUCH;
        m_key_bindings[GLFW_KEY_ESCAPE]       = Action::EXIT;
        m_key_bindings[GLFW_KEY_F11]          = Action::FULL_SCREEN;
    }

    void InputHandler::onKeyInput(int key, int scancode, int action, int mods) {
        auto binding = m_key_bindings.find(key);
        if (binding != m_key_bindings.end()) {
            m_action_states[binding->second] = action != GLFW_RELEASE;
        }
    }

    bool InputHandler::operator[](Action action) const {
        auto state = m_action_states.find(action);
        if (state != m_action_states.end()) {
            return state->second;
        }
        return false;
    }

}