/*
 * File:   tmr0.c
 * Author: Josea
 *
 * Created on 22 de julio de 2022, 09:56 AM
 */


#include <xc.h>
#include "tmr0.h"

void tmr0_init (uint8_t prescaler){
    //----------SETUP------------------------------------------
    OPTION_REGbits.T0CS = 0; //Oscilador interno
    OPTION_REGbits.PSA = 0;  //Asignamos el Prescaler al TMR0
    TMR0 = _tmr0_value;     //Valor inicial para 500ms
    
    //Interrupciones
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    
    
    switch(prescaler){
            case 0: 
                OPTION_REGbits.PSA = 0;  //Asignamos el Prescaler al WDT
                OPTION_REGbits.PS = 0b000; //Prescaler 1/1
                break;
            case 2: 
                OPTION_REGbits.PS = 0b000; //Prescaler 1/2 
                break;
            case 4:
                OPTION_REGbits.PS = 0b001; //Prescaler 1/4     
                break;
            case 8:
                OPTION_REGbits.PS = 0b010; //Prescaler 1/8                    
                break;
            case 16:
                OPTION_REGbits.PS = 0b011; //Prescaler 1/16
                break;
            case 32:
                OPTION_REGbits.PS = 0b100; //Prescaler 1/32
                break;
            case 64:
                OPTION_REGbits.PS = 0b101; //Prescaler 1/64
                break;
            case 128:
                OPTION_REGbits.PS = 0b110; //Prescaler 1/128  
                break;
            case 256:
                OPTION_REGbits.PS = 0b111; //Prescaler 1/256
                break;
        default:
            OPTION_REGbits.PS = 0b111; //Prescaler 1/256
                break;      
        }
}

void tmr0_reload(void){
    TMR0 = _tmr0_value;     //Valor inicial para 500ms
}