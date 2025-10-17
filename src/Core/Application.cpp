#include "Core/Application.h"

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
      timer_(std::make_unique<Timer>())
{
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
        pollEvents();
        timer_->tick();
        onUpdate();
    }

    onShutdown();
    running_ = false;
}

void Application::onInit() {}

void Application::onUpdate() {}

void Application::onShutdown() {}

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

void Application::pollEvents()
{
    if (window_)
    {
        window_->pollEvents();
    }
}
} // namespace nre
