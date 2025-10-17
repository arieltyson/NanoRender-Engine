#pragma once

#include <functional>
#include <string>

#if defined(NRE_USE_GLFW)
struct GLFWwindow;
#endif

namespace nre
{
enum class WindowAPI
{
    GLFW
};

enum class CursorMode
{
    Normal,
    Hidden,
    Disabled
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
    int framebufferWidth() const noexcept { return framebufferWidth_; }
    int framebufferHeight() const noexcept { return framebufferHeight_; }

    using ResizeCallback = std::function<void(int width, int height)>;
    using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
    using CursorPosCallback = std::function<void(double x, double y)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;

    void setResizeCallback(ResizeCallback callback);
    void setKeyCallback(KeyCallback callback);
    void setCursorPosCallback(CursorPosCallback callback);
    void setMouseButtonCallback(MouseButtonCallback callback);
    void setCursorMode(CursorMode mode);
    CursorMode cursorMode() const noexcept { return cursorMode_; }

private:
    void initializeBackend();
    void shutdownBackend();
    void handleResize(int width, int height);
    void handleKey(int key, int scancode, int action, int mods);
    void handleCursorPos(double x, double y);
    void handleMouseButton(int button, int action, int mods);

#if defined(NRE_USE_GLFW)
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif

    WindowConfig config_;
    void* handle_ = nullptr;
    bool shouldClose_ = false;
    int framebufferWidth_ = 0;
    int framebufferHeight_ = 0;

    ResizeCallback resizeCallback_;
    KeyCallback keyCallback_;
    CursorPosCallback cursorPosCallback_;
    MouseButtonCallback mouseButtonCallback_;
#if defined(NRE_USE_GLFW)
    static CursorMode fromGlfwMode(int mode);
    static int toGlfwMode(CursorMode mode);
#endif
    CursorMode cursorMode_ = CursorMode::Normal;
};
} // namespace nre
