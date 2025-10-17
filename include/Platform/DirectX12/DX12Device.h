#pragma once

namespace nre
{
class DX12Device
{
public:
    DX12Device();
    ~DX12Device();

    DX12Device(const DX12Device&) = delete;
    DX12Device& operator=(const DX12Device&) = delete;
    DX12Device(DX12Device&&) noexcept = delete;
    DX12Device& operator=(DX12Device&&) noexcept = delete;

    void initialize();
    void shutdown();
};
} // namespace nre
