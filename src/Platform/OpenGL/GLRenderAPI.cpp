#include "Platform/OpenGL/GLRenderAPI.h"

#include "Platform/OpenGL/GLContext.h"

namespace nre
{
GLRenderAPI::GLRenderAPI() = default;

GLRenderAPI::~GLRenderAPI() = default;

void GLRenderAPI::initialize()
{
    if (!context_)
    {
        context_ = new GLContext();
        context_->initialize();
    }
}

void GLRenderAPI::shutdown()
{
    if (context_)
    {
        context_->shutdown();
        delete context_;
        context_ = nullptr;
    }
}

void GLRenderAPI::beginFrame() {}

void GLRenderAPI::endFrame() {}

RenderCapabilities GLRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.shaderFloat64 = true;
    return caps;
}
} // namespace nre
