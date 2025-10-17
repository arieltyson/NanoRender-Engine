#pragma once

#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace nre
{
class Transform
{
public:
    Transform();

    void setPosition(const Vector3& position);
    void setRotation(const Quaternion& rotation);
    void setScale(const Vector3& scale);

    const Vector3& position() const noexcept { return position_; }
    const Quaternion& rotation() const noexcept { return rotation_; }
    const Vector3& scale() const noexcept { return scale_; }

    Matrix4 localMatrix() const;

private:
    Vector3 position_;
    Quaternion rotation_;
    Vector3 scale_;
};
} // namespace nre
