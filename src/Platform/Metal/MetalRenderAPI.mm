#include "Platform/Metal/MetalRenderAPI.h"

#include <memory>
#include <stdexcept>

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

void MetalRenderAPI::setViewport(int width, int height)
{
    viewportWidth_ = width;
    viewportHeight_ = height;
}

void MetalRenderAPI::setClearColor(float r, float g, float b, float a)
{
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
}

std::unique_ptr<Mesh> MetalRenderAPI::createMesh()
{
    throw std::runtime_error("Metal mesh creation not implemented yet.");
}

std::unique_ptr<Shader> MetalRenderAPI::createShader(const std::vector<ShaderSource>&)
{
    throw std::runtime_error("Metal shader creation not implemented yet.");
}

RenderCapabilities MetalRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.rayTracing = false;
    return caps;
}
} // namespace nre
