/*
 * File:   main_master.c
 * Author:  Aldo Caniz y Jose Najera
 *
 * Created on 9 de septiembre de 2022, 08:53 AM
 */

//Conectar Master con LCD y con ESP por medio de SPI ademas de conectar a pic esclavo por UART
#include "LCD.h"
#include "Osciladors.h"
#include "SPI.h"
#include <xc.h>

uint8_t mensaje = 1;

void setup(void);

void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        mensaje = RCREG;
    }
}

void main(void) {
    while(1){
        if (PIR1bits.TXIF){             // Esperamos a que este libre el ?
            TXREG = mensaje;    // Cargamos caracter a enviar
        }
        //Prueba SPI
        spiWrite(mensaje);
        __delay_ms(100);
    }
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
    
    
    //SPI
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    // Configuraciones de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepci n
}