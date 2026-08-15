#include <stdio.h>
int main(){
    int lower, upper, step;
    float celsius, fahrenheit;
    lower = 0;      // Lower limit of temperature table
    upper = 100;    // Upper limit
    step = 20;      // Step size
    printf("Celsius to Fahrenheit Temperature Conversion Table:\n");
    printf("Celsius\tFahrenheit\n");
    celsius = lower;
    while (celsius <= upper) {
        fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
        printf("%-10.0f%-6.1f\n", celsius, fahrenheit);
        celsius += step;
    }
    return 0;
}