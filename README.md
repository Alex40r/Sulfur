# Sulfur

[![CMake - Windows](https://github.com/Alex40r/Sulfur/actions/workflows/cmake-windows.yml/badge.svg)](https://github.com/Alex40r/Sulfur/actions/workflows/cmake-windows.yml)
[![CMake - Ubuntu](https://github.com/Alex40r/Sulfur/actions/workflows/cmake-ubuntu.yml/badge.svg)](https://github.com/Alex40r/Sulfur/actions/workflows/cmake-ubuntu.yml)

**An Efficient C++ Vulkan Graphics Engine**

*Sulfur* is a graphics engine written in C++ 14, and uses the Vulkan API.
It is designed to be fast, efficient, robust, and featureful.

Is it currently a **work-in-progress**, and is subject to major changes and frequent updates.

# Build

### Requirements

- CMake 3.8+
- Vulkan SDK
- C++ 14 compiler

### Building

```bash
git clone https://github.com/Alex40r/Sulfur.git --recursive

cd Sulfur

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release
```