#pragma once

#include "Renderer/Shader.h"

#include <string>
#include <string_view>
#include <unordered_map>

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

namespace nre
{
class GLShader final : public Shader
{
public:
    explicit GLShader(std::vector<ShaderSource> sources);
    ~GLShader() override;

    void compile() override;
    void reload(const std::vector<ShaderSource>& sources) override;
    void bind() const override;
    void unbind() const override;
    void setMatrix4(std::string_view name, const float* data) override;
    void bindUniformBlock(std::string_view name, unsigned int binding) override;

private:
    unsigned int compileStage(const ShaderSource& source) const;
    void destroyProgram() noexcept;
    int uniformLocation(std::string_view name);

    std::vector<ShaderSource> sources_;
    unsigned int program_ = 0;
    std::unordered_map<std::string, int> uniformLocationCache_;
};
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
