#pragma once

#include <string>

namespace nre
{
enum class WindowAPI
{
    GLFW
};

struct WindowConfig
{
    std::string title = "NanoRender Engine";
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool vsync = true;
    WindowAPI api = WindowAPI::GLFW;
};

class Window
{
public:
    explicit Window(const WindowConfig& config);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = delete;
    Window& operator=(Window&&) noexcept = delete;

    void pollEvents();
    void swapBuffers();
    bool shouldClose() const noexcept;
    void* nativeHandle() noexcept;
    const void* nativeHandle() const noexcept;

    const WindowConfig& config() const noexcept { return config_; }

private:
    void initializeBackend();
    void shutdownBackend();

    WindowConfig config_;
    void* handle_ = nullptr;
    bool shouldClose_ = false;
};
} // namespace nre
