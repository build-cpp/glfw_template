# glfw_template

Simple template to get started with OpenGL using [GLFW](https://www.glfw.org/docs/latest/quick_guide.html) and [GLEW](https://github.com/nigels-com/glew).

This template uses [cmkr](https://github.com/build-cpp/cmkr) together with [vcpkg](https://github.com/microsoft/vcpkg) for frictionless cross platform dependency management with CMake.

## Building (IDE)

Clone this repository and open it in your favorite IDE with CMake support ([Visual Studio](https://visualstudio.microsoft.com/), [CLion](https://www.jetbrains.com/clion/), [Qt Creator](https://www.qt.io/product/development-tools)). Everything should work out of the box.

## Building (command line)

```sh
cmake -B build
cmake --build build
```

**Note:** On Ubuntu you need to run `sudo apt-get install libgl-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev` to use this template.

## cmake.toml

Under the hood cmkr generates the `CMakeLists.txt` required to build this project from the `cmake.toml` file:

```toml
[cmake]
version = "3.15"
cmkr-include = "cmake/cmkr.cmake"

[project]
name = "glfw_template"
include-after = [
    "cmake/generate_shaders.cmake"
]

# See https://vcpkg.io/en/packages.html for available packages
# Chose a version from https://github.com/microsoft/vcpkg/releases
[vcpkg]
version = "2021.05.12"
packages = ["glfw3", "glew"]

# vcpkg will download the packages, but you still need to find them to use them
[find-package]
glfw3 = {}
GLEW = {}

[target.example]
type = "executable"
sources = [
    "src/example.cpp",
    "shaders/FragmentShader.glsl",
    "shaders/VertexShader.glsl",
]
link-libraries = ["glfw", "GLEW::GLEW"]
compile-features = ["cxx_std_11"]
cmake-after = """
generate_shaders(${CMKR_TARGET})
"""
```
