#include <stm32f103xb.h>
#include <stdbool.h>
#include "delay.h"
int est[10][7]={
    {1,1,1,1,1,1,0},  /*0*/  
    {0,1,1,0,0,0,0},  /*1*/
    {1,1,0,1,1,0,1},  /*2*/  
    {1,1,1,1,0,0,1},  /*3*/      
    {0,1,1,0,0,1,1},  /*4*/  
    {1,0,1,1,0,1,1},  /*5*/  
    {1,0,1,1,1,1,1},  /*6*/  
    {1,1,1,0,0,0,0},  /*7*/  
    {1,1,1,1,1,1,1},  /*8*/  
    {1,1,1,1,0,1,1},  /*9*/
};
int i = 0, cont = 0;
volatile bool direccion = true;
int boton = 8;
int pines[7] = {1,2,3,4,5,6,7};
int main (void){
    RCC -> APB2ENR|=RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

        /*declaracion de pines*/
    for(i=0; i<7;i++){
        GPIOA -> CRL &=~(0xF<<(pines[i]*4)); /*limpia el puerto a poniendo todo en 0*/
        GPIOA -> CRL |=(0b0001<<(pines[i]*4)); /*setean los pines que quiero usar en 1*/
    }
        GPIOA -> CRH &=~(0xF<<((boton%8)*4)); /*limpia el puerto a poniendo todo en 0*/
        GPIOA -> CRH |=(0b0100<<((boton%8)*4)); /*setean los pines que quiero usar en 1*/
        
        AFIO->EXTICR[2] &= ~(0xF << (8%4 * 4));
        AFIO->EXTICR[2] |= (0x0 << (8%4 * 4));

        EXTI->IMR  |= (1 << 8);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
        EXTI->RTSR |= (1 << 8);   // Habilitar flanco de subida
        EXTI->FTSR &= ~(1 << 8);  // Deshabilitar flanco de bajada

        NVIC_EnableIRQ(EXTI9_5_IRQn);
        NVIC_SetPriority(EXTI9_5_IRQn, 1);
    while (1){
        
        
        if(direccion){
            for(i=0; i<7; i++){
                if(est[cont][i]){
                    GPIOA -> BSRR|= (1<<pines[i]);
                }else{
                    GPIOA -> BSRR|= (1<<(pines[i]+16));
                }
            }if(cont>9){
                cont=0;
            }
            cont++;
        }else{
            for(i=0; i<7; i++){
                if(est[cont][i]){
                    GPIOA -> BSRR|= (1<<pines[i]);
                }else{
                    GPIOA -> BSRR|= (1<<(pines[i]+16));
                }
            }if(cont<0){
                cont=9;
            }
            cont--;
        }        
        delay_ms(1000);
    }
    
}
void EXTI9_5_IRQHandler(){
    if (EXTI->PR & (1 << 8)){ //lee el estado del boton,
        EXTI->PR |= (1 << 8);
        direccion =! direccion; 
        }
}