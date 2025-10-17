#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Math/Vector3.h"

namespace nre
{
struct BoundingBox
{
    Vector3 min;
    Vector3 max;
};

class OctreeNode
{
public:
    explicit OctreeNode(const BoundingBox& bounds);

    bool isLeaf() const noexcept;
    void subdivide();

    const BoundingBox& bounds() const noexcept { return bounds_; }
    std::vector<int>& objects() noexcept { return objects_; }
    const std::vector<int>& objects() const noexcept { return objects_; }
    std::array<std::unique_ptr<OctreeNode>, 8>& children() noexcept { return children_; }

private:
    BoundingBox bounds_;
    std::vector<int> objects_;
    std::array<std::unique_ptr<OctreeNode>, 8> children_{};
};

class Octree
{
public:
    Octree(const BoundingBox& bounds, std::size_t maxDepth);

    OctreeNode& root() noexcept { return *root_; }
    const OctreeNode& root() const noexcept { return *root_; }

private:
    std::unique_ptr<OctreeNode> root_;
    std::size_t maxDepth_;
};
} // namespace nre
