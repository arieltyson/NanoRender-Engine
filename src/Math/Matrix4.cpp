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

Matrix4 Matrix4::translation(const Vector3& translation)
{
    Matrix4 result = identity();
    result.data[12] = translation.x;
    result.data[13] = translation.y;
    result.data[14] = translation.z;
    return result;
}

Matrix4 Matrix4::scale(const Vector3& scale)
{
    Matrix4 result = identity();
    result.data[0] = scale.x;
    result.data[5] = scale.y;
    result.data[10] = scale.z;
    return result;
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    const Vector3 f = (target - eye).normalized();
    const Vector3 s = Vector3::cross(f, up.normalized()).normalized();
    const Vector3 u = Vector3::cross(s, f);

    Matrix4 result = identity();
    result.data[0] = s.x;
    result.data[4] = s.y;
    result.data[8] = s.z;

    result.data[1] = u.x;
    result.data[5] = u.y;
    result.data[9] = u.z;

    result.data[2] = -f.x;
    result.data[6] = -f.y;
    result.data[10] = -f.z;

    result.data[12] = -Vector3::dot(s, eye);
    result.data[13] = -Vector3::dot(u, eye);
    result.data[14] = Vector3::dot(f, eye);

    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const noexcept
{
    Matrix4 result{};
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            float value = 0.0F;
            for (int inner = 0; inner < 4; ++inner)
            {
                value += at(row, inner) * rhs.at(inner, column);
            }
            result.at(row, column) = value;
        }
    }
    return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& rhs) noexcept
{
    *this = *this * rhs;
    return *this;
}

float& Matrix4::at(int row, int column) noexcept
{
    return data[static_cast<std::size_t>(column * 4 + row)];
}

float Matrix4::at(int row, int column) const noexcept
{
    return data[static_cast<std::size_t>(column * 4 + row)];
}
} // namespace nre
