#pragma once

#include <memory>

namespace nre
{
enum class APIType
{
    OpenGL,
    Vulkan,
    DirectX12,
    Metal
};

struct RenderCapabilities
{
    bool rayTracing = false;
    bool shaderFloat64 = false;
    bool supportsMeshShaders = false;
};

class CommandBuffer;

class RenderAPI
{
public:
    virtual ~RenderAPI() = default;

    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual RenderCapabilities capabilities() const noexcept = 0;

    static std::unique_ptr<RenderAPI> create(APIType api);
};
} // namespace nre
