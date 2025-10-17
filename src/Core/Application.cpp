#include "Core/Application.h"

#include "Core/Input.h"
#include "Core/Timer.h"
#include "Core/Window.h"

namespace nre
{
namespace
{
WindowConfig makeWindowConfig(const ApplicationConfig& config)
{
    WindowConfig windowConfig;
    windowConfig.title = config.title;
    windowConfig.width = config.width;
    windowConfig.height = config.height;
    windowConfig.fullscreen = false;
    windowConfig.vsync = true;
    return windowConfig;
}
} // namespace

Application::Application(ApplicationConfig config)
    : config_(std::move(config)),
      window_(std::make_unique<Window>(makeWindowConfig(config_))),
      timer_(std::make_unique<Timer>()),
      input_(std::make_unique<Input>())
{
    window_->setResizeCallback([this](int width, int height) {
        this->onResize(width, height);
    });
    window_->setKeyCallback([this](int key, int scancode, int action, int mods) {
        if (input_)
        {
            input_->handleKeyEvent({key, scancode, action, mods});
        }
        this->onKey(key, scancode, action, mods);
    });
    window_->setCursorPosCallback([this](double x, double y) {
        if (input_)
        {
            input_->handleCursorPosition(x, y);
        }
        this->onMouseMove(x, y);
    });
    window_->setMouseButtonCallback([this](int button, int action, int mods) {
        if (input_)
        {
            input_->handleMouseButtonEvent({button, action, mods});
        }
        this->onMouseButton(button, action, mods);
    });
}

Application::~Application() = default;

void Application::run()
{
    if (running_)
    {
        return;
    }

    running_ = true;
    onInit();

    while (running_ && window_ && !window_->shouldClose())
    {
        if (input_)
        {
            input_->update();
        }
        pollEvents();
        timer_->tick();
        onUpdate();
        window_->swapBuffers();
    }

    onShutdown();
    running_ = false;
}

void Application::onInit() {}

void Application::onUpdate() {}

void Application::onShutdown() {}

void Application::onResize(int /*width*/, int /*height*/) {}

void Application::onKey(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {}

void Application::onMouseMove(double /*x*/, double /*y*/) {}

void Application::onMouseButton(int /*button*/, int /*action*/, int /*mods*/) {}

Window& Application::window() noexcept
{
    return *window_;
}

const Window& Application::window() const noexcept
{
    return *window_;
}

Timer& Application::timer() noexcept
{
    return *timer_;
}

const Timer& Application::timer() const noexcept
{
    return *timer_;
}

Input& Application::input() noexcept
{
    return *input_;
}

const Input& Application::input() const noexcept
{
    return *input_;
}

void Application::pollEvents()
{
    if (window_)
    {
        window_->pollEvents();
    }
}
} // namespace nre
