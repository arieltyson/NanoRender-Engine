#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "Core/ResourceHandle.h"

namespace nre
{
class RenderAPI;

enum class RenderResourceType
{
    ColorTarget,
    DepthTarget,
    UniformBuffer,
    Texture,
    External
};

struct RenderResourceDesc
{
    std::string name;
    RenderResourceType type = RenderResourceType::External;
    bool external = true;
};

struct FrameRenderContext
{
    RenderAPI& renderAPI;
    std::uint64_t frameIndex = 0;
    double deltaSeconds = 0.0;
    double elapsedSeconds = 0.0;
    void* userData = nullptr;
};

struct RenderPass
{
    std::string name;
    std::function<void(FrameRenderContext&)> setup;
    std::function<void(FrameRenderContext&)> execute;
    std::vector<ResourceHandle> reads;
    std::vector<ResourceHandle> writes;
    std::vector<ResourceHandle> dependencies;
    bool enabled = true;
    bool measureTime = true;
};

class RenderGraph
{
public:
    RenderGraph() = default;

    ResourceHandle addPass(RenderPass pass);
    ResourceHandle addResource(RenderResourceDesc desc);
    void setPassEnabled(ResourceHandle handle, bool enabled);
    bool isPassEnabled(ResourceHandle handle) const;
    void clear();

    void execute(FrameRenderContext& context);

    struct PassStatistics
    {
        ResourceHandle handle;
        std::string name;
        bool enabled = false;
        double lastDurationMs = 0.0;
    };

    const std::vector<PassStatistics>& statistics() const noexcept { return statistics_; }

private:
    struct PassRecord
    {
        ResourceHandle handle;
        RenderPass pass;
        double lastDurationMs = 0.0;
    };

    struct ResourceRecord
    {
        ResourceHandle handle;
        RenderResourceDesc desc;
        ResourceHandle lastWriter;
    };

    ResourceRecord* findResource(ResourceHandle handle);
    const ResourceRecord* findResource(ResourceHandle handle) const;

    std::vector<PassRecord> passes_;
    std::vector<ResourceRecord> resources_;
    std::vector<PassStatistics> statistics_;
    std::uint64_t nextPassId_ = 0;
    std::uint64_t nextResourceId_ = 0;
};
} // namespace nre
