# Basic Examples

Welcome to the Basic Examples section. Here, we showcase fundamental code snippets to help you get started with Zynk. 

## Example 1: Basic Variables and Output

This example demonstrates the use of variables and basic output functions.

    def main() -> null {
        var x: int = 1;
        var y: string = "elo";
        println(x);
        println(y);
    }
    main();

## Example 2: Control Flow with Loops and Conditionals

This example shows how to use loops and conditionals in Zynk.

    def main() -> null {
        var x: int = 1;

        while(true) {
            x = x + 1;
            if(x >= 10) {
                break;
            }
        }
    }
    main();

## Example 3: Nested Functions

This example demonstrates defining and calling functions, including nested functions.

    def main() -> null {
        var number: int = 1;

        def displayNumber() -> null {
            println(number);
            var multiplier: float = 1.5;
            def calculateAndPrint() -> null {
                println(number * multiplier);
            }
            calculateAndPrint();
        }
        displayNumber();
    }
    main();

## Example 4: Reading Input

This example demonstrates how to read input from the user and use it in your program.

    def main() -> null {
        var name: string = readInput("Enter your name: ");
        println(name);
    }
    main();

## Example 5: Type Casting

This example demonstrates how to use type casting in Zynk. Type casting allows you to convert values between different types.

    def main() -> null {
        var intValue: int = 5;
        var floatValue: float = float(intValue);
        var stringValue: string = string(floatValue);
        
        println(intValue);
        println(floatValue);
        println(stringValue);
        
        var stringInput: string = "42";
        var castedInt: int = int(stringInput);
        println(castedInt + 5);
    }
    main();

## Example 6: Logical Operators (AND, OR)

This example demonstrates the use of logical operators `AND` and `OR` in Zynk.


    def main() -> null {
        var a: bool = true;
        var b: bool = false;
        var c: bool = a and b;
        var d: bool = a or b;

        if(c && d) {
            println("Both c and d are true.");
        }
    }
    main();

## Example 7: Formatted Strings (f-string)

This example demonstrates how to use f-strings to create formatted strings in Zynk.

    def main() -> null {
        var name: string = "Alice";
        var age: int = 30;
        var height: float = 185;

        // Using f-string to create a formatted string
        var message: string = f"Name: {name}, Age: {age}, Height: {height}";

        // Printing the formatted string
        println(message);
        println(f"{name} will be {age + 1} years old next year.");
    }
    main();