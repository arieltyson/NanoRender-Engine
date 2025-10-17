#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

namespace nre
{
class Camera
{
public:
    Camera();

    void setPerspective(float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane);
    void setView(const Matrix4& view);
    void lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

    const Matrix4& view() const noexcept { return view_; }
    const Matrix4& projection() const noexcept { return projection_; }
    Matrix4 viewProjection() const noexcept { return projection_ * view_; }

private:
    Matrix4 view_;
    Matrix4 projection_;
    Vector3 position_;
    Vector3 target_;
    Vector3 up_{0.0F, 1.0F, 0.0F};
};
} // namespace nre
