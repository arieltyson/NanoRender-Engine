#pragma once

#include <array>

namespace nre
{
struct Matrix4
{
    std::array<float, 16> data{1.0F, 0.0F, 0.0F, 0.0F,
                               0.0F, 1.0F, 0.0F, 0.0F,
                               0.0F, 0.0F, 1.0F, 0.0F,
                               0.0F, 0.0F, 0.0F, 1.0F};

    static Matrix4 identity();
    static Matrix4 perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane);
};
} // namespace nre
