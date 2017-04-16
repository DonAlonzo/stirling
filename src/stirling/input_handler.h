#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

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

        void addCommand(Action action, std::function<void()> command);
        void onKeyInput(int key, int scancode, int action, int mods);
        bool operator[](Action action) const;

    private:
        InputHandler();

        std::unordered_map<int, Action> m_key_bindings;
        std::unordered_map<Action, bool> m_action_states;
        std::unordered_map<Action, std::vector<std::function<void()>>> m_commands;

    public:
        InputHandler(const InputHandler&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;
    };
}