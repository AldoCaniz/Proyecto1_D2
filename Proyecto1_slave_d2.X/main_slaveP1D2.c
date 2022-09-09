/*
 * File:   main_slaveP1D2.c
 * Author: Josea
 *
 * Created on 9 de septiembre de 2022, 09:22 AM
 */


#include <xc.h>
#include "I2C.h"
#include "oscilador.h"
#include "tmr0.h"

uint8_t mensaje = 1;

void setup(void);

void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        mensaje = RCREG;
    }
}

void main(void) {
    return;
}

void setup(void){
    TXSTAbits.SYNC = 0;         // Comunicaci n ascincrona (full-duplex)
    TXSTAbits.BRGH = 1;         // Baud rate de alta velocidad 
    BAUDCTLbits.BRG16 = 1;      // 16-bits para generar el baud rate
    
    SPBRG = 25;
    SPBRGH = 0;                 // Baud rate ~9600, error -> 0.16%
    
    RCSTAbits.SPEN = 1;         // Habilitamos comunicación
    TXSTAbits.TX9 = 0;          // Utilizamos solo 8 bits
    TXSTAbits.TXEN = 1;         // Habilitamos transmisor
    RCSTAbits.CREN = 1;         // Habilitamos receptor
}
