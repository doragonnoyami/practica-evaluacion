#include <stm32f103xb.h>
#include <stdbool.h>
#include "delay.h"
#include "ctype.h"
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
int i = 0, cont = 0, uni = 0, dec = 0;
volatile bool direccion = true;
int seg1[7] = {1,2,3,4,5,6,7};
int seg2[7] = {8,9,10,11,12,13,14};
int boton_A= 1, boton_B=2, boton_C=3;
void BCD_init(){
    RCC-> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
    RCC-> APB2ENR |= RCC_APB2ENR_AFIOEN;

    for(i=0; i<7;i++){
        GPIOA -> CRL &=~ (0xF<<(seg1[i]*4));
        GPIOA -> CRL |= (0b0001<<(seg1[i]*4));
        GPIOA -> CRH &=~ (0xF<<((seg2[i]%8)*4));
        GPIOA -> CRH |= (0b0001<<((seg2[i]%8)*4));
    }
        GPIOB -> CRH &=~ (0xF<<((boton_A%8)*4));
        GPIOB -> CRH |= (0b0100<<((boton_A%8)*4));
        GPIOB -> CRH &=~ (0xF<<((boton_B%8)*4));
        GPIOB -> CRH |= (0b0100<<((boton_B%8)*4));
        GPIOB -> CRH &=~ (0xF<<((boton_C%8)*4));
        GPIOB -> CRH |= (0b0100<<((boton_C%8)*4));

        AFIO->EXTICR[0] &= ~(0xF << (boton_A* 4));
        AFIO->EXTICR[0] |= (0x0 << (boton_A* 4));
        AFIO->EXTICR[0] &= ~(0xF << (boton_B* 4));
        AFIO->EXTICR[0] |= (0x0 << (boton_B* 4));
        AFIO->EXTICR[0] &= ~(0xF << (boton_C* 4));
        AFIO->EXTICR[0] |= (0x0 << (boton_C* 4));

        EXTI->IMR  |= (1 << boton_A);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
        EXTI->RTSR |= (1 << boton_A);   // Habilitar flanco de subida
        EXTI->FTSR &= ~(1 << boton_A);  // Deshabilitar flanco de bajada
        EXTI->IMR  |= (1 << boton_B);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
        EXTI->RTSR |= (1 << boton_B);   // Habilitar flanco de subida
        EXTI->FTSR &= ~(1 << boton_B);  // Deshabilitar flanco de bajada
        EXTI->IMR  |= (1 << boton_C);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
        EXTI->RTSR |= (1 << boton_C);   // Habilitar flanco de subida
        EXTI->FTSR &= ~(1 << boton_C);  // Deshabilitar flanco de bajada

        NVIC_EnableIRQ(EXTI1_IRQn);
        NVIC_SetPriority(EXTI1_IRQn, 1);
        NVIC_EnableIRQ(EXTI2_IRQn);
        NVIC_SetPriority(EXTI2_IRQn, 1);
        NVIC_EnableIRQ(EXTI3_IRQn);
        NVIC_SetPriority(EXTI3_IRQn, 0);

        for(i=0; i<7;i++){
            if(est[0][i]){
                GPIOA -> BSRR|= (1<<seg1[i]);
                GPIOA -> BSRR|= (1<<seg2[i]);
            }else{
                GPIOA -> BSRR|= (1<<(seg1[i]+16));
                GPIOA -> BSRR|= (1<<(seg2[i]+16));
            }
        }
}
void BCD_puls(char letra){
    letra=toupper(letra);//sirve para que la minuscula sea equivalente a la mayuscula
    if(letra='C'){
        uni=0;
        dec=0;
    }
    if(letra='A'){
        for(i=0; i<7;i++){
            if(est[uni][i]){
                GPIOA -> BSRR|= (1<<seg1[i]);
            }else{
                GPIOA -> BSRR|= (1<<(seg1[i]+16));
            }
            if(est[dec][i]){
                GPIOA -> BSRR|= (1<<seg2[i]);
            }else{
                GPIOA -> BSRR|= (1<<(seg2[i]+16));
            }
            uni++;
            if(uni>9){
                uni=0;
                dec++;
            }
        }
    }
}