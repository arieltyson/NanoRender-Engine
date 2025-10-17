#pragma once

#include <array>
#include <string>

namespace nre
{
struct PBRMaterialProperties
{
    std::array<float, 3> albedo{1.0F, 1.0F, 1.0F};
    float metallic = 0.0F;
    float roughness = 1.0F;
    float ao = 1.0F;
};

class Material
{
public:
    virtual ~Material();

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual const PBRMaterialProperties& properties() const noexcept = 0;
};
} // namespace nre
