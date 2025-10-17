#pragma once

#include "Renderer/RenderAPI.h"

namespace nre
{
class VKDevice;

class VKRenderAPI final : public RenderAPI
{
public:
    VKRenderAPI();
    ~VKRenderAPI() override;

    void initialize() override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    RenderCapabilities capabilities() const noexcept override;

private:
    VKDevice* device_ = nullptr;
};
} // namespace nre
