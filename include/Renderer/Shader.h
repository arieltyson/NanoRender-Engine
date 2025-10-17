#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace nre
{
enum class ShaderStage : std::uint8_t
{
    Vertex,
    Fragment,
    Compute
};

struct ShaderSource
{
    ShaderStage stage;
    std::string source;
    std::string entryPoint = "main";
    std::string filePath;
};

class Shader
{
public:
    virtual ~Shader();

    virtual void compile() = 0;
    virtual void reload(std::string_view newSource) = 0;
};
} // namespace nre
