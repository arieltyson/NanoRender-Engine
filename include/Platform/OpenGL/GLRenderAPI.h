#pragma once

#include "Renderer/RenderAPI.h"

namespace nre
{
class GLContext;

class GLRenderAPI final : public RenderAPI
{
public:
    GLRenderAPI();
    ~GLRenderAPI() override;

    void initialize() override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    RenderCapabilities capabilities() const noexcept override;

private:
    GLContext* context_ = nullptr;
};
} // namespace nre
