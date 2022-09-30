/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OSCILADOR_H
#define	OSCILADOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
//------------Variables----------------
uint8_t freq = 0;

//------------Prototipo----------------
void int_osc_MHz (uint8_t freq);



#endif