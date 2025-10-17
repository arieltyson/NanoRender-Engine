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

set(NRE_GLFW_TARGET "")
set(NRE_HAVE_GLFW OFF)

if (NRE_ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)

    find_package(glfw3 CONFIG QUIET)
    if (glfw3_FOUND)
        if (TARGET glfw)
            set(NRE_GLFW_TARGET glfw)
        elseif (TARGET glfw3)
            set(NRE_GLFW_TARGET glfw3)
        endif()
    endif()

    if (NRE_GLFW_TARGET STREQUAL "")
        find_package(glfw3 3.3 QUIET)
        if (glfw3_FOUND)
            if (TARGET glfw)
                set(NRE_GLFW_TARGET glfw)
            elseif (TARGET glfw3)
                set(NRE_GLFW_TARGET glfw3)
            endif()
        endif()
    endif()

    if (NRE_GLFW_TARGET STREQUAL "")
        find_package(GLFW3 QUIET)
        if (GLFW3_FOUND)
            add_library(NRE::glfw INTERFACE)
            target_include_directories(NRE::glfw INTERFACE ${GLFW3_INCLUDE_DIR})
            target_link_libraries(NRE::glfw INTERFACE ${GLFW3_LIBRARIES})
            set(NRE_GLFW_TARGET NRE::glfw)
        endif()
    endif()

    if (NOT NRE_GLFW_TARGET STREQUAL "")
        set(NRE_HAVE_GLFW ON)
    else()
        message(WARNING "GLFW was not found; window creation will fall back to a stub implementation.")
    endif()
endif()

add_subdirectory(src)

if (NRE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (NRE_BUILD_TESTS)
    enable_testing()
    message(STATUS "Test targets can be added in future phases.")
endif()
