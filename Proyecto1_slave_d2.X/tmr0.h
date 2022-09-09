/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
#ifndef TMR0_H
#define TMR0_H
#define _tmr0_value 178;

#include <xc.h> // include processor files - each processor file is guarded.  
//------------Variables----------------
uint8_t prescaler = 0;

//------------Prototipo----------------
void tmr0_init (uint8_t prescaler);
void tmr0_reload(void);

#endif