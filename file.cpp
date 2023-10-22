#include <iostream>

// Function to add two numbers
int add(int a, int b) {
    return a + b;
}

int main() {
    // Declare variables
    int num1 = 5;
    int num2 = 10;

    // Calculate the sum
    int result = add(num1, num2);

    // Output the result
    std::cout << "The sum is: " << result << std::endl;

    // Check a condition
    if (result > 10) {
        std::cout << "Result is greater than 10." << std::endl;
    } else {
        std::cout << "Result is not greater than 10." << std::endl;
    }

    /*
    Multi-line comment
    It spans multiple lines
    */

    // Return from the main function
    return 0;
}
