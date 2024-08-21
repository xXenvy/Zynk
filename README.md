<div align="center">
  
  <img src="https://github.com/xXenvy/Zynk/assets/111158232/2f92acf0-409e-4e6d-b80f-2379b01d31f7" width="200" height="200">

  # Zynk - Turing-complete, interpreted programming language.

  ![commits](https://img.shields.io/github/commit-activity/w/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
  ![license](https://img.shields.io/github/license/xXenvy/Zynk?style=for-the-badge&color=%2315b328)
  ![tests](https://img.shields.io/github/check-runs/xXenvy/Zynk/master?style=for-the-badge&label=Tests&color=%2315b328)
  ![cpp](https://img.shields.io/badge/C++%20version-17-blue.svg?style=for-the-badge&logo=c%2B%2B&color=%2315b328)
</div>
<!--- ![release](https://img.shields.io/github/v/release/xXenvy/Zynk?include_prereleases&style=for-the-badge&color=%2315b328) -->

## `📃` Introduction
Zynk is a Turing-complete, interpreted programming language, written in C++. This project was created as a personal endeavor to learn and practice C++. It is not intended for extensive practical use but rather serves as a learning platform.

## `🌟` Features
- **Interpreted Language:** Zynk is an interpreted language, which facilitates quick development and testing cycles.
- **Turing-Complete:** Zynk is capable of expressing any computable function, adhering to the turing-complete classification.
- **Strongly Typed:** Zynk enforces strict type rules, ensuring that variables are explicitly typed and type conversions are controlled, reducing runtime errors.

## `📖` Documentation

Comprehensive documentation for Zynk is available on [**Link**](https://xxenvy.github.io/Zynk/). The documentation includes:

- **[Installation Guide](https://xxenvy.github.io/Zynk/installation/):** Detailed instructions for installing Zynk.
- **[Hello World Tutorial](https://xxenvy.github.io/Zynk/examples/hello_world/):** A step-by-step guide to writing your first Zynk program.
- **[Basic Examples](https://xxenvy.github.io/Zynk/examples/basics/):** Simple code examples to get you started.
- **[Data Types](https://xxenvy.github.io/Zynk/data-types/):** Overview of primitive data types in Zynk.

## `🔧` Example Usage

```java
def main() -> null {
    var name: string = readInput("Enter your name: ");
    var age: int = int(readInput("Enter your age: "));
    println(f"Welcome, {name}! You are {age} years old.");
}
main();
```