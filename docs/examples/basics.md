# Basic Examples

Welcome to the Basic Examples section. Here, we showcase fundamental code snippets to help you get started with Zynk. 

## Example 1: Basic Variables and Output

This example demonstrates the use of variables and basic output functions.

    def main() {
        var x: int = 1;
        var y: string = "elo";
        println(x);
        println(y);
    }
    main();

## Example 2: Control Flow with Loops and Conditionals

This example shows how to use loops and conditionals in Zynk.

    def main() {
        var x: int = 1;

        while(true) {
            if(x == 1) {
                break;
            }
        }
    }
    main();

## Example 3: Nested Functions

This example demonstrates defining and calling functions, including nested functions.

    def main() {
        var number: int = 1;

        def displayNumber() {
            println(number);
            var multiplier: float = 1.5;
            def calculateAndPrint() {
                println(number * multiplier);
            }
            calculateAndPrint();
        }
        displayNumber();
    }
    main();