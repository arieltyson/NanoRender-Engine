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

std::vector<ShaderSource> loadShaderSources(const std::vector<ShaderFileDescriptor>& descriptors)
{
    std::vector<ShaderSource> sources;
    sources.reserve(descriptors.size());

    for (const auto& [stage, path] : descriptors)
    {
        ShaderSource source{};
        source.stage = stage;
        source.source = readFileToString(path);
        source.filePath = path;
        sources.push_back(std::move(source));
    }

    return sources;
}
} // namespace nre
