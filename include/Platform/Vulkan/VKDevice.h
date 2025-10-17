#pragma once

namespace nre
{
class VKDevice
{
public:
    VKDevice();
    ~VKDevice();

    VKDevice(const VKDevice&) = delete;
    VKDevice& operator=(const VKDevice&) = delete;
    VKDevice(VKDevice&&) noexcept = delete;
    VKDevice& operator=(VKDevice&&) noexcept = delete;

    void initialize();
    void shutdown();
};
} // namespace nre
