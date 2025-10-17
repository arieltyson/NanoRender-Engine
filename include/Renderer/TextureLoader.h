#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace nre
{
class Texture;

class TextureLoader
{
public:
    std::shared_ptr<Texture> load(const std::string& path);
    void clear();

private:
    std::unordered_map<std::string, std::weak_ptr<Texture>> cache_;
};
} // namespace nre
