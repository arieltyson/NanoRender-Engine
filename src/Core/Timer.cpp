#include "Core/Timer.h"

namespace nre
{
Timer::Timer()
{
    reset();
}

double Timer::deltaSeconds() const noexcept
{
    return deltaSeconds_;
}

double Timer::elapsedSeconds() const noexcept
{
    using namespace std::chrono;
    return duration_cast<duration<double>>(clock::now() - start_).count();
}

void Timer::tick()
{
    const auto now = clock::now();
    const auto frameDuration = now - last_;
    last_ = now;
    using namespace std::chrono;
    deltaSeconds_ = duration_cast<duration<double>>(frameDuration).count();
}

void Timer::reset()
{
    start_ = clock::now();
    last_ = start_;
    deltaSeconds_ = 0.0;
}
} // namespace nre
