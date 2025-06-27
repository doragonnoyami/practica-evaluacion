#include "stm32f103xb.h"  

volatile uint32_t tick_ms = 0;


void delayInit(void) {
    // Configura el SysTick para interrumpir cada 1 ms (72 MHz / 72000 = 1000 Hz)
    SysTick_Config(SystemCoreClock / 1000);
}

/* Manejo de timer */
void SysTick_Handler(void) {
    if (tick_ms > 0) tick_ms--;
}

/* Función delay en milisegundos */ 
void delay_ms(int ms) {
    delayInit();
    tick_ms = ms;
    while (tick_ms);
}