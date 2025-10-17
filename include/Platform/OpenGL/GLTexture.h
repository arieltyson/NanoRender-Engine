#pragma once

#include "Renderer/Texture.h"

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

namespace nre
{
class GLTexture final : public Texture
{
public:
    explicit GLTexture(const TextureDescriptor& descriptor);
    ~GLTexture() override;

    void loadFromFile(const std::string& path) override;
    void upload(const void* data, std::size_t size) override;
    void bind(std::uint32_t slot) const override;

    std::uint32_t width() const noexcept override { return descriptor_.width; }
    std::uint32_t height() const noexcept override { return descriptor_.height; }
    TextureFormat format() const noexcept override { return descriptor_.format; }

private:
    void ensureCreated();
    void destroy() noexcept;

    TextureDescriptor descriptor_;
    unsigned int textureId_ = 0;
};
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
