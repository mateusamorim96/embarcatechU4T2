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
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_RED, 0);
}

void turn_off_leds() {
    // Desliga todos os LEDs
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_RED, 0);
}

int main() {
    // Inicializa o sistema
    stdio_init_all();
    setup();

    // Configura a UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Mensagem de inicialização
    uart_puts(UART_ID, "Hello, UART!\n");

    char command[20];
    while (true) {
        // Lê comandos da UART1
        int i = 0;
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\n' || c == '\r') break;
            if (i < sizeof(command) - 1) command[i++] = c;
        }
        command[i] = '\0'; // Termina a string

        // Processa o comando recebido
        if (strcmp(command, "GREEN") == 0) {
            turn_off_leds();   
            gpio_put(LED_GREEN, 1); 
            uart_puts(UART_ID, "LED verde ligado\n");
        } else if (strcmp(command, "BLUE") == 0) {
            turn_off_leds();
            gpio_put(LED_BLUE, 0);
            uart_puts(UART_ID, "LED azul ligado\n");
        } else if (strcmp(command, "RED") == 0) {
            turn_off_leds();   
            gpio_put(LED_RED, 1); 
            uart_puts(UART_ID, "LED vermelho ligado\n");
        } else if (strcmp(command, "OFF") == 0) {
            turn_off_leds();
            uart_puts(UART_ID, "Todos os LEDs desligados\n");
        } else if (i > 0) {
            uart_puts(UART_ID, "Comando invalido.\n");
        }
    }

    return 0;
}
