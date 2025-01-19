#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define LED_RED 13
#define LED_BLUE 12
#define LED_GREEN 11

// Define o pino do buzzer
#define BUZZER 21

void setup() {
    gpio_init(LED_GREEN);   //inicializa o pino 11 para o led verde
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_RED, 0);
}

void turn_off_leds(){
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_RED, 0);
}

int main()
{
    stdio_init_all();
    setup();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart
    printf("Sistema ok. Qual o comando UART?\n");
    while (true) {
        char command[20];

        if(scanf("%19s", command) == 1){
            if(strcmp(command, "LED_GREEN") == 0){

            }
        }
        sleep_ms(1000);
    }
}
