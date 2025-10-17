#pragma once

#include <cstdint>
#include <unordered_map>

namespace nre
{
struct KeyEvent
{
    int key = 0;
    int scancode = 0;
    int action = 0;
    int mods = 0;
};

struct MouseButtonEvent
{
    int button = 0;
    int action = 0;
    int mods = 0;
};

class Input
{
public:
    void update();

    void handleKeyEvent(const KeyEvent& event);
    void handleMouseButtonEvent(const MouseButtonEvent& event);
    void handleCursorPosition(double x, double y);

    bool isKeyDown(int key) const noexcept;
    bool wasKeyPressed(int key) const noexcept;
    bool wasKeyReleased(int key) const noexcept;

    bool isMouseButtonDown(int button) const noexcept;
    bool wasMouseButtonPressed(int button) const noexcept;
    bool wasMouseButtonReleased(int button) const noexcept;

    double cursorX() const noexcept { return cursorX_; }
    double cursorY() const noexcept { return cursorY_; }
    double cursorDeltaX() const noexcept { return cursorDeltaX_; }
    double cursorDeltaY() const noexcept { return cursorDeltaY_; }

    void reset();

private:
    struct ButtonState
    {
        bool down = false;
        bool pressed = false;
        bool released = false;
    };

    using KeyStateMap = std::unordered_map<int, ButtonState>;

    KeyStateMap keyStates_;
    KeyStateMap mouseButtonStates_;
    double cursorX_ = 0.0;
    double cursorY_ = 0.0;
    double lastCursorX_ = 0.0;
    double lastCursorY_ = 0.0;
    double cursorDeltaX_ = 0.0;
    double cursorDeltaY_ = 0.0;
    bool firstCursorEvent_ = true;
};
} // namespace nre
