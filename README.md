<div align="center">
  
  <img src="https://github.com/xXenvy/Zynk/assets/111158232/2f92acf0-409e-4e6d-b80f-2379b01d31f7" width="200" height="200">

  # Zynk - Touring-complete, interpreted programming language

  ![commits](https://img.shields.io/github/commit-activity/w/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
  ![license](https://img.shields.io/github/license/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
  ![tests](https://img.shields.io/github/check-runs/xXenvy/Zynk/master?style=for-the-badge&label=Tests&color=%2315b328)
</div>
<!--- ![release](https://img.shields.io/github/v/release/xXenvy/Zynk?include_prereleases&style=for-the-badge&color=%2315b328) -->

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
cmake --build . --config Release
```

5. Once the compilation is complete, you can run the Zynk interpreter from the `src` directory:
```shell
./Zynk
 ```

## `🚀` Road to 1.0
> Keep in mind that **Zynk** is a very young project!

As Zynk progresses towards version 1.0, the following components are planned and required for the interpreter:
- [x] **Lexer:** Implementing a lexer to tokenize input source code.
- [x] **Parser:** Developing a parser to parse tokens into an abstract syntax tree (AST).
- [x] **Evaluator:** Creating an evaluator to interpret and execute code represented by the AST.
- [x] **Runtime:** Implementing a runtime environment responsible for managing execution contexts, memory allocation, and variable scope resolution during program execution.
- [x] **GC:** Implementing a Garbage Collection (GC) system to automatically manage memory, reclaim unused memory.
- [ ] **Documentation:** Developing thorough documentation that covers usage instructions, language syntax and examples.
- [ ] **Visual Studio Code Plugin:** Creating a plugin for Visual Studio Code to enhance the development experience for Zynk.

