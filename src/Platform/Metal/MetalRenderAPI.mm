#include "Platform/Metal/MetalRenderAPI.h"

#include "Platform/Metal/MetalDevice.h"

namespace nre
{
MetalRenderAPI::MetalRenderAPI() = default;

MetalRenderAPI::~MetalRenderAPI()
{
    shutdown();
}

void MetalRenderAPI::initialize()
{
    if (!device_)
    {
        device_ = new MetalDevice();
        device_->initialize();
    }
}

void MetalRenderAPI::shutdown()
{
    if (device_)
    {
        device_->shutdown();
        delete device_;
        device_ = nullptr;
    }
}

void MetalRenderAPI::beginFrame() {}

void MetalRenderAPI::endFrame() {}

RenderCapabilities MetalRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.rayTracing = false;
    return caps;
}
} // namespace nre
