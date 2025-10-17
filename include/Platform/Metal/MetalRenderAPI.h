#pragma once

#include "Renderer/RenderAPI.h"

namespace nre
{
class MetalDevice;

class MetalRenderAPI final : public RenderAPI
{
public:
    MetalRenderAPI();
    ~MetalRenderAPI() override;

    void initialize() override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    void setViewport(int width, int height) override;
    void setClearColor(float r, float g, float b, float a) override;
    std::unique_ptr<Mesh> createMesh() override;
    std::unique_ptr<Shader> createShader(const std::vector<ShaderSource>& sources) override;
    std::unique_ptr<Texture> createTexture(const TextureDescriptor& descriptor) override;
    RenderCapabilities capabilities() const noexcept override;

private:
    MetalDevice* device_ = nullptr;
    int viewportWidth_ = 0;
    int viewportHeight_ = 0;
    float clearColor_[4] = {0.0F, 0.0F, 0.0F, 1.0F};
};
} // namespace nre
