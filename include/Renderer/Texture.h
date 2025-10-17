#pragma once

#include <cstdint>
#include <string>

namespace nre
{
enum class TextureFormat : std::uint8_t
{
    RGBA8,
    RGBA16F,
    Depth24Stencil8
};

struct TextureDescriptor
{
    std::uint32_t width = 1;
    std::uint32_t height = 1;
    TextureFormat format = TextureFormat::RGBA8;
    bool generateMipmaps = true;
};

class Texture
{
public:
    virtual ~Texture();

    virtual void loadFromFile(const std::string& path) = 0;
    virtual void upload(const void* data, std::size_t size) = 0;
    virtual void bind(std::uint32_t slot) const = 0;
};
} // namespace nre
