#include "Scene/Octree.h"

namespace nre
{
OctreeNode::OctreeNode(const BoundingBox& bounds) : bounds_(bounds) {}

bool OctreeNode::isLeaf() const noexcept
{
    for (const auto& child : children_)
    {
        if (child)
        {
            return false;
        }
    }
    return true;
}

void OctreeNode::subdivide()
{
    if (!isLeaf())
    {
        return;
    }

    for (auto& child : children_)
    {
        child = std::make_unique<OctreeNode>(bounds_);
    }
}

Octree::Octree(const BoundingBox& bounds, std::size_t maxDepth)
    : root_(std::make_unique<OctreeNode>(bounds)), maxDepth_(maxDepth)
{
    (void)maxDepth_;
}
} // namespace nre
