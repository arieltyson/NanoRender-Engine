#include "Renderer/TextureLoader.h"

#include <cstdint>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Renderer/RenderAPI.h"
#include "Renderer/Texture.h"

namespace nre
{
namespace
{
void skipComments(std::istream& stream)
{
    stream >> std::ws;
    while (stream.peek() == '#')
    {
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        stream >> std::ws;
    }
}
} // namespace

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

    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open texture file: " + path);
    }

    std::string magic;
    file >> magic;
    if (magic != "P6")
    {
        throw std::runtime_error("Unsupported texture format (expected binary PPM P6): " + path);
    }

    skipComments(file);
    int width = 0;
    int height = 0;
    file >> width >> height;
    skipComments(file);
    int maxValue = 0;
    file >> maxValue;
    file.get(); // consume single whitespace after header

    if (width <= 0 || height <= 0 || maxValue != 255)
    {
        throw std::runtime_error("Invalid PPM header in texture: " + path);
    }

    const std::size_t pixelCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    std::vector<unsigned char> rgb(pixelCount * 3);
    file.read(reinterpret_cast<char*>(rgb.data()), static_cast<std::streamsize>(rgb.size()));
    if (!file)
    {
        throw std::runtime_error("Failed to read pixel data from texture: " + path);
    }

    std::vector<unsigned char> rgba(pixelCount * 4);
    for (std::size_t i = 0; i < pixelCount; ++i)
    {
        rgba[i * 4 + 0] = rgb[i * 3 + 0];
        rgba[i * 4 + 1] = rgb[i * 3 + 1];
        rgba[i * 4 + 2] = rgb[i * 3 + 2];
        rgba[i * 4 + 3] = 255;
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
