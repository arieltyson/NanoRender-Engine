#include "Renderer/RenderAPI.h"

#include <stdexcept>

#include "Renderer/CommandBuffer.h"

#if defined(NRE_ENABLE_OPENGL)
#include "Platform/OpenGL/GLRenderAPI.h"
#endif

#if defined(NRE_ENABLE_VULKAN)
#include "Platform/Vulkan/VKRenderAPI.h"
#endif

#if defined(NRE_ENABLE_DIRECTX12)
#include "Platform/DirectX12/DX12RenderAPI.h"
#endif

#if defined(NRE_ENABLE_METAL)
#include "Platform/Metal/MetalRenderAPI.h"
#endif

namespace nre
{
std::unique_ptr<RenderAPI> RenderAPI::create(APIType api)
{
    switch (api)
    {
    case APIType::OpenGL:
#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)
        return std::make_unique<GLRenderAPI>();
#elif defined(NRE_ENABLE_OPENGL)
        throw std::runtime_error("OpenGL backend requires GLFW support but it was not detected.");
#else
        throw std::runtime_error("OpenGL backend is disabled in this build.");
#endif
    case APIType::Vulkan:
#if defined(NRE_ENABLE_VULKAN)
        return std::make_unique<VKRenderAPI>();
#else
        throw std::runtime_error("Vulkan backend is disabled in this build.");
#endif
    case APIType::DirectX12:
#if defined(NRE_ENABLE_DIRECTX12)
        return std::make_unique<DX12RenderAPI>();
#else
        throw std::runtime_error("DirectX 12 backend is disabled in this build.");
#endif
    case APIType::Metal:
#if defined(NRE_ENABLE_METAL)
        return std::make_unique<MetalRenderAPI>();
#else
        throw std::runtime_error("Metal backend is disabled in this build.");
#endif
    default:
        break;
    }

    throw std::runtime_error("Unsupported rendering API requested.");
}
} // namespace nre
