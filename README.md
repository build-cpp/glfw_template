# glfw_template

This is a template showcasing [cmkr](https://github.com/MoAlyousef/cmkr) together with [vcpkg](https://github.com/microsoft/vcpkg) for frictionless cross platform dependency management with CMake.

## Building (IDE)

Clone this repository and open it in your favorite IDE with CMake support (Visual Studio, CLion, Qt Creator). Everything should work out of the box.

## Building (command line)

```
cmake -Bbuild
```

Then open the `.sln` (Windows) or run `make` (Unix) from the `build` directory.

## cmake.toml

Under the hood cmkr generates the `CMakeLists.txt` required to build this project from the `cmake.toml` file:

```toml
[cmake]
version = "3.15"
cmkr-include = "cmake/cmkr.cmake"

[project]
name = "vcpkg_template"

# See https://github.com/microsoft/vcpkg#getting-started on how to use vcpkg
# Chose a version from https://github.com/microsoft/vcpkg/releases
# During CMake configuration you will be told how to find and link to the packages
[vcpkg]
version = "2020.11"
packages = ["glfw3", "GLEW"]

# vcpkg will download the packages, but you still need to find them to use them
[find-package]
glfw3 = {}
GLEW = {}

[target.example]
type = "executable"
sources = ["src/example.cpp"]
link-libraries = ["glfw", "GLEW::GLEW"]
```
