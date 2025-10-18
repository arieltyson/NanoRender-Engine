set(NRE_CORE_SOURCES
    Core/Application.cpp
    Core/Input.cpp
    Core/Timer.cpp
    Core/Window.cpp
    Core/ResourceRegistry.cpp
    Renderer/RenderAPI.cpp
    Renderer/Shader.cpp
    Renderer/Material.cpp
    Renderer/Mesh.cpp
    Renderer/Texture.cpp
    Renderer/CommandBuffer.cpp
    Renderer/MeshFactory.cpp
    Renderer/MeshCache.cpp
    Renderer/ShaderLoader.cpp
    Renderer/TextureLoader.cpp
    Renderer/RenderGraph.cpp
    ../external/imgui/imgui.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_impl_glfw.cpp
    ../external/imgui/imgui_impl_opengl3.cpp
    Scene/SceneGraph.cpp
    Scene/Camera.cpp
    Scene/Transform.cpp
    Scene/Octree.cpp
    Math/Vector3.cpp
    Math/Matrix4.cpp
    Math/SIMD_Math.cpp
)

set(NRE_LIBRARY_TYPE STATIC)
if (NRE_BUILD_SHARED)
    set(NRE_LIBRARY_TYPE SHARED)
endif()

add_library(nanorender ${NRE_LIBRARY_TYPE} ${NRE_CORE_SOURCES})

include(CompilerWarnings)
nre_enable_warnings(nanorender)

target_compile_features(nanorender PUBLIC cxx_std_17)

target_include_directories(
    nanorender
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:${NRE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../external
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/ThirdParty/imgui
)

target_sources(
    nanorender
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
        FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/Application.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/Input.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/ResourceHandle.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/ResourceRegistry.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/Timer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core/Window.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/RenderAPI.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/Shader.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/ShaderLoader.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/Material.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/Mesh.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/MeshCache.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/MeshFactory.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/RenderGraph.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/Texture.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/TextureLoader.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Renderer/CommandBuffer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Scene/SceneGraph.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Scene/Camera.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Scene/Transform.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Scene/Octree.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Math/Vector3.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Math/Matrix4.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Math/Quaternion.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Math/SIMD_Math.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/DirectX12/DX12RenderAPI.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/DirectX12/DX12Device.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/Vulkan/VKRenderAPI.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/Vulkan/VKDevice.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/OpenGL/GLRenderAPI.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/OpenGL/GLContext.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/OpenGL/GLMesh.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/OpenGL/GLShader.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/OpenGL/GLTexture.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/Metal/MetalRenderAPI.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/Platform/Metal/MetalDevice.h
)

if (NRE_ENABLE_OPENGL)
    target_sources(
        nanorender
        PRIVATE
            Platform/OpenGL/GLRenderAPI.cpp
            Platform/OpenGL/GLContext.cpp
            Platform/OpenGL/GLMesh.cpp
            Platform/OpenGL/GLShader.cpp
            Platform/OpenGL/GLTexture.cpp
    )
    target_compile_definitions(nanorender PUBLIC NRE_ENABLE_OPENGL)
    if (NOT NRE_GLFW_TARGET STREQUAL "")
        target_link_libraries(nanorender PUBLIC ${NRE_GLFW_TARGET})
        target_compile_definitions(nanorender PUBLIC NRE_USE_GLFW)
    endif()
    if (TARGET OpenGL::GL)
        target_link_libraries(nanorender PUBLIC OpenGL::GL)
    endif()
    if (APPLE)
        target_compile_definitions(nanorender PUBLIC GL_SILENCE_DEPRECATION)
    endif()
endif()

if (NRE_ENABLE_VULKAN)
    target_sources(
        nanorender
        PRIVATE
            Platform/Vulkan/VKRenderAPI.cpp
            Platform/Vulkan/VKDevice.cpp
    )
    target_compile_definitions(nanorender PUBLIC NRE_ENABLE_VULKAN)
endif()

if (NRE_ENABLE_DIRECTX12)
    if (WIN32)
        target_sources(
            nanorender
            PRIVATE
                Platform/DirectX12/DX12RenderAPI.cpp
                Platform/DirectX12/DX12Device.cpp
        )
        target_compile_definitions(nanorender PUBLIC NRE_ENABLE_DIRECTX12)
    else()
        message(WARNING "DirectX 12 backend requested but not available on this platform.")
    endif()
endif()

if (NRE_ENABLE_METAL)
    if (APPLE)
        target_sources(
            nanorender
            PRIVATE
                Platform/Metal/MetalRenderAPI.mm
                Platform/Metal/MetalDevice.mm
        )
        target_compile_definitions(nanorender PUBLIC NRE_ENABLE_METAL)
    else()
        message(WARNING "Metal backend requested but only available on Apple platforms.")
    endif()
endif()

if (UNIX AND NOT APPLE)
    target_link_libraries(nanorender PUBLIC pthread)
endif()


install(
    TARGETS nanorender
    EXPORT NanoRenderEngineTargets
    FILE_SET public_headers DESTINATION ${NRE_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION ${NRE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${NRE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin
)

install(
    EXPORT NanoRenderEngineTargets
    NAMESPACE nre::
    DESTINATION ${NRE_INSTALL_LIBDIR}/cmake/NanoRenderEngine
)
