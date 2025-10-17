#include "Core/Application.h"
#include "Core/Window.h"
#include "Renderer/RenderAPI.h"

#include <exception>
#include <iostream>
#include <memory>

int main()
{
    nre::ApplicationConfig config;
    config.title = "NanoRender Example";
    config.width = 800;
    config.height = 600;

    class ExampleApplication : public nre::Application
    {
    public:
        explicit ExampleApplication(const nre::ApplicationConfig& cfg)
            : nre::Application(cfg)
        {
        }

    protected:
        void onInit() override
        {
            try
            {
                renderAPI_ = nre::RenderAPI::create(nre::APIType::OpenGL);
                renderAPI_->initialize();
                renderAPI_->setViewport(window().config().width, window().config().height);
                renderAPI_->setClearColor(0.1F, 0.12F, 0.25F, 1.0F);
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Failed to initialize rendering backend: " << ex.what() << '\n';
                renderAPI_.reset();
            }
        }

        void onUpdate() override
        {
            if (renderAPI_)
            {
                renderAPI_->beginFrame();
                renderAPI_->endFrame();
            }
        }

        void onShutdown() override
        {
            if (renderAPI_)
            {
                renderAPI_->shutdown();
                renderAPI_.reset();
            }
        }

    private:
        std::unique_ptr<nre::RenderAPI> renderAPI_;
    };

    ExampleApplication app(config);
    app.run();
    return 0;
}
