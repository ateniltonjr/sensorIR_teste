#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições de pinos
#define IN1 18   // Pino para LED/Motor
#define IN2 19   // Pino para LED/Motor

// Aumentamos a resolução do PWM para 10 bits (0–1023)
#define PWM_RES 1023

// Configura um pino como PWM
uint pwm_setup(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, PWM_RES);   // Agora 10 bits
    pwm_set_clkdiv(slice, 4.0);     // Frequência ajustada
    pwm_set_enabled(slice, true);
    return slice;
}

// Define velocidade em um pino PWM
void motor_set_speed(uint gpio, uint speed) {
    pwm_set_gpio_level(gpio, speed);
}

// Girar para frente (PWM no IN1, IN2 desligado)
void motor_forward(uint speed) {
    motor_set_speed(IN1, speed);
    motor_set_speed(IN2, 0);
}

// Girar para trás (PWM no IN2, IN1 desligado)
void motor_reverse(uint speed) {
    motor_set_speed(IN1, 0);
    motor_set_speed(IN2, speed);
}

// Parar motor (ambos desligados)
void motor_stop() {
    motor_set_speed(IN1, 0);
    motor_set_speed(IN2, 0);
}

void acelerar() {
    for(int i = 0; i <= PWM_RES; i++) {
        motor_forward(i);
        sleep_ms(5);   // Mais suave
    }
}

void desacelerar() {
    for(int i = PWM_RES; i >= 0; i--) {
        motor_forward(i);
        sleep_ms(5);
    }
}

void acelerar_tras() {
    for(int i = 0; i <= PWM_RES; i++) {
        motor_reverse(i);
        sleep_ms(5);
    }
}

void desacelerar_tras() {
    for(int i = PWM_RES; i >= 0; i--) {
        motor_reverse(i);
        sleep_ms(5);
    }
}

int main() {
    stdio_init_all();

    // Configura IN1 e IN2 como PWM
    pwm_setup(IN1);
    pwm_setup(IN2);

    while (1) {
        // Frente acelerando
        acelerar();

        // Frente desacelerando
        desacelerar();

        // Trás acelerando
        acelerar_tras();

        // Trás desacelerando
        desacelerar_tras();
    }
}
