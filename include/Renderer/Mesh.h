#pragma once

#include <cstdint>
#include <vector>

namespace nre
{
struct Vertex
{
    float position[3];
    float normal[3];
    float uv[2];
};

class Mesh
{
public:
    virtual ~Mesh();

    virtual void upload(const std::vector<Vertex>& vertices,
                        const std::vector<std::uint32_t>& indices) = 0;
    virtual void draw() const = 0;
};
} // namespace nre
