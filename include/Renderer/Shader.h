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
    virtual void reload(const std::vector<ShaderSource>& sources) = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void setMatrix4(std::string_view name, const float* data) = 0;
    virtual void setInt(std::string_view name, int value) = 0;
    virtual void bindUniformBlock(std::string_view name, unsigned int binding) = 0;
};
} // namespace nre
