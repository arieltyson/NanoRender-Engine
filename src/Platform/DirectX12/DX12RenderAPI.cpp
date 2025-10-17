#include "Platform/DirectX12/DX12RenderAPI.h"

#include "Platform/DirectX12/DX12Device.h"

namespace nre
{
DX12RenderAPI::DX12RenderAPI() = default;

DX12RenderAPI::~DX12RenderAPI()
{
    shutdown();
}

void DX12RenderAPI::initialize()
{
    if (!device_)
    {
        device_ = new DX12Device();
        device_->initialize();
    }
}

void DX12RenderAPI::shutdown()
{
    if (device_)
    {
        device_->shutdown();
        delete device_;
        device_ = nullptr;
    }
}

void DX12RenderAPI::beginFrame() {}

void DX12RenderAPI::endFrame() {}

RenderCapabilities DX12RenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.rayTracing = true;
    return caps;
}
} // namespace nre
