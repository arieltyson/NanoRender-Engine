#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "Core/ResourceHandle.h"

namespace nre
{
class RenderAPI;

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
    std::vector<ResourceHandle> dependencies;
    bool enabled = true;
    bool measureTime = true;
};

class RenderGraph
{
public:
    RenderGraph() = default;

    ResourceHandle addPass(RenderPass pass);
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

    std::vector<PassRecord> passes_;
    std::vector<PassStatistics> statistics_;
};
} // namespace nre
