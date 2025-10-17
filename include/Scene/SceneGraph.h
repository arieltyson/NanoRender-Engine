#pragma once

#include <memory>
#include <vector>

namespace nre
{
class Transform;

class SceneNode
{
public:
    SceneNode();
    ~SceneNode();

    SceneNode(const SceneNode&) = delete;
    SceneNode& operator=(const SceneNode&) = delete;
    SceneNode(SceneNode&&) noexcept = default;
    SceneNode& operator=(SceneNode&&) noexcept = default;

    SceneNode& addChild();
    const std::vector<std::unique_ptr<SceneNode>>& children() const noexcept;
    Transform& transform() noexcept;
    const Transform& transform() const noexcept;

private:
    std::vector<std::unique_ptr<SceneNode>> children_;
    std::unique_ptr<Transform> transform_;
};

class SceneGraph
{
public:
    SceneGraph();

    SceneNode& root() noexcept { return *root_; }
    const SceneNode& root() const noexcept { return *root_; }

private:
    std::unique_ptr<SceneNode> root_;
};
} // namespace nre
