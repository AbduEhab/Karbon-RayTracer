# Overview

This Raytracer is built to demonstrate the renderer built in the "Ray Tracing in One Weekend" book series. This is a Ray-Tracer built on the same architecture used in my [Coal-RayTracer](https://github.com/AbduEhab/Coal-RayTracer).

Fully supports Windows (full UI and stuff) - NOW SUPPORTS GCC (tested on windows only). Linux works in `Legacy mode` which would output the rendered image onto the terminal, macOS was not tested.

## Requirements (for compiling)
- [CMake](https://cmake.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)
- x64 Compiler (DOES NOT WORK WITH x86 COMPILERS!!!)

## Getting Started
Once you've cloned the repo, setup cmake with `cmake -S . -B build`. The binaries should be automatically copied in to the bin folder (no mater the build config or platform). 

### 3rd party libaries
- [Walnut](https://github.com/TheCherno/Walnut)
- [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [L2DFileDialog](https://github.com/Limeoats/L2DFileDialog)

### Demo Renders
![Demo 1](https://github.com/AbduEhab/Karbon-RayTracer/blob/main/Renders/render.jpg)
