# Installation

To get Zynk up and running, you have two main options: downloading the precompiled interpreter or building from source. Follow the instructions below based on your preference.

## 1. Download Precompiled Interpreter (Recommended)

1. Go to the [Releases](https://github.com/xXenvy/Zynk/releases) page on GitHub.
2. Download the latest release of interpreter suitable for your operating system.
3. Extract the interpreter to your desired location.

## 2. Building from Source

If you prefer to build Zynk from source, follow these steps:

### Prerequisites

Before building Zynk, make sure you have the following software installed:

- **CMake:** A cross-platform build system generator. You can download it from the [CMake website](https://cmake.org/download/).
- **A C++ Compiler:** Such as GCC, Clang, or MSVC. Ensure that the compiler is compatible with C++17 or later.
- **Git:** For cloning the repository. You can download it from the [Git website](https://git-scm.com/downloads).

### Build Instructions

1. **Clone the repository:**

    To clone a specific version, replace `TAG_NAME` with the desired tag. You can find available tags on the [Tags page](https://github.com/xXenvy/Zynk/tags) on GitHub.

        git clone --branch TAG_NAME --single-branch https://github.com/xXenvy/Zynk.git
    
    Example for cloning the `v1.0.0` tag:

        git clone --branch v1.0.0 --single-branch https://github.com/xXenvy/Zynk.git

2. **Navigate to the project directory:**

    ```
    cd Zynk
    ```

3. **Create a directory for temporary files (commonly named `build`):**

    ```
    mkdir build
    ```

4. **Change to the build directory:**

    ```
    cd build
    ```

5. **Configure CMake and build the project:**

    First, run:

    ```
    cmake ..
    ```

    Then build the project:

    ``` 
    cmake --build .
    ```

6. **Once the compilation is complete, you can run the Zynk interpreter from the `src` directory:**

    ```
    ./Zynk
    ```