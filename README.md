# NanoRender Engine 🎮🔥

# A cross-platform real-time graphics pipeline built with modern C++17

⚙️ Engineered for multi-API rendering across DirectX 12, Vulkan, OpenGL, and Metal

# Rendering Immersive Experiences at Scale 🌐

## Project Description 🗺️

NanoRender Engine demonstrates unified graphics programming concepts through a single C++ interface that targets DirectX 12, Vulkan, Metal, and OpenGL. The project explores how AAA-quality rendering techniques can be organized, optimized, and delivered across desktop and mobile platforms with a consistent architecture.

## Demo:

Coming soon.

## Quickstart 🧭

The snippet below builds the engine and runs the sample application. Copy it into a terminal from the repository root.

```bash
cmake -S . -B build                                # Configure the project
cmake --build build                                # Compile library + example
./build/examples/nanorender-example                # Launch the sample app
```

### Expected Output

During `cmake --build build` you should see compilation progress culminating in:

```
[100%] Linking CXX executable nanorender-example
[100%] Built target nanorender_example
```

Running `./build/examples/nanorender-example`:

- When GLFW is installed → an 800×600 OpenGL window opens with a cleared background color.
- Without GLFW → console prints `Failed to initialize rendering backend: OpenGL backend requires GLFW support but it was not detected.` and the app exits cleanly.
- Use `W/A/S/D` to move, `Q/E` to move vertically while the camera orbits the triangle.

### Prerequisites 📋

- CMake 3.22+
- C++17-compliant compiler (MSVC 2019+, Clang 12+, GCC 11+, Apple Clang)
- GLFW 3.3+
- Vulkan SDK (for Vulkan backend)
- DirectX 12 SDK (for Windows backend)
- Xcode with Metal toolchain (for macOS/iOS backend)
- Python 3 (optional, for tooling scripts)

### Frameworks

- C++17 STL
- CMake
- GLFW
- GLM
- DirectX 12
- Vulkan
- OpenGL 4.5+
- Metal

### Packages

- SPIRV-Reflect
- Dear ImGui
- JSON for Modern C++

## Skills Demonstrated 🖌️

This project applies systems-level rendering expertise to distill modern engine architecture into a portable educational codebase. NanoRender Engine highlights patterns and abstractions inspired by production-grade renderers from studios such as Roblox, Epic, and Unity.

- **Core Framework**: The `Application`, `Window`, and `Timer` layers coordinate platform services, frame pacing, and lifecycle management using RAII and smart pointers.
- **RenderAPI Abstraction**: A factory-driven interface instantiates DirectX 12, Vulkan, Metal, or OpenGL backends at runtime, decoupling higher-level systems from platform-specific GPU code.
- **Scene & Data Structures**: An octree-backed scene graph combines hierarchical transforms, cameras, and spatial partitioning to accelerate frustum culling for large object counts.
- **Physically Based Rendering**: Cook-Torrance BRDF materials blend metallic, roughness, and albedo inputs while leveraging SIMD-accelerated math for view and light calculations.
- **Async Shader Pipeline**: std::async-powered compilation composes GLSL, HLSL, and Metal Shading Language into SPIR-V bytecode with reflection data for descriptor binding.
- **Post-Processing & Debugging**: SSAO and bilateral denoising compute passes share GPU profiling markers with an ImGui overlay for quick iteration and tuning.

## Contributing ⚙️

We believe that great knowledge like that which you possess is meant to be shared, and we welcome contributions from fellow geniuses and developers alike. If you have a beneficial or creative code snippet to add or a design improvement to suggest, feel free to fork our repository, make your enhancements, and submit a pull request. Together, let's push real-time graphics forward.

## License 🪪

This project is licensed under the [MIT License](LICENSE), which means you're free to modify, distribute, and use the code as you see fit. Just remember to attribute the original work to **Ariel Tyson's GitHub** and keep the aspiring graphics engineer spirit alive!

---
