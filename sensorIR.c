#include <stdio.h>
#include "pico/stdlib.h"

#define SENSOR1 16   // GPIO onde está ligado o IR 1
#define SENSOR2 17   // GPIO onde está ligado o IR 2

#define led1 12
#define led2 13
 
void iniciar_sensor(){
    gpio_init(SENSOR1);
    gpio_set_dir(SENSOR1, GPIO_IN);
    gpio_pull_up(SENSOR1);  // ativa pull-up (opcional, depende do sensor)

    gpio_init(SENSOR2);
    gpio_set_dir(SENSOR2, GPIO_IN);
    gpio_pull_up(SENSOR2);  // ativa pull-up (opcional, depende do sensor)
}

void iniciar_leds(){
    gpio_init(led1);
    gpio_set_dir(led1, GPIO_OUT);

    gpio_init(led2);
    gpio_set_dir(led2, GPIO_OUT);
}

void leds(bool l1, bool l2){
    gpio_put(led1, l1);
    gpio_put(led2, l2);
}

int s1;
int s2;

void ligar_led()
{
    s1 = gpio_get(SENSOR1);
    s2 = gpio_get(SENSOR2);

    if (s1 == 0 & s2 == 0) // Objeto detectado
    {
        leds(1, 1); // Liga LED
    }
    else if (s1 == 0 & s2 == 1)
    {
        leds(1, 0); // Liga LED
    }
    else if (s1 == 1 & s2 == 0)
    {
        leds(0, 1); // Liga LED
    }
    else
    {
        leds(0, 0); // Desliga LED
    }
    
}

#include "pico/bootrom.h"
#define botaoB 6
void iniciar_botoes() {
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
}

void bootsel() 
{   if (gpio_get(botaoB) == 0)
    {
        reset_usb_boot(0, 0);
    }
    
}  

int main() {
    // Inicializa comunicação serial
    stdio_init_all();
    iniciar_leds();
    iniciar_sensor();

    iniciar_botoes();
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &bootsel);

    while (true) {
        // Leitura dos sensores (0 ou 1)
        
        s1 = gpio_get(SENSOR1);
        s2 = gpio_get(SENSOR2);
        ligar_led();
        printf("S1=%d S2 = %d\n", s1, s2);

        sleep_ms(100);
    }
}
