#include "Platform/DirectX12/DX12RenderAPI.h"

#include <memory>
#include <stdexcept>

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

void DX12RenderAPI::setViewport(int width, int height)
{
    viewportWidth_ = width;
    viewportHeight_ = height;
}

void DX12RenderAPI::setClearColor(float r, float g, float b, float a)
{
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
}

std::unique_ptr<Mesh> DX12RenderAPI::createMesh()
{
    throw std::runtime_error("DirectX 12 mesh creation not implemented yet.");
}

std::unique_ptr<Shader> DX12RenderAPI::createShader(const std::vector<ShaderSource>&)
{
    throw std::runtime_error("DirectX 12 shader creation not implemented yet.");
}

RenderCapabilities DX12RenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.rayTracing = true;
    return caps;
}
} // namespace nre
