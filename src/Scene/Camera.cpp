#include "Scene/Camera.h"

namespace
{
constexpr float kDegToRad = 3.14159265358979323846F / 180.0F;
}

namespace nre
{
Camera::Camera()
{
    setPerspective(60.0F, 16.0F / 9.0F, 0.1F, 1000.0F);
    lookAt({0.0F, 0.0F, 5.0F}, {0.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F});
}

void Camera::setPerspective(float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
    const float radians = verticalFovDegrees * kDegToRad;
    projection_ = Matrix4::perspective(radians, aspectRatio, nearPlane, farPlane);
}

void Camera::setView(const Matrix4& viewMatrix)
{
    view_ = viewMatrix;
}

void Camera::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    position_ = eye;
    target_ = target;
    up_ = up;
    view_ = Matrix4::lookAt(eye, target, up);
}
} // namespace nre
