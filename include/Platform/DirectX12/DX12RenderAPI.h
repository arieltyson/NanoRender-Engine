#pragma once

#include "Renderer/RenderAPI.h"

namespace nre
{
class DX12Device;

class DX12RenderAPI final : public RenderAPI
{
public:
    DX12RenderAPI();
    ~DX12RenderAPI() override;

    void initialize() override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    RenderCapabilities capabilities() const noexcept override;

private:
    DX12Device* device_ = nullptr;
};
} // namespace nre
