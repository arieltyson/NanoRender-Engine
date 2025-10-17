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
    RenderCapabilities capabilities() const noexcept override;

private:
    MetalDevice* device_ = nullptr;
};
} // namespace nre
