#include "Core/Input.h"

namespace nre
{
void Input::update()
{
    for (auto& [key, state] : keyStates_)
    {
        state.pressed = false;
        state.released = false;
    }

    for (auto& [button, state] : mouseButtonStates_)
    {
        state.pressed = false;
        state.released = false;
    }

    cursorDeltaX_ = 0.0;
    cursorDeltaY_ = 0.0;
}

void Input::handleKeyEvent(const KeyEvent& event)
{
    auto& state = keyStates_[event.key];
    switch (event.action)
    {
    case 0: // GLFW_RELEASE
        if (state.down)
        {
            state.down = false;
            state.released = true;
        }
        break;
    case 1: // GLFW_PRESS
        if (!state.down)
        {
            state.down = true;
            state.pressed = true;
        }
        break;
    case 2: // GLFW_REPEAT
        state.down = true;
        break;
    default:
        break;
    }
}

void Input::handleMouseButtonEvent(const MouseButtonEvent& event)
{
    auto& state = mouseButtonStates_[event.button];
    switch (event.action)
    {
    case 0: // GLFW_RELEASE
        if (state.down)
        {
            state.down = false;
            state.released = true;
        }
        break;
    case 1: // GLFW_PRESS
        if (!state.down)
        {
            state.down = true;
            state.pressed = true;
        }
        break;
    default:
        break;
    }
}

void Input::handleCursorPosition(double x, double y)
{
    cursorX_ = x;
    cursorY_ = y;

    if (firstCursorEvent_)
    {
        lastCursorX_ = x;
        lastCursorY_ = y;
        cursorDeltaX_ = 0.0;
        cursorDeltaY_ = 0.0;
        firstCursorEvent_ = false;
        return;
    }

    cursorDeltaX_ += x - lastCursorX_;
    cursorDeltaY_ += y - lastCursorY_;
    lastCursorX_ = x;
    lastCursorY_ = y;
}

bool Input::isKeyDown(int key) const noexcept
{
    if (const auto it = keyStates_.find(key); it != keyStates_.end())
    {
        return it->second.down;
    }
    return false;
}

bool Input::wasKeyPressed(int key) const noexcept
{
    if (const auto it = keyStates_.find(key); it != keyStates_.end())
    {
        return it->second.pressed;
    }
    return false;
}

bool Input::wasKeyReleased(int key) const noexcept
{
    if (const auto it = keyStates_.find(key); it != keyStates_.end())
    {
        return it->second.released;
    }
    return false;
}

bool Input::isMouseButtonDown(int button) const noexcept
{
    if (const auto it = mouseButtonStates_.find(button); it != mouseButtonStates_.end())
    {
        return it->second.down;
    }
    return false;
}

bool Input::wasMouseButtonPressed(int button) const noexcept
{
    if (const auto it = mouseButtonStates_.find(button); it != mouseButtonStates_.end())
    {
        return it->second.pressed;
    }
    return false;
}

bool Input::wasMouseButtonReleased(int button) const noexcept
{
    if (const auto it = mouseButtonStates_.find(button); it != mouseButtonStates_.end())
    {
        return it->second.released;
    }
    return false;
}

void Input::reset()
{
    keyStates_.clear();
    mouseButtonStates_.clear();
    cursorX_ = 0.0;
    cursorY_ = 0.0;
    lastCursorX_ = 0.0;
    lastCursorY_ = 0.0;
    cursorDeltaX_ = 0.0;
    cursorDeltaY_ = 0.0;
    firstCursorEvent_ = true;
}

} // namespace nre
