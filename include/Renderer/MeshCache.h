#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Renderer/Mesh.h"
#include "Renderer/MeshFactory.h"

namespace nre
{
class RenderAPI;

class MeshCache
{
public:
    explicit MeshCache(RenderAPI& api);

    std::shared_ptr<Mesh> loadFromFile(const std::string& path);

    std::shared_ptr<Mesh> loadFromGenerator(const std::string& key,
                                            const std::function<MeshData()>& generator);

    void clear();

private:
    std::shared_ptr<Mesh> createMesh(const MeshData& data);

    RenderAPI* api_ = nullptr;
    std::unordered_map<std::string, std::weak_ptr<Mesh>> cache_;
};
} // namespace nre
