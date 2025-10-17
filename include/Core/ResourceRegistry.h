#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Core/ResourceHandle.h"

namespace nre
{
template <typename T>
class ResourceRegistry
{
public:
    ResourceHandle add(std::shared_ptr<T> resource)
    {
        const ResourceHandle handle{++nextId_};
        resources_.emplace(handle.id, std::move(resource));
        return handle;
    }

    std::shared_ptr<T> get(ResourceHandle handle) const
    {
        if (handle)
        {
            if (const auto it = resources_.find(handle.id); it != resources_.end())
            {
                return it->second;
            }
        }
        return nullptr;
    }

    void remove(ResourceHandle handle)
    {
        if (handle)
        {
            resources_.erase(handle.id);
        }
    }

    void clear()
    {
        resources_.clear();
        nextId_ = 0;
    }

private:
    std::unordered_map<std::uint64_t, std::shared_ptr<T>> resources_;
    std::uint64_t nextId_ = 0;
};
} // namespace nre
