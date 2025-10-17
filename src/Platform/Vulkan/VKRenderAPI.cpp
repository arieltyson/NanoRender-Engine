#include "Platform/Vulkan/VKRenderAPI.h"

#include "Platform/Vulkan/VKDevice.h"

namespace nre
{
VKRenderAPI::VKRenderAPI() = default;

VKRenderAPI::~VKRenderAPI()
{
    shutdown();
}

void VKRenderAPI::initialize()
{
    if (!device_)
    {
        device_ = new VKDevice();
        device_->initialize();
    }
}

void VKRenderAPI::shutdown()
{
    if (device_)
    {
        device_->shutdown();
        delete device_;
        device_ = nullptr;
    }
}

void VKRenderAPI::beginFrame() {}

void VKRenderAPI::endFrame() {}

RenderCapabilities VKRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.shaderFloat64 = true;
    return caps;
}
} // namespace nre
