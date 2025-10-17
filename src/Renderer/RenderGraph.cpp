#include "Renderer/RenderGraph.h"

#include <stdexcept>

namespace nre
{
namespace
{
ResourceHandle makeHandle(std::uint64_t id)
{
    return ResourceHandle{id};
}
}

ResourceHandle RenderGraph::addPass(RenderPass pass)
{
    if (!pass.execute)
    {
        throw std::invalid_argument("RenderGraph pass requires an execute callback.");
    }

    const std::uint64_t newId = passes_.size() + 1;
    passes_.push_back(PassRecord{makeHandle(newId), std::move(pass)});
    return passes_.back().handle;
}

void RenderGraph::setPassEnabled(ResourceHandle handle, bool enabled)
{
    if (!handle)
    {
        return;
    }

    for (auto& record : passes_)
    {
        if (record.handle == handle)
        {
            record.pass.enabled = enabled;
            break;
        }
    }
}

bool RenderGraph::isPassEnabled(ResourceHandle handle) const
{
    if (!handle)
    {
        return false;
    }

    for (const auto& record : passes_)
    {
        if (record.handle == handle)
        {
            return record.pass.enabled;
        }
    }
    return false;
}

void RenderGraph::clear()
{
    passes_.clear();
}

void RenderGraph::execute(FrameRenderContext& context)
{
    for (auto& record : passes_)
    {
        if (!record.pass.enabled)
        {
            continue;
        }

        if (record.pass.setup)
        {
            record.pass.setup(context);
        }

        if (record.pass.execute)
        {
            record.pass.execute(context);
        }
    }
}
} // namespace nre
