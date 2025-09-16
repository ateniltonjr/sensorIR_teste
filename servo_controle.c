#include "pico/stdlib.h" //Padrão
#include "hardware/pwm.h" // Biblioteca para controle de PWM
#include <stdio.h>      // Biblioteca para funções de entrada e saída

// BOOTSEEL
#define botaoB 6
void iniciar_botoes() {
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
}

#include "pico/bootrom.h"
void bootsel() 
{   if (gpio_get(botaoB) == 0)
    {
        reset_usb_boot(0, 0);
    } 
}

#define PWM_FREQ 50
#define TOP 25000
#define SERVO_PIN 18
#define CLOCK_DIVIDER 100.0f

// Variáveis globais
uint slice_num; // Variável para armazenar o número do slice PWM
uint channel;  // Variável para armazenar o canal PWM

uint16_t calcula_pulso(uint16_t angulo) {  // Calcula o pulso PWM com base no ângulo
    return 500 + (angulo * (2400 - 500) / 180);  // Retorna o valor do pulso correspondente ao ângulo
}

void posicao(uint16_t pulse_us) {  // Define a posição do servo com base no pulso
    uint16_t level = (pulse_us * TOP) / 20000;  // Calcula o nível PWM baseado no pulso
    pwm_set_chan_level(slice_num, channel, level);  // Define o nível do canal PWM
}

void servo_config() {  // Configura o servo para operar com PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);  // Configura o pino do servo como PWM
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);  // Obtém o slice correspondente ao pino
    channel = pwm_gpio_to_channel(SERVO_PIN);  // Obtém o canal correspondente ao pino

    pwm_config config = pwm_get_default_config();  // Obtém a configuração padrão do PWM
    pwm_config_set_clkdiv(&config, CLOCK_DIVIDER);  // Define o divisor de clock
    pwm_config_set_wrap(&config, TOP);  // Define o valor de contagem máximo
        pwm_init(slice_num, &config, true);  // Inicializa o PWM com a configuração definida
        pwm_set_enabled(slice_num, true);    // Habilita o PWM no slice do pino 28
}

void servo_movimento_periodico() {  // Rotina de movimentação periódica suave
        posicao(calcula_pulso(0));   // Vai para 0 graus
        sleep_ms(1000);              // Espera 1 segundo
        posicao(calcula_pulso(90));  // Vai para 90 graus
        sleep_ms(1000);              // Espera 1 segund
}

int main() {
    iniciar_botoes();
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &bootsel);
    servo_config();  // Habilita a configuração do serv

    while (true) { servo_movimento_periodico(); } // Implementação da função que estará no loop
    return 0;  // Este ponto nunca será alcançado
}