#pragma once

namespace nre
{
class MetalDevice
{
public:
    MetalDevice();
    ~MetalDevice();

    MetalDevice(const MetalDevice&) = delete;
    MetalDevice& operator=(const MetalDevice&) = delete;
    MetalDevice(MetalDevice&&) noexcept = delete;
    MetalDevice& operator=(MetalDevice&&) noexcept = delete;

    void initialize();
    void shutdown();
};
} // namespace nre
