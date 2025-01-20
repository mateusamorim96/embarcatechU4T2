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
void BUZZER_CONFIG (uint gpio, float frequencia, float duty_cycle){
    gpio_set_function(gpio, GPIO_FUNC_PWM); //DEFINE O PINO COMO SAÍDA PWM
    uint slice_num = pwm_gpio_to_slice_num(gpio); //OBTEM O NUMERO DA SLICE ASSOCIADA AO PINO
    uint channel = pwm_gpio_to_channel(gpio); //OBETEM O NÚMERO DO CANAL

    float clk_div = 4.0f;  // DIVISOR DE CLOCK
    uint32_t wrap = (uint32_t)((125e6 / (frequencia * clk_div)) - 1);
   
    // CONFIGURAÇÃO PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clk_div);
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);
    
    // CONFIGURAÇÃO DUTY_CICLE
     uint32_t level = (uint32_t)(wrap * duty_cycle);
    pwm_set_chan_level(slice_num, channel, level);
    pwm_set_enabled(slice_num, true);

}
void CONTROLE_BUZZER(){

    BUZZER_CONFIG(BUZZER, 880.0f, 0.5f); //CONFIGURA FREQUENCIA E DUTY_CYCLE
        sleep_ms(2000);
        
        
        uint slice_num = pwm_gpio_to_slice_num(BUZZER);
        uint channel = pwm_gpio_to_channel(BUZZER);
        pwm_set_chan_level(slice_num, channel, 0);  //DESLIGAR BUZZER

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
        } else if (strcmp(command, "WHITE") == 0) {
            turn_off_leds();   
            gpio_put(LED_RED, 1); 
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_BLUE, 1);
            uart_puts(UART_ID, "LED branco ligado\n");
        } else if (strcmp(command, 'BUZZER'))
            CONTROLE_BUZZER();
            uart_puts(UART_ID, "Buzzer ligado\n" );

        else if (strcmp(command, "OFF") == 0) {
            turn_off_leds();
            uart_puts(UART_ID, "Todos os LEDs desligados\n");
        } else if (i > 0) {
            uart_puts(UART_ID, "Comando invalido.\n");
        }
    }

    return 0;
}
