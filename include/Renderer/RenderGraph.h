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
    bool enabled = true;
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

private:
    struct PassRecord
    {
        ResourceHandle handle;
        RenderPass pass;
    };

    std::vector<PassRecord> passes_;
};
} // namespace nre
