#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace nre
{
class Texture;
class RenderAPI;

class TextureLoader
{
public:
    explicit TextureLoader(RenderAPI& api);

    std::shared_ptr<Texture> load(const std::string& path);
    void clear();

private:
    RenderAPI* api_ = nullptr;
    std::unordered_map<std::string, std::weak_ptr<Texture>> cache_;
};
} // namespace nre
