#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Renderer/Shader.h"

namespace nre
{
struct ShaderFileDescriptor
{
    ShaderStage stage;
    std::string path;
};

class ShaderLoader
{
public:
    struct Result
    {
        const std::vector<ShaderSource>& sources;
        bool reloaded = false;
    };

    Result load(const std::vector<ShaderFileDescriptor>& descriptors);
    void clear();

private:
    std::vector<ShaderSource> cachedSources_;
    std::vector<ShaderFileDescriptor> cachedDescriptors_;
    std::vector<std::filesystem::file_time_type> cachedTimestamps_;

    bool isCacheValid(const std::vector<ShaderFileDescriptor>& descriptors) const;
};
} // namespace nre
