# RRenderer

Is a lightweight and modular Renderer built from scratch. It's designed as a learning project,
focusing on clear abstractions over explicit Graphics APIs. The only implementation is Vulkan
for now.

## Building

### Prerequisites

- C++23 compatible compiler
- CMake $\geq$ 3.30
- VulkanSDK $\geq$ 1.3
- git

### Build instructions

#### Clone the Repository

```bash
git clone https://github.com/FelixHommel/RRenderer.git
cd RRenderer
```

#### Configure CMake and Build the Program

> If you want to use debug builds instead replace ```release``` with ```debug```.
> You also can also force use clang or gcc with ```release-clang```, ```debug-clang```, ```release-gcc``` or ```debug-gcc```.

```bash
cmake --preset release
cmake --build --preset release
```

#### Run the executable

```bash
./build/release/src/app/App
```

## Dependencies

> All dependencies (except VulkanSDK) are pulled in from CMake, so there is no need to install
> them manually on the system.

- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [googletest](https://github.com/google/googletest)

## Acknowledgements / Credits

- [Brendan Galea's](https://www.youtube.com/c/BrendanGalea) Vulkan (c++) Game Engine Tutorials
