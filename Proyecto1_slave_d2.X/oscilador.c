/*
 * File:   oscilador.c
 * Author: Josea
 *
 * Created on 22 de julio de 2022, 08:43 AM
 */

#include "oscilador.h"
#include <xc.h>

//------------Funcion------------------
void int_osc_MHz (uint8_t freq){
    OSCCONbits.SCS = 1;
    
    switch(freq){
            case 1:
                OSCCONbits.IRCF = 0b100; //1Mhz             
                break;
            case 2:
                OSCCONbits.IRCF = 0b101; //2Mhz      
                break;
            case 4:
                 OSCCONbits.IRCF = 0b110; //4Mhz                      
                break;
            case 8:
                OSCCONbits.IRCF = 0b111; //8Mhz      
                break;
        default:
            OSCCONbits.IRCF = 0b110; //4Mhz  
                break;      
        }
}
