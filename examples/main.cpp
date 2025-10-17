#include "Core/Application.h"
#include "Core/Window.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderAPI.h"
#include "Renderer/Shader.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

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
                renderAPI_->setViewport(window().framebufferWidth(), window().framebufferHeight());
                renderAPI_->setClearColor(0.1F, 0.12F, 0.25F, 1.0F);

                const char* vertexSource = R"(#version 410 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 vColor;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    vColor = normalize(aNormal * 0.5 + 0.5);
}
)";

                const char* fragmentSource = R"(#version 410 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
)";

                std::vector<nre::ShaderSource> sources{
                    {nre::ShaderStage::Vertex, vertexSource},
                    {nre::ShaderStage::Fragment, fragmentSource}
                };

                shader_ = renderAPI_->createShader(sources);
                shader_->compile();

                mesh_ = renderAPI_->createMesh();
                std::vector<nre::Vertex> vertices{
                    {{-0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}},
                    {{0.0F, 0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.5F, 1.0F}},
                    {{0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}
                };
                std::vector<std::uint32_t> indices{0U, 1U, 2U};
                mesh_->upload(vertices, indices);
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Failed to initialize rendering backend: " << ex.what() << '\n';
                renderAPI_.reset();
                shader_.reset();
                mesh_.reset();
            }
        }

        void onUpdate() override
        {
            if (renderAPI_)
            {
                renderAPI_->beginFrame();
                if (shader_ && mesh_)
                {
                    shader_->bind();
                    mesh_->draw();
                    shader_->unbind();
                }
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
            shader_.reset();
            mesh_.reset();
        }

        void onResize(int width, int height) override
        {
            if (renderAPI_)
            {
                renderAPI_->setViewport(width, height);
            }
        }

    private:
        std::unique_ptr<nre::RenderAPI> renderAPI_;
        std::unique_ptr<nre::Shader> shader_;
        std::unique_ptr<nre::Mesh> mesh_;
    };

    ExampleApplication app(config);
    app.run();
    return 0;
}
