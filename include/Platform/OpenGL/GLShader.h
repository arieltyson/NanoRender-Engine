#pragma once

#include "Renderer/Shader.h"

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

private:
    unsigned int compileStage(const ShaderSource& source) const;
    void destroyProgram() noexcept;

    std::vector<ShaderSource> sources_;
    unsigned int program_ = 0;
};
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
