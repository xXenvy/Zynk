# Zynk - Touring-complete, interpreted programming language

![commits](https://img.shields.io/github/commit-activity/w/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
![license](https://img.shields.io/github/license/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
![release](https://img.shields.io/github/v/release/xXenvy/Zynk?include_prereleases&style=for-the-badge&color=%2315b328)

## `📃` Introduction
Zynk is a Touring-complete, interpreted programming language written in C++. This project was created as a personal endeavor to learn and practice C++. It is not intended for extensive practical use but rather serves as a learning platform.

## `🌟` Features
- **Interpreted Language:** Zynk is an interpreted language, which facilitates quick development and testing cycles.
- **Touring-Complete:** Zynk is capable of expressing any computable function, adhering to the touring-complete classification.
- **C++ Backend:** Written in C++, Zynk leverages the performance and capabilities of C++ while providing a user-friendly interface.

## `🔨` Installation
### 1. Download Precompiled Interpreter (Recommended)
1. Go to the [Releases](https://github.com/xXenvy/Zynk/releases) page.
2. Download the latest release package for your operating system.
3. Extract the package to your desired location.

### 2. Build from Source
1. Clone the repository:
```shell
git clone https://github.com/xXenvy/Zynk.git
```

2. Navigate to the project directory:
```shell
cd Zynk
```

3. Create a directory for temporary files (commonly named `build`):
```shell
mkdir build
cd build
```

4. Configure CMake and build the project.
> Command to build project depends on your system.
```shell
cmake ..
cmake --build .
```

5. Once the compilation is complete, you can run the Zynk interpreter from the `src/Debug` directory:
```shell
./Zynk
 ```
