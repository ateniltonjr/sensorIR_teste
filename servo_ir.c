#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 9
#define SENSOR 18
#define TOP 25000
#define CLOCK_DIVIDER 100.0f

uint slice_num, channel;

uint16_t calcula_pulso(uint16_t angulo) {
    return 500 + (angulo * (2400 - 500) / 180);
}

void servo_config() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    channel = pwm_gpio_to_channel(SERVO_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, CLOCK_DIVIDER);
    pwm_config_set_wrap(&config, TOP);
    pwm_init(slice_num, &config, true);
    pwm_set_enabled(slice_num, true);
}

void set_servo_angle(uint16_t angulo) {
    uint16_t level = (calcula_pulso(angulo) * TOP) / 20000;
    pwm_set_chan_level(slice_num, channel, level);
}

void sensor_config() {
    gpio_init(SENSOR);
    gpio_set_dir(SENSOR, GPIO_IN);
    gpio_pull_up(SENSOR); // mantém em HIGH quando não pressionado
}

int main() {
    stdio_init_all();
    servo_config();
    sensor_config();

    // Servo em 0° inicialmente
    set_servo_angle(0);

    while (true) {
        int s = gpio_get(SENSOR);

        // Exibe estado do sensor no serial monitor
        printf("Sensor: %d\n", s);

        // Controle do servo
        if (s == 0) {
            set_servo_angle(180);  // Sensor ativo -> servo em 90°
        } else {
            set_servo_angle(0);   // Sensor inativo -> servo em 0°
        }

        sleep_ms(50);
    }
}
