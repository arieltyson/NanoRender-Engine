#pragma once

namespace nre
{
class GLContext
{
public:
    GLContext();
    ~GLContext();

    GLContext(const GLContext&) = delete;
    GLContext& operator=(const GLContext&) = delete;
    GLContext(GLContext&&) noexcept = delete;
    GLContext& operator=(GLContext&&) noexcept = delete;

    void initialize();
    void shutdown();
};
} // namespace nre
