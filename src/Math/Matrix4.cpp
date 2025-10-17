#include "Math/Matrix4.h"

#include <cmath>

namespace nre
{
Matrix4 Matrix4::identity()
{
    return {};
}

Matrix4 Matrix4::perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane)
{
    Matrix4 result{};
    const float tanHalfFov = std::tan(fovRadians * 0.5F);

    result.data[0] = 1.0F / (aspectRatio * tanHalfFov);
    result.data[5] = 1.0F / tanHalfFov;
    result.data[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result.data[11] = -1.0F;
    result.data[14] = -(2.0F * farPlane * nearPlane) / (farPlane - nearPlane);
    result.data[15] = 0.0F;

    return result;
}
} // namespace nre
