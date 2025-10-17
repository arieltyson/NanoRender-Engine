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

void VKRenderAPI::setViewport(int width, int height)
{
    viewportWidth_ = width;
    viewportHeight_ = height;
}

void VKRenderAPI::setClearColor(float r, float g, float b, float a)
{
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
}

RenderCapabilities VKRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.shaderFloat64 = true;
    return caps;
}
} // namespace nre
