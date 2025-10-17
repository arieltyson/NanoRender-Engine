#pragma once

namespace nre
{
struct Quaternion
{
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
    float w = 1.0F;

    constexpr Quaternion() = default;
    constexpr Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};
} // namespace nre
