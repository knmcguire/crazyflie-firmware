This is an simple app for the crazyflie bolt that sends the estimated position of the EKF over uart as an array of floats.

How to build and flash the app (put the bolt into bootloader mode by holding the on/of button for a few secons until the blue leds start blinking):

```bash
make clean
make all -j12
make cload
```

The app starts sending the position over uart2. Connect the Rx2 pin of the uart to a TX pin of an ftdi cable and you can read it out with this python script uart_readout.py included in the same folder

```bash
python3 uart_readout.py
```

If another firmware needs to read out the position, this is the c code that can be implemented into the firmware. This is based on the raspberry pico but this might be different for other microcontrollers / autopilots

```c
#include <stdio.h>

#include <pico/stdlib.h>
#include "hardware/uart.h"
#include <pico/time.h>

#define UART_ID uart0
#define BAUD_RATE 9600

#define UART_TX_PIN 28 // these might need to be changed
#define UART_RX_PIN 29 // these might need to be changed

int main() {

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uint8_t start_byte = 0xAA;
    uint8_t end_byte = 0x55;
    uint8_t data[3 * sizeof(float) + 2]; // Extra space for start and end bytes
    float array[3] = {0.0, 0.0, 0.0};

    while (1){

        while (uart_getc(UART_ID) != start_byte);
        for (int i = 1; i < sizeof(data) - 1; i++) { // Start at 1 to skip start byte
            data[i] = uart_getc(UART_ID);
        }
        // Check for end byte
        if (data[sizeof(data) - 1] != end_byte) {
            // Handle error
        }
        // Copy data into array
        for (int i = 0; i < 4; i++) {
            memcpy(&array[i], &data[i * sizeof(float) + 1], sizeof(float)); // Offset by 1 for start byte
        }
    }
}
