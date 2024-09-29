#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>



// Define the GPIO pins

#define GPIO_INPUT 233   // button sensor

#define GPIO_OUTPUT 71   // led



// Function to export a GPIO pin 

void export_gpio(int gpio_pin) {

    FILE *export_file = fopen("/sys/class/gpio/export", "w");

    if (export_file == NULL) {

        // Print an error message if the file couldn't open

        printf("Unable to export GPIO %d.\n", gpio_pin);

        return;

    

    // Write the GPIO pin number to the export file

    fprintf(export_file, "%d", gpio_pin);

    fclose(export_file);

    } else {

        printf("GPIO %d is already exported.\n", gpio_pin);

    }



}



// Function to set the directions

void set_gpio_direction(int gpio_pin, const char *direction) {

    char path[64];

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpio_pin);

    FILE *direction_file = fopen(path, "w");

    if (direction_file == NULL) {

        printf("Unable to set direction for GPIO %d.\n", gpio_pin);

        return;

    }



    fprintf(direction_file, "%s", direction);

    fclose(direction_file);

    printf("Setting GPIO %d direction to %s...\n", gpio_pin, direction);

}

// Function to read the value of a GPIO input pin

int read_gpio(int gpio_pin) {

    char path[64], value_str[4];

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio_pin);

    FILE *value_file = fopen(path, "r");  //in read mode

    if (value_file == NULL) {

        printf("Unable to read GPIO %d.\n", gpio_pin);

        return -1;

    }



    fgets(value_str, sizeof(value_str), value_file);

    fclose(value_file);

    // Convert the value string to an integer (0 or 1) and return it

    return atoi(value_str);

}



// Function to write a value to a GPIO output pin (0 = LOW, 1 = HIGH)

void write_gpio(int gpio_pin, int value) {

    char path[64];

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio_pin);

    FILE *value_file = fopen(path, "w");

    if (value_file == NULL) {

        printf("Unable to control GPIO %d.\n", gpio_pin);

        return;

    }



    // Write the desired value ("0" for LOW, "1" for HIGH)

    fprintf(value_file, "%d", value);

    fclose(value_file);

}

int main() {

    // Export GPIO pins

    export_gpio(GPIO_INPUT);

    export_gpio(GPIO_OUTPUT);



    // Set the directios

    set_gpio_direction(GPIO_INPUT, "in");

    set_gpio_direction(GPIO_OUTPUT, "out");



    while (1) {

        int input_value = read_gpio(GPIO_INPUT);

        printf("Current input status of GPIO %d is %d\n", GPIO_INPUT, input_value);

        if (input_value == 1) {

            // If input is HIGH (1), set output pin HIGH to turn on LED

            write_gpio(GPIO_OUTPUT, 1);

            printf("Input is HIGH. Setting GPIO %d to HIGH...\n", GPIO_OUTPUT);

        } else {

            // If input is LOW (0), set output pin LOW to turn off LED

            write_gpio(GPIO_OUTPUT, 0);

            printf("Input is LOW. Setting GPIO %d to LOW...\n", GPIO_OUTPUT);

        }





        printf("Waiting for 0.1 seconds...\n");

        usleep(100000); 

    }



    return 0;

}
