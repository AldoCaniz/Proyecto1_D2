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

#define _XTAL_FREQ 4000000
#define IN_MIN 0                // Valor minimo de entrada del potenciometro
#define IN_MAX 255              // Valor máximo de entrada del potenciometro
#define OUT_MIN 0               // Valor minimo de ancho de pulso de señal PWM
#define OUT_MAX 500             // Valor máximo de ancho de pulso de señal PWM

uint8_t mensaje = 1;
uint8_t ENVIO, reloj, presion, pot, boton = 0;
unsigned short CCPR = 0; 

void setup(void);
unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, 
            unsigned short out_min, unsigned short out_max);



void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        mensaje = RCREG;
    }
    
    if (PIR1bits.SSPIF){                            // Interrupcion del SPI
        ENVIO = spiRead();                          // Se lee lo que se desea enviar al Master
        if (ENVIO == 1){
            spiWrite(reloj);                            // Enviamos el valor del RTC al ESP32
        } else if (ENVIO == 2) {
            spiWrite(presion);                            // Enviamos el valor del sensor de presion al ESP32
        } else if (ENVIO == 3){
            spiWrite(pot);                            // Enviamos el valor del sensor de presion al ESP32
        } else if (ENVIO == 4){
            spiWrite(boton);                            // Enviamos el valor del sensor de presion al ESP32
        }
        PIR1bits.SSPIF = 0;
    }
    if(PIR1bits.ADIF){                      // Fue interrupción del ADC?
        if(ADCON0bits.CHS == 0){            // Verificamos sea AN0 el canal seleccionado
            CCPR = map(ADRESH, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX); // Valor de ancho de pulso
            CCPR1L = (uint8_t)(CCPR>>2);    // Guardamos los 8 bits mas significativos en CPR1L
            CCP1CONbits.DC1B = CCPR & 0b11; // Guardamos los 2 bits menos significativos en DC1B
            PIR1bits.ADIF = 0;                  // Limpiamos bandera de interrupción
    }
    
}
    }
    

void main(void) {
    while(1){
        if (PIR1bits.TXIF){             // Esperamos a que este libre el ?
            TXREG = mensaje;    // Cargamos caracter a enviar
        }
        
        
        if(ADCON0bits.GO == 0){             // No hay proceso de conversion
            if(ADCON0bits.CHS == 0b0000)    
                ADCON0bits.CHS = 0b0001;    // Cambio de canal
            else if(ADCON0bits.CHS == 0b0001)
                ADCON0bits.CHS = 0b0000;    // Cambio de canal
            __delay_us(40);                 // Tiempo de adquisición            
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversión
        }
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
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Configurando el PIC en modo SLAVE para recibir / enviar datos al ESP32
    // Configuraciones de interrupciones
 
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepci n
    PIR1bits.ADIF = 0;          // Limpiamos bandera de ADC
    PIE1bits.ADIE = 1;          // Habilitamos interrupcion de ADC
    INTCONbits.PEIE = 1;        // Habilitamos int. de perifericos
    INTCONbits.GIE = 1;         // Habilitamos int. globales
    
    
    
    // Configuración ADC
    ADCON0bits.ADCS = 0b01;     // Fosc/8
    ADCON1bits.VCFG0 = 0;       // VDD
    ADCON1bits.VCFG1 = 0;       // VSS
    ADCON0bits.CHS = 0b0000;    // Seleccionamos el AN0
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
    __delay_us(40);             // Sample time
    
    // Configuración PWM
    TRISCbits.TRISC2 = 1;       // Deshabilitamos salida de CCP1
    TRISCbits.TRISC3 = 1;       // Deshabilitamos salida de CCP1
    PR2 = 124;                  // periodo de 2ms
    
    // Configuración CCP
    CCP1CON = 0;                // Apagamos CCP1
    CCP1CONbits.P1M = 0;        // Modo single output
    CCP1CONbits.CCP1M = 0b1100; // PWM
    
    CCPR1L = 125>>2;
    CCP1CONbits.DC1B = 125 & 0b11;    // 0.5ms ancho de pulso / 25% ciclo de trabajo
    
    TRISCbits.TRISC2 = 0;       // Habilitamos salida de PWM
    TRISCbits.TRISC3 = 0;       // Habilitamos salida de PWM
    
    
    
}

unsigned short map(uint8_t x, uint8_t x0, uint8_t x1, 
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}