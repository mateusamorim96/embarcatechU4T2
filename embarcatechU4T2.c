#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define LED_RED 13
#define LED_BLUE 12
#define LED_GREEN 11

// Define o pino do buzzer
#define BUZZER 21

void setup() {
    gpio_init(LED_GREEN);   // Inicializa o pino 11 para o LED verde
    gpio_init(LED_BLUE);    // Inicializa o pino 12 para o LED azul
    gpio_init(LED_RED);     // Inicializa o pino 13 para o LED vermelho
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Apaga todos os LEDs ao iniciar
    gpio_put(LED_GREEN, 1);
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_RED, 1);
}

void turn_off_leds() {
    // Desliga todos os LEDs
    gpio_put(LED_GREEN, 1);
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_RED, 1);
}

int main() {
    stdio_init_all();
    setup();

    // Configura a UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_puts(UART_ID, "Hello, UART!\n");
    printf("Sistema ok. Qual o comando UART?\n");

    while (true) {
        char command[20];

        // Lê o comando do terminal
        if(scanf("%19s", command) == 1) {
            if(strcmp(command, "GREEN") == 0) {
                turn_off_leds();   
                gpio_put(LED_GREEN, 0); 

                printf("Led verde ligado\n");
            } else {
                printf("Comando inválido.\n");
            }
        }
    }

    return 0;
}
