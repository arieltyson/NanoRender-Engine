#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Timer.h"
#include "Core/Window.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Renderer/Mesh.h"
#include "Renderer/MeshFactory.h"
#include "Renderer/MeshCache.h"
#include "Renderer/RenderAPI.h"
#include "Renderer/RenderGraph.h"
#include "Renderer/Shader.h"
#include "Renderer/ShaderLoader.h"
#include "Renderer/Texture.h"
#include "Renderer/TextureLoader.h"
#include "Scene/Camera.h"

#include <cmath>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

#if defined(NRE_USE_GLFW)
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif
int main()
{
    nre::ApplicationConfig config;
    config.title = "NanoRender Example";
    config.width = 800;
    config.height = 600;

    struct alignas(16) FrameData
    {
        nre::Matrix4 viewProjection{nre::Matrix4::identity()};
        float time = 0.0F;
        float padding[3] = {0.0F, 0.0F, 0.0F};
    };

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

#if defined(NRE_USE_GLFW)
                ImGui::CreateContext();
                ImGui::StyleColorsDark();
                ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window().nativeHandle()), true);
                ImGui_ImplOpenGL3_Init("#version 410");
#endif

                meshCache_ = std::make_unique<nre::MeshCache>(*renderAPI_);
                textureLoader_ = std::make_unique<nre::TextureLoader>(*renderAPI_);

                meshCache_ = std::make_unique<nre::MeshCache>(*renderAPI_);
                textureLoader_ = std::make_unique<nre::TextureLoader>(*renderAPI_);

                shaderDescriptors_ = {
                    {nre::ShaderStage::Vertex, "assets/shaders/basic.vert"},
                    {nre::ShaderStage::Fragment, "assets/shaders/basic.frag"}
                };

                const auto loadResult = shaderLoader_.load(shaderDescriptors_);

                shader_ = renderAPI_->createShader(loadResult.sources);
                shader_->compile();
                shader_->bindUniformBlock("FrameData", 0);

                mesh_ = meshCache_->loadFromGenerator("triangle", [] {
                    return nre::makeTriangle();
                });

                if (textureLoader_)
                {
                    texture_ = textureLoader_->load("assets/textures/albedo.ppm");
                }

                updateProjection();
                updateCamera(0.0F);

                glGenBuffers(1, &frameUniformBuffer_);
                glBindBuffer(GL_UNIFORM_BUFFER, frameUniformBuffer_);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), nullptr, GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, 0, frameUniformBuffer_);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                shader_->bind();
                shader_->setMatrix4("uModel", nre::Matrix4::identity().dataPtr());
                shader_->setInt("uAlbedo", 0);
                shader_->unbind();

                captureCursor(true);

                nre::FrameRenderContext bootstrap{*renderAPI_, frameIndex_, 0.0, 0.0, this};
                updateFrameData(bootstrap);

                renderGraph_.clear();
                frameUniformResource_ = renderGraph_.addResource({"FrameDataUBO", nre::RenderResourceType::UniformBuffer, true});
                swapchainResource_ = renderGraph_.addResource({"SwapchainColor", nre::RenderResourceType::ColorTarget, true});
                framePassHandle_ = renderGraph_.addPass({
                    "FrameUniforms",
                    nullptr,
                    [this](nre::FrameRenderContext& context) {
                        updateFrameData(context);
                    },
                    {},
                    {frameUniformResource_}
                });

                geometryPassHandle_ = renderGraph_.addPass({
                    "Geometry",
                    [this](nre::FrameRenderContext&) {
                        if (texture_)
                        {
                            texture_->bind(0);
                        }
                    },
                    [this](nre::FrameRenderContext&) {
                        if (!shader_ || !mesh_)
                        {
                            return;
                        }
                        shader_->bind();
                        mesh_->draw();
                        shader_->unbind();
                    },
                    {frameUniformResource_},
                    {swapchainResource_}
                });

#if defined(NRE_USE_GLFW)
                uiPassHandle_ = renderGraph_.addPass({
                    "Diagnostics",
                    nullptr,
                    [this](nre::FrameRenderContext& context) {
                        ImGui::Begin("Diagnostics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                        ImGui::Text("Frame: %llu", static_cast<unsigned long long>(context.frameIndex));
                        ImGui::Text("Delta: %.3f ms", context.deltaSeconds * 1000.0);
                        const float fps = context.deltaSeconds > 0.0 ? 1.0f / static_cast<float>(context.deltaSeconds) : 0.0f;
                        ImGui::Text("FPS: %.1f", fps);
                        ImGui::Separator();
                        for (const auto& stats : renderGraph_.statistics())
                        {
                            bool enabled = stats.enabled;
                            if (ImGui::Checkbox(stats.name.c_str(), &enabled))
                            {
                                renderGraph_.setPassEnabled(stats.handle, enabled);
                            }
                            ImGui::SameLine();
                            ImGui::Text("%.3f ms", stats.lastDurationMs);
                        }
                        ImGui::End();
                    },
                    {frameUniformResource_, swapchainResource_},
                    {swapchainResource_}
                });
#else
                uiPassHandle_ = {};
#endif
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
#if defined(NRE_USE_GLFW)
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
#endif
                const float deltaTime = static_cast<float>(timer().deltaSeconds());
                updateCamera(deltaTime);

                const auto reloadResult = shaderLoader_.load(shaderDescriptors_);
                if (reloadResult.reloaded && shader_)
                {
                    try
                    {
                        shader_->reload(reloadResult.sources);
                        shader_->bindUniformBlock("FrameData", 0);
                        shader_->bind();
                        shader_->setMatrix4("uModel", nre::Matrix4::identity().dataPtr());
                        shader_->setInt("uAlbedo", 0);
                        shader_->unbind();
                    }
                    catch (const std::exception& ex)
                    {
                        std::cerr << "Shader reload failed: " << ex.what() << '\n';
                    }
                }

                nre::FrameRenderContext frameContext{
                    *renderAPI_,
                    frameIndex_++,
                    deltaTime,
                    timer().elapsedSeconds(),
                    this
                };
                renderGraph_.execute(frameContext);
#if defined(NRE_USE_GLFW)
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
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
            texture_.reset();
            if (frameUniformBuffer_ != 0)
            {
                glDeleteBuffers(1, &frameUniformBuffer_);
                frameUniformBuffer_ = 0;
            }
            captureCursor(false);
            if (textureLoader_)
            {
                textureLoader_->clear();
                textureLoader_.reset();
            }
            if (meshCache_)
            {
                meshCache_->clear();
                meshCache_.reset();
            }
#if defined(NRE_USE_GLFW)
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#endif
        }

        void onResize(int width, int height) override
        {
            if (renderAPI_)
            {
                renderAPI_->setViewport(width, height);
            }
            updateProjection();
        }

        void onKey(int key, int scancode, int action, int mods) override
        {
            nre::Application::onKey(key, scancode, action, mods);
#if defined(NRE_USE_GLFW)
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                captureCursor(false);
            }
#endif
        }

        void onMouseButton(int button, int action, int mods) override
        {
            nre::Application::onMouseButton(button, action, mods);
#if defined(NRE_USE_GLFW)
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !cursorCaptured_)
            {
                captureCursor(true);
            }
#endif
        }

    private:
        void captureCursor(bool capture)
        {
            cursorCaptured_ = capture;
            window().setCursorMode(capture ? nre::CursorMode::Disabled : nre::CursorMode::Normal);
            input().reset();
        }

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

            const nre::Vector3 forward = cameraForward_.normalized();
            const nre::Vector3 right = nre::Vector3::cross(forward, cameraUp_).normalized();

            if (inputState.isKeyDown(GLFW_KEY_W))
            {
                cameraPosition_ += forward * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_S))
            {
                cameraPosition_ -= forward * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_A))
            {
                cameraPosition_ -= right * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_D))
            {
                cameraPosition_ += right * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_Q))
            {
                cameraPosition_ -= cameraUp_ * velocity;
            }
            if (inputState.isKeyDown(GLFW_KEY_E))
            {
                cameraPosition_ += cameraUp_ * velocity;
            }

            if (cursorCaptured_)
            {
                constexpr float sensitivity = 0.1F;
                yaw_ += static_cast<float>(inputState.cursorDeltaX()) * sensitivity;
                pitch_ -= static_cast<float>(inputState.cursorDeltaY()) * sensitivity;
                if (pitch_ > 89.0F)
                {
                    pitch_ = 89.0F;
                }
                if (pitch_ < -89.0F)
                {
                    pitch_ = -89.0F;
                }

                constexpr float degToRad = 3.14159265358979323846F / 180.0F;
                const float yawRad = yaw_ * degToRad;
                const float pitchRad = pitch_ * degToRad;
                nre::Vector3 direction;
                direction.x = std::cos(yawRad) * std::cos(pitchRad);
                direction.y = std::sin(pitchRad);
                direction.z = std::sin(yawRad) * std::cos(pitchRad);
                cameraForward_ = direction.normalized();
            }
#else
            (void)deltaTime;
#endif
            cameraTarget_ = cameraPosition_ + cameraForward_;
            camera_.lookAt(cameraPosition_, cameraTarget_, cameraUp_);
        }

        void updateFrameData(const nre::FrameRenderContext& context)
        {
            frameData_.viewProjection = camera_.projection() * camera_.view();
            frameData_.time = static_cast<float>(context.elapsedSeconds);
            glBindBuffer(GL_UNIFORM_BUFFER, frameUniformBuffer_);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FrameData), &frameData_);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        std::unique_ptr<nre::RenderAPI> renderAPI_;
        std::unique_ptr<nre::Shader> shader_;
        std::shared_ptr<nre::Mesh> mesh_;
        std::shared_ptr<nre::Texture> texture_;
        nre::Camera camera_;
        nre::Vector3 cameraPosition_{0.0F, 0.0F, 2.0F};
        nre::Vector3 cameraTarget_{0.0F, 0.0F, 1.0F};
        nre::Vector3 cameraUp_{0.0F, 1.0F, 0.0F};
        nre::Vector3 cameraForward_{0.0F, 0.0F, -1.0F};
        float yaw_ = -90.0F;
        float pitch_ = 0.0F;
        bool cursorCaptured_ = false;
        GLuint frameUniformBuffer_ = 0;
        FrameData frameData_{};
        nre::RenderGraph renderGraph_;
        nre::ResourceHandle framePassHandle_{};
        nre::ResourceHandle geometryPassHandle_{};
        nre::ResourceHandle uiPassHandle_{};
        nre::ResourceHandle frameUniformResource_{};
        nre::ResourceHandle swapchainResource_{};
        nre::ShaderLoader shaderLoader_;
        std::unique_ptr<nre::TextureLoader> textureLoader_;
        std::unique_ptr<nre::MeshCache> meshCache_;
        std::vector<nre::ShaderFileDescriptor> shaderDescriptors_;
        std::uint64_t frameIndex_ = 0;
    };

    ExampleApplication app(config);
    app.run();
    return 0;
}
