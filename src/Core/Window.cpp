#include "Core/Window.h"

#include <stdexcept>

#if defined(NRE_USE_GLFW)
#include <atomic>

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
    (void)errorCode;
    (void)description;
    // In a production build this should forward to the engine logger.
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
    handle_ = window;
    shouldClose_ = false;
    config_.width = width;
    config_.height = height;
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
} // namespace nre
