#pragma once

#include <chrono>

namespace nre
{
class Timer
{
public:
    using clock = std::chrono::steady_clock;

    Timer();

    double deltaSeconds() const noexcept;
    double elapsedSeconds() const noexcept;

    void tick();
    void reset();

private:
    clock::time_point start_;
    clock::time_point last_;
    double deltaSeconds_ = 0.0;
};
} // namespace nre
