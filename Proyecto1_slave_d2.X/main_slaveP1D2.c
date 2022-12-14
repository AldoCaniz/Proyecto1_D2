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

uint8_t mensaje;
uint8_t minutos = 0;

void setup(void);
void motorDC(void);

void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        mensaje = RCREG;
    }
}

void main(void) {
    while (1){
        PORTBbits.RB7 = mensaje;
        
        //I2C_probando ejemplo "single byte read, control register" del datasheet
        I2C_Master_Start();    // Iniciamos la comunicacion
        I2C_Master_Write(0b11010000); //Esta es la direccion del RTC segun el datasheet, con el ultimo bit en 0 por que vamos a escribir
        I2C_Master_Write(0x01); //Le decimos que este es el registro que queremos leer (01h es el de los minutos) 
        I2C_Master_RepeatedStart(); // Reiniciamos la comunicacion para poder leer el registro
        I2C_Master_Write(0b11010001); // Mandamos la direccion del RTC pero con un 1 al final para leer
        minutos = I2C_Master_Read(0); // guardamos el valor del registro de segundos en la variable
        I2C_Master_Stop(); // Terminamos la comunicacion I2C
        __delay_ms(200);    //Damos un delay por que estamos en el loop
        
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
    
    //Configuracion I2C
    I2C_Master_Init(100000);    //Iniciamos la comunicacion I2C
    
    
    //Puertos
    TRISD = 0;
    PORTD = 0;
    
    // Configuraciones de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepcion
}

void motorDC(void){
    if (minutos == 30){
        PORTDbits.RD1 = 1;  //Cada 30 minutos hacemos que suba el asta de la estación
    }
    if (minutos == 31){
        PORTDbits.RD1 = 0;
    }
};