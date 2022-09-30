/*
 * File:   presion.c
 * Author: 
 *
 */

#include <xc.h>
#include "presion.h"

//--Variables--
uint32_t t_fine;

int calculateTemp(uint32_t temp, uint16_t dig1, uint16_t dig2, uint16_t dig3){
    uint32_t var1, var2, T;
    
    var1 = ((((temp >> 3) - ((uint32_t)dig1 << 1))) * ((uint32_t)dig2)) >> 11;
    var2 = (((((temp >> 4) - ((uint32_t)dig1)) * ((temp >> 4) - ((uint32_t)dig1))) >> 12) * ((uint32_t)dig3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}


uint32_t readPressure(uint32_t adc_P, uint16_t dig01, uint16_t dig02, uint16_t dig03,
        uint16_t dig04, uint16_t dig05, uint16_t dig06, uint16_t dig07,
        uint16_t dig08, uint16_t dig09){
  signed long var1, var2;
  unsigned long p;

  // Must be done first to get the t_fine variable set up
  //dato ya shifteado a la derecha

  var1 = ((signed long)t_fine>>1) - (signed long)64000;
  var2 = ((var1>>2) * (var1>>2)>>11) * (signed long)dig06;
  var2 = var2 + ((var1*((signed long)dig05))<<1);
  var2 = (var2>>2) + (((signed long)dig04) << 16);
  var1 = (((dig03*(((var1>>2)*(var1>>2))>>13))>>3)+((((signed long)dig02)*var1)>>1))>>18;
  var1 = ((((32768+var1))*((signed long)dig01))>>15);
  
  if (var1 == 0){return 0;}
  
  p = (((signed long)(((signed long)1048576)-adc_P)-(var2>>12)))*3125;
  if (p< 0x80000000){
      p = (p<<1)/((signed long)var1);
  }
  else
  {
      p = (p/(signed long)var1)*2;
  }
  var1 = (((signed long)dig09)*((signed long)(((p>>3)*(p>>3))>>13)))>>12;
  var2 = (((signed long)(p>>2))*((signed long)dig08))>>13;
  
  p = (signed long)((signed long)p+((var1+var2+dig07)>>4));
  return p;
}