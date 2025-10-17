#include "Core/Window.h"

#include <stdexcept>
#include <utility>

#if defined(NRE_USE_GLFW)
#include <atomic>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace
{
std::atomic<int>& glfwRefCount()
{
    static std::atomic<int> ref{0};
    return ref;
}

void glfwErrorCallback(int errorCode, const char* description)
{
    std::cerr << "GLFW error (" << errorCode << "): "
              << (description != nullptr ? description : "<no description>") << '\n';
}

void retainGlfw()
{
    auto& ref = glfwRefCount();
    const auto previous = ref.fetch_add(1, std::memory_order_acq_rel);
    if (previous == 0)
    {
        glfwSetErrorCallback(glfwErrorCallback);
        if (glfwInit() == GLFW_FALSE)
        {
            ref.fetch_sub(1, std::memory_order_acq_rel);
            throw std::runtime_error("Failed to initialize GLFW.");
        }
    }
}

void releaseGlfw()
{
    auto& ref = glfwRefCount();
    const auto previous = ref.fetch_sub(1, std::memory_order_acq_rel);
    if (previous == 1)
    {
        glfwTerminate();
    }
}
} // namespace
#endif // NRE_USE_GLFW

namespace nre
{
Window::Window(const WindowConfig& config) : config_(config)
{
    if (config_.api != WindowAPI::GLFW)
    {
        throw std::runtime_error("Requested window API is not yet implemented.");
    }
    initializeBackend();
}

Window::~Window()
{
    shutdownBackend();
}

void Window::pollEvents()
{
#if defined(NRE_USE_GLFW)
    glfwPollEvents();
    auto* window = static_cast<GLFWwindow*>(handle_);
    if (window != nullptr)
    {
        shouldClose_ = glfwWindowShouldClose(window) == GLFW_TRUE;
    }
#else
    shouldClose_ = true;
#endif
}

void Window::swapBuffers()
{
#if defined(NRE_USE_GLFW)
    auto* window = static_cast<GLFWwindow*>(handle_);
    if (window != nullptr)
    {
        glfwSwapBuffers(window);
    }
#endif
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

void Window::initializeBackend()
{
#if defined(NRE_USE_GLFW)
    retainGlfw();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWmonitor* monitor = nullptr;
    int width = config_.width;
    int height = config_.height;
    if (config_.fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
        if (const GLFWvidmode* mode = glfwGetVideoMode(monitor))
        {
            width = mode->width;
            height = mode->height;
        }
    }

    GLFWwindow* window = glfwCreateWindow(width, height, config_.title.c_str(), monitor, nullptr);
    if (window == nullptr)
    {
        releaseGlfw();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(config_.vsync ? 1 : 0);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Window::framebufferSizeCallback);
    glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetCursorPosCallback(window, Window::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Window::mouseButtonCallback);
    handle_ = window;
    shouldClose_ = false;
    config_.width = width;
    config_.height = height;

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    framebufferWidth_ = framebufferWidth;
    framebufferHeight_ = framebufferHeight;

    if (resizeCallback_)
    {
        resizeCallback_(framebufferWidth_, framebufferHeight_);
    }
#else
    (void)config_;
    handle_ = nullptr;
    shouldClose_ = false;
#endif
}

void Window::shutdownBackend()
{
#if defined(NRE_USE_GLFW)
    if (auto* window = static_cast<GLFWwindow*>(handle_); window != nullptr)
    {
        glfwDestroyWindow(window);
        handle_ = nullptr;
    }
    releaseGlfw();
#else
    handle_ = nullptr;
#endif
}

void Window::setResizeCallback(ResizeCallback callback)
{
    resizeCallback_ = std::move(callback);
#if defined(NRE_USE_GLFW)
    if (framebufferWidth_ > 0 && framebufferHeight_ > 0 && resizeCallback_)
    {
        resizeCallback_(framebufferWidth_, framebufferHeight_);
    }
#endif
}

void Window::setKeyCallback(KeyCallback callback)
{
    keyCallback_ = std::move(callback);
}

void Window::setCursorPosCallback(CursorPosCallback callback)
{
    cursorPosCallback_ = std::move(callback);
}

void Window::setMouseButtonCallback(MouseButtonCallback callback)
{
    mouseButtonCallback_ = std::move(callback);
}

void Window::handleResize(int width, int height)
{
    framebufferWidth_ = width;
    framebufferHeight_ = height;

#if defined(NRE_USE_GLFW)
    int windowWidth = 0;
    int windowHeight = 0;
    if (auto* window = static_cast<GLFWwindow*>(handle_); window != nullptr)
    {
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
    }
    if (windowWidth > 0 && windowHeight > 0)
    {
        config_.width = windowWidth;
        config_.height = windowHeight;
    }
#endif

    if (resizeCallback_)
    {
        resizeCallback_(framebufferWidth_, framebufferHeight_);
    }
}

void Window::handleKey(int key, int scancode, int action, int mods)
{
    if (keyCallback_)
    {
        keyCallback_(key, scancode, action, mods);
    }
}

void Window::handleCursorPos(double x, double y)
{
    if (cursorPosCallback_)
    {
        cursorPosCallback_(x, y);
    }
}

void Window::handleMouseButton(int button, int action, int mods)
{
    if (mouseButtonCallback_)
    {
        mouseButtonCallback_(button, action, mods);
    }
}

#if defined(NRE_USE_GLFW)
void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
    {
        self->handleResize(width, height);
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
    {
        self->handleKey(key, scancode, action, mods);
    }
}

void Window::cursorPosCallback(GLFWwindow* window, double x, double y)
{
    if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
    {
        self->handleCursorPos(x, y);
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
    {
        self->handleMouseButton(button, action, mods);
    }
}
#endif
} // namespace nre
