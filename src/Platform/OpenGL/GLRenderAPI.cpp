#include "Platform/OpenGL/GLRenderAPI.h"

#include "Platform/OpenGL/GLContext.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

namespace nre
{
GLRenderAPI::GLRenderAPI() = default;

GLRenderAPI::~GLRenderAPI()
{
    shutdown();
}

void GLRenderAPI::initialize()
{
    if (context_ == nullptr)
    {
        context_ = new GLContext();
        context_->initialize();
    }
}

void GLRenderAPI::shutdown()
{
    if (context_ != nullptr)
    {
        context_->shutdown();
        delete context_;
        context_ = nullptr;
    }
}

void GLRenderAPI::beginFrame()
{
    const int width = viewportWidth_ > 0 ? viewportWidth_ : 1;
    const int height = viewportHeight_ > 0 ? viewportHeight_ : 1;
    glViewport(0, 0, width, height);
    glClearColor(clearColor_[0], clearColor_[1], clearColor_[2], clearColor_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderAPI::endFrame() {}

void GLRenderAPI::setViewport(int width, int height)
{
    viewportWidth_ = width > 0 ? width : 1;
    viewportHeight_ = height > 0 ? height : 1;
}

void GLRenderAPI::setClearColor(float r, float g, float b, float a)
{
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
}

RenderCapabilities GLRenderAPI::capabilities() const noexcept
{
    RenderCapabilities caps{};
    caps.shaderFloat64 = true;
    return caps;
}
} // namespace nre
