#pragma once

namespace nre
{
class CommandBuffer
{
public:
    virtual ~CommandBuffer();

    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void submit() = 0;
};
} // namespace nre
