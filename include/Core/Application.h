#pragma once

#include <memory>
#include <string>

namespace nre
{
class Window;
class Timer;
class Input;

struct ApplicationConfig
{
    std::string title = "NanoRender Engine";
    int width = 1280;
    int height = 720;
    bool enableValidation = false;
};

class Application
{
public:
    explicit Application(ApplicationConfig config);
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) noexcept = delete;
    Application& operator=(Application&&) noexcept = delete;

    void run();

protected:
    virtual void onInit();
    virtual void onUpdate();
    virtual void onShutdown();
    virtual void onResize(int width, int height);
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void onMouseMove(double x, double y);
    virtual void onMouseButton(int button, int action, int mods);

    Window& window() noexcept;
    const Window& window() const noexcept;
    Timer& timer() noexcept;
    const Timer& timer() const noexcept;
    Input& input() noexcept;
    const Input& input() const noexcept;

private:
    void pollEvents();

    ApplicationConfig config_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<Input> input_;
    bool running_ = false;
};
} // namespace nre
