#include "Scene/Transform.h"

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
    const float sx = scale_.x;
    const float sy = scale_.y;
    const float sz = scale_.z;

    const float x = rotation_.x;
    const float y = rotation_.y;
    const float z = rotation_.z;
    const float w = rotation_.w;

    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;
    const float xy = x * y;
    const float xz = x * z;
    const float yz = y * z;
    const float wx = w * x;
    const float wy = w * y;
    const float wz = w * z;

    Matrix4 result = Matrix4::identity();
    result.data[0] = (1.0F - 2.0F * (yy + zz)) * sx;
    result.data[1] = (2.0F * (xy + wz)) * sx;
    result.data[2] = (2.0F * (xz - wy)) * sx;

    result.data[4] = (2.0F * (xy - wz)) * sy;
    result.data[5] = (1.0F - 2.0F * (xx + zz)) * sy;
    result.data[6] = (2.0F * (yz + wx)) * sy;

    result.data[8] = (2.0F * (xz + wy)) * sz;
    result.data[9] = (2.0F * (yz - wx)) * sz;
    result.data[10] = (1.0F - 2.0F * (xx + yy)) * sz;

    result.data[12] = position_.x;
    result.data[13] = position_.y;
    result.data[14] = position_.z;

    return result;
}
} // namespace nre
