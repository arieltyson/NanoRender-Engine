#include "Scene/SceneGraph.h"

#include "Scene/Transform.h"

namespace nre
{
SceneNode::SceneNode() : transform_(std::make_unique<Transform>()) {}

SceneNode::~SceneNode() = default;

SceneNode& SceneNode::addChild()
{
    children_.push_back(std::make_unique<SceneNode>());
    return *children_.back();
}

const std::vector<std::unique_ptr<SceneNode>>& SceneNode::children() const noexcept
{
    return children_;
}

Transform& SceneNode::transform() noexcept
{
    return *transform_;
}

const Transform& SceneNode::transform() const noexcept
{
    return *transform_;
}

SceneGraph::SceneGraph() : root_(std::make_unique<SceneNode>()) {}
} // namespace nre
