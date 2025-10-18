#include "Renderer/TextureLoader.h"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#include "Renderer/RenderAPI.h"
#include "Renderer/Texture.h"

namespace nre
{
TextureLoader::TextureLoader(RenderAPI& api) : api_(&api) {}

std::shared_ptr<Texture> TextureLoader::load(const std::string& path)
{
    if (auto it = cache_.find(path); it != cache_.end())
    {
        if (auto existing = it->second.lock())
        {
            return existing;
        }
    }

    if (api_ == nullptr)
    {
        throw std::runtime_error("TextureLoader has no associated RenderAPI.");
    }

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    std::vector<unsigned char> rgba;
    if (pixels == nullptr || width <= 0 || height <= 0)
    {
        // Fallback 2x2 checkerboard (magenta/black) so failures are obvious but non-fatal.
        width = height = 2;
        rgba = {
            255, 0, 255, 255,   0, 0, 0, 255,
            0, 0, 0, 255,       255, 0, 255, 255
        };
    }
    else
    {
        const std::size_t pixelCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
        rgba.assign(pixels, pixels + pixelCount * 4);
        stbi_image_free(pixels);
    }

    TextureDescriptor descriptor;
    descriptor.width = static_cast<std::uint32_t>(width);
    descriptor.height = static_cast<std::uint32_t>(height);
    descriptor.format = TextureFormat::RGBA8;

    auto texture = api_->createTexture(descriptor);
    if (!texture)
    {
        throw std::runtime_error("RenderAPI failed to create texture for: " + path);
    }

    texture->upload(rgba.data(), rgba.size());

    auto shared = std::shared_ptr<Texture>(texture.release());
    cache_[path] = shared;
    return shared;
}

void TextureLoader::clear()
{
    cache_.clear();
}
} // namespace nre
