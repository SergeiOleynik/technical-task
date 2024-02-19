
/*								 convert.cpp
 *								 Created: 17.02.2024 
 */ 

#include "convert.h"


CONVERT::CONVERT(){
	
 /*					timer 0 init
  *CS02:CS00:clkI/O/1024 (From Prescaler)
  *8 000 000/1024 = 7812 / 255 = 36 Hz interrupt
  *TOIE0:Timer/Counter0 Overflow Interrupt Enable*/
  TCCR0B |= (1<<CS02)|(1<<CS00);
  TIMSK  |= (1<<TOIE0);
  
  
 /*					ADC init
  *ADATE: ADC Auto Trigger Enable
  *ADIE: ADC Interrupt Enable
  *ADPS2:ADPS1:ADPS0:ADC Prescaler Select = 128
  *8 000 000 / 128 = 62 500
  *ADC0D:ADC2D:Digital Input Disable*/
  ADCSRA |= (1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  DIDR0 |= (1<<ADC0D)|(1<<ADC2D);
  
  asm("sei");
    	
}
