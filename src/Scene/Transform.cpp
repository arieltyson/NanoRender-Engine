#include "Scene/Transform.h"

#include <cmath>

namespace nre
{
Transform::Transform() : position_(0.0F, 0.0F, 0.0F), rotation_(0.0F, 0.0F, 0.0F, 1.0F), scale_(1.0F, 1.0F, 1.0F) {}

void Transform::setPosition(const Vector3& position)
{
    position_ = position;
}

void Transform::setRotation(const Quaternion& rotation)
{
    rotation_ = rotation;
}

void Transform::setScale(const Vector3& scale)
{
    scale_ = scale;
}

Matrix4 Transform::localMatrix() const
{
    // Placeholder implementation returns identity until math library is fleshed out.
    (void)position_;
    (void)rotation_;
    (void)scale_;
    return Matrix4::identity();
}
} // namespace nre
