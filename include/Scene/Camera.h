#pragma once

#include "Math/Matrix4.h"

namespace nre
{
class Camera
{
public:
    Camera();

    void setPerspective(float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane);
    void setView(const Matrix4& view);

    const Matrix4& view() const noexcept { return view_; }
    const Matrix4& projection() const noexcept { return projection_; }

private:
    Matrix4 view_;
    Matrix4 projection_;
};
} // namespace nre
