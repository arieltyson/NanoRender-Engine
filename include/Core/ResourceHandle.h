#pragma once

#include <cstddef>
#include <cstdint>

namespace nre
{
struct ResourceHandle
{
    std::uint64_t id = 0;

    constexpr bool operator==(const ResourceHandle& other) const noexcept
    {
        return id == other.id;
    }

    constexpr bool operator!=(const ResourceHandle& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr explicit operator bool() const noexcept { return id != 0; }
};

} // namespace nre
