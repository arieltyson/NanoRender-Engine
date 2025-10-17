#include "Renderer/ShaderLoader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace nre
{
namespace
{
std::string readFileToString(const std::string& path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
} // namespace

bool ShaderLoader::isCacheValid(const std::vector<ShaderFileDescriptor>& descriptors) const
{
    if (descriptors.size() != cachedDescriptors_.size())
    {
        return false;
    }

    for (std::size_t index = 0; index < descriptors.size(); ++index)
    {
        if (descriptors[index].stage != cachedDescriptors_[index].stage ||
            descriptors[index].path != cachedDescriptors_[index].path)
        {
            return false;
        }

        const auto currentTimestamp = std::filesystem::last_write_time(descriptors[index].path);
        if (currentTimestamp != cachedTimestamps_[index])
        {
            return false;
        }
    }

    return true;
}

ShaderLoader::Result ShaderLoader::load(const std::vector<ShaderFileDescriptor>& descriptors)
{
    if (isCacheValid(descriptors))
    {
        return {cachedSources_, false};
    }

    cachedSources_.clear();
    cachedDescriptors_ = descriptors;
    cachedTimestamps_.resize(descriptors.size());

    cachedSources_.reserve(descriptors.size());
    for (std::size_t index = 0; index < descriptors.size(); ++index)
    {
        const auto& descriptor = descriptors[index];
        ShaderSource source{};
        source.stage = descriptor.stage;
        source.source = readFileToString(descriptor.path);
        source.filePath = descriptor.path;
        cachedSources_.push_back(std::move(source));

        cachedTimestamps_[index] = std::filesystem::last_write_time(descriptor.path);
    }

    return {cachedSources_, true};
}

void ShaderLoader::clear()
{
    cachedSources_.clear();
    cachedDescriptors_.clear();
    cachedTimestamps_.clear();
}
} // namespace nre
