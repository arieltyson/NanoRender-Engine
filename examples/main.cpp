#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Timer.h"
#include "Core/Window.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Renderer/Mesh.h"
#include "Renderer/MeshFactory.h"
#include "Renderer/RenderAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/ShaderLoader.h"
#include "Scene/Camera.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#if defined(NRE_USE_GLFW)
#include <GLFW/glfw3.h>
#endif
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

                const auto sources = nre::loadShaderSources({
                    {nre::ShaderStage::Vertex, "assets/shaders/basic.vert"},
                    {nre::ShaderStage::Fragment, "assets/shaders/basic.frag"}
                });

                shader_ = renderAPI_->createShader(sources);
                shader_->compile();

                mesh_ = renderAPI_->createMesh();
                const auto meshData = nre::makeTriangle();
                mesh_->upload(meshData.vertices, meshData.indices);

                updateProjection();
                updateCamera(0.0F);
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
                const float deltaTime = static_cast<float>(timer().deltaSeconds());
                updateCamera(deltaTime);

                if (shader_ && mesh_)
                {
                    shader_->bind();
                    const nre::Matrix4 model = nre::Matrix4::identity();
                    const nre::Matrix4 mvp = camera_.projection() * camera_.view() * model;
                    shader_->setMatrix4("uMVP", mvp.dataPtr());
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
            updateProjection();
        }

    private:
        void updateProjection()
        {
            const int fbWidth = window().framebufferWidth();
            const int fbHeight = window().framebufferHeight();
            if (fbWidth > 0 && fbHeight > 0)
            {
                const float aspect = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
                camera_.setPerspective(60.0F, aspect, 0.1F, 1000.0F);
            }
        }

        void updateCamera(float deltaTime)
        {
#if defined(NRE_USE_GLFW)
            constexpr float moveSpeed = 2.0F;
            const float velocity = moveSpeed * deltaTime;
            auto& inputState = input();

            const nre::Vector3 forward = (cameraTarget_ - cameraPosition_).normalized();
            const nre::Vector3 right = nre::Vector3::cross(forward, cameraUp_).normalized();

            if (inputState.isKeyDown(GLFW_KEY_W))
            {
                cameraPosition_ += forward * velocity;
                cameraTarget_ += forward * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_S))
            {
                cameraPosition_ -= forward * velocity;
                cameraTarget_ -= forward * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_A))
            {
                cameraPosition_ -= right * velocity;
                cameraTarget_ -= right * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_D))
            {
                cameraPosition_ += right * velocity;
                cameraTarget_ += right * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_Q))
            {
                cameraPosition_ -= cameraUp_ * velocity;
                cameraTarget_ -= cameraUp_ * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_E))
            {
                cameraPosition_ += cameraUp_ * velocity;
                cameraTarget_ += cameraUp_ * velocity;
            }
#else
            (void)deltaTime;
#endif
            camera_.lookAt(cameraPosition_, cameraTarget_, cameraUp_);
        }

        std::unique_ptr<nre::RenderAPI> renderAPI_;
        std::unique_ptr<nre::Shader> shader_;
        std::unique_ptr<nre::Mesh> mesh_;
        nre::Camera camera_;
        nre::Vector3 cameraPosition_{0.0F, 0.0F, 2.0F};
        nre::Vector3 cameraTarget_{0.0F, 0.0F, 0.0F};
        nre::Vector3 cameraUp_{0.0F, 1.0F, 0.0F};
    };

    ExampleApplication app(config);
    app.run();
    return 0;
}
