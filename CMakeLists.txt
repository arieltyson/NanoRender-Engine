cmake_minimum_required(VERSION 3.24)

project(
    NanoRenderEngine
    VERSION 0.1.0
    DESCRIPTION "Cross-platform real-time rendering engine"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(StandardProjectSettings)

option(NRE_BUILD_SHARED "Build NanoRender Engine as a shared library" OFF)
option(NRE_BUILD_EXAMPLES "Build example applications" ON)
option(NRE_BUILD_TESTS "Build unit tests" OFF)

set(NRE_DEFAULT_ENABLE_OPENGL ON)
set(NRE_DEFAULT_ENABLE_VULKAN ON)
set(NRE_DEFAULT_ENABLE_DIRECTX12 OFF)
set(NRE_DEFAULT_ENABLE_METAL OFF)

if (WIN32)
    set(NRE_DEFAULT_ENABLE_DIRECTX12 ON)
endif()

if (APPLE)
    set(NRE_DEFAULT_ENABLE_METAL ON)
endif()

option(NRE_ENABLE_OPENGL "Enable OpenGL backend" ${NRE_DEFAULT_ENABLE_OPENGL})
option(NRE_ENABLE_VULKAN "Enable Vulkan backend" ${NRE_DEFAULT_ENABLE_VULKAN})
option(NRE_ENABLE_DIRECTX12 "Enable DirectX 12 backend" ${NRE_DEFAULT_ENABLE_DIRECTX12})
option(NRE_ENABLE_METAL "Enable Metal backend" ${NRE_DEFAULT_ENABLE_METAL})

set(NRE_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation include directory")
set(NRE_INSTALL_LIBDIR "lib" CACHE STRING "Installation library directory")

add_subdirectory(src)

if (NRE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (NRE_BUILD_TESTS)
    enable_testing()
    message(STATUS "Test targets can be added in future phases.")
endif()
