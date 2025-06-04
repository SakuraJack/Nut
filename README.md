# Nut

Nut is a **render engine** written in modern C++, designed with cross-platform support in mind.

## Purpose

The goal of Nut is to try building a game engine from scratch.

> **Note:**  
> It currently supports **Windows** only, and utilizes **OpenGL** as its graphics API.  
> The engine may have performance issues, bugs, or memory leaks.  
> I will continue to improve it as part of my learning process.  
> Future plans include adding support for **Linux** and **macOS**, as well as integrating the **Vulkan** graphics API.

## Features

- Modern C++ design
- OpenGL rendering (via GLAD)
- Window management and event handling (via GLFW)
- Logging (via spdlog)
- Immediate-mode GUI (via ImGui)
- Math utilities (via glm)
- Model loading (via Assimp)
- Entity-Component-System (ECS) architecture (via EnTT)
- (Planned) Vulkan API support
- (Planned) Cross-platform support (Linux, macOS)
- Extensible and modular design

## Core Dependencies

- [GLFW](https://www.glfw.org/) – Window creation and event handling
- [GLAD](https://glad.dav1d.de/) – OpenGL function loader
- [spdlog](https://github.com/gabime/spdlog) – Logging system
- [ImGui](https://github.com/ocornut/imgui) – GUI
- [glm](https://github.com/g-truc/glm) – Mathematics library
- [Assimp](https://github.com/assimp/assimp) – Model loading
- [EnTT](https://github.com/skypjack/entt) – Entity-Component-System (ECS) framework

## Getting Started

### Prerequisites

- Windows OS
- [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) (required)
- C++17 compatible compiler (provided with VS2022)
- [Premake5](https://premake.github.io/) (for generating project files, will be downloaded via setup script if missing)

### Setup & Building

1. **Clone the repository:**
    ```bash
    git clone https://github.com/SakuraJack/Nut.git
    cd Nut
    ```

2. **Run the setup script to fetch dependencies and generate project files:**
    ```bash
    cd scripts
    Setup.bat
    ```
    This script will download all necessary dependencies and generate Visual Studio 2022 project files for you.

3. **Open the generated solution in Visual Studio 2022.**

### Usage

Run the solution in Visual Studio 2022 and press **"Run"** (or F5) to launch the engine.

## Author

- [SakuraJack](https://github.com/SakuraJack)
