#include "Core/Window.h"

namespace nre
{
Window::Window(const WindowConfig& config) : config_(config)
{
    // Platform-specific window creation will be supplied by platform layers.
}

Window::~Window() = default;

void Window::pollEvents()
{
    // For the placeholder implementation we mark the window for closure
    // after a single event pump so the sample loop exits cleanly.
    shouldClose_ = true;
}

bool Window::shouldClose() const noexcept
{
    return shouldClose_;
}

void* Window::nativeHandle() noexcept
{
    return handle_;
}

const void* Window::nativeHandle() const noexcept
{
    return handle_;
}
} // namespace nre
