#include "Renderer/RenderGraph.h"

#include <algorithm>
#include <chrono>
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
    statistics_.push_back({passes_.back().handle, passes_.back().pass.name, passes_.back().pass.enabled, 0.0});
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
            for (auto& stats : statistics_)
            {
                if (stats.handle == handle)
                {
                    stats.enabled = enabled;
                    break;
                }
            }
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
    statistics_.clear();
}

void RenderGraph::execute(FrameRenderContext& context)
{
    std::vector<ResourceHandle> executed;
    executed.reserve(passes_.size());

    for (std::size_t index = 0; index < passes_.size(); ++index)
    {
        auto& record = passes_[index];
        if (!record.pass.enabled)
        {
            continue;
        }

        bool dependenciesSatisfied = true;
        for (const auto& dependency : record.pass.dependencies)
        {
            if (!dependency)
            {
                continue;
            }

            const bool dependencyExecuted = std::find(executed.begin(), executed.end(), dependency) != executed.end();
            if (!dependencyExecuted)
            {
                dependenciesSatisfied = false;
                break;
            }
        }

        if (!dependenciesSatisfied)
        {
            continue;
        }

        if (record.pass.setup)
        {
            record.pass.setup(context);
        }

        double elapsedMs = 0.0;
        if (record.pass.execute)
        {
            if (record.pass.measureTime)
            {
                const auto start = std::chrono::steady_clock::now();
                record.pass.execute(context);
                const auto end = std::chrono::steady_clock::now();
                elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();
                record.lastDurationMs = elapsedMs;
            }
            else
            {
                record.pass.execute(context);
                record.lastDurationMs = 0.0;
            }
        }

        for (auto& stats : statistics_)
        {
            if (stats.handle == record.handle)
            {
                stats.name = record.pass.name;
                stats.enabled = record.pass.enabled;
                stats.lastDurationMs = record.lastDurationMs;
                break;
            }
        }

        executed.push_back(record.handle);
    }
}
} // namespace nre
