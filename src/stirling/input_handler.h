#pragma once

#include <unordered_map>

namespace stirling {
    enum class Action{
        NO_ACTION,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        STRAFE_LEFT,
        STRAFE_RIGHT,
        JUMP,
        CROUCH,
        EXIT,
        FULL_SCREEN
    };

    class InputHandler {
    public:
        static InputHandler& getInstance();

        void onKeyInput(int key, int scancode, int action, int mods);
        bool operator[](Action action) const;

    private:
        InputHandler();

        std::unordered_map<int, Action> m_key_bindings;
        std::unordered_map<Action, bool> m_action_states;

    public:
        InputHandler(const InputHandler&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;
    };
}