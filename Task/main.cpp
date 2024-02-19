/*
 *								Created: 17.02.2024 
 */ 

#include "main.h"

LCD      lcd;
CONVERT  cnv;

	/*************************************************
	 *      1h = 3 600 000 mS = 3 600 S              *
	 *      100mS = 0.000028h, 1h = 36 000(100mS)    *
	 *      1h = 36 000*0.000028 = 1.008             *
	 *      Q = I * t, Ah	                         *
	 *      Wh = I * U * t = U * Q, Wh				 *
	 *************************************************/
	 
	/*сигнали напруги, та струму подаються кожні 10Hz, тобто 100mS     t = 100,
	  В одному часі 3 600 000 mS, відповідно 100mS = 36 000
	  100/3 600 000 = 0.00002777 часа 								   inputTime = (t/3600000)
      Ємність батареї : Q = I * t Ah
	  Енергія батареї : Wh = U * Q Wh
	  За одне вимірювання цих показників: Q = I * 0.000028h
	  Якщо вирувати(припустити,звісно, що струм,напруга не будуть залишатись на початковому рівні), 
	  ці значення за годину,(в часі 36 000 раз вимірювання з частотою 10Hz):
	  Q = I * (t*36 000) Ah                                            timeForConvert=(inputTime*36000)
	  Wh = U * Q Wh 
	  якщо напруга на батареї меньша ніж задане значення(для цього приклада constLowBattery	= 2.55), вимірювання припиняються
	 */
float	constLowBattery	= 2.55,
		t = 100,							//100mS = 10Hz
       	inputTime = (t/3600000),            //1h -> (100ms) = 100 / 3 600 000 = 0.000028 
		timeForConvert=(inputTime*36000);     
	
							     /***** interrupt handler *****/
		  /**** Timer/Counter0 Overflow interrupt ****/
ISR(TIMER0_OVF_vect){

 switch(cnv.prescaller){
  case 6:  cnv.tmpForPrint=measurementVoltage; 
		   ADCSRA |= (1<<ADEN)|(1<<ADSC);		  
  break;
  case 10: if(cnv.flagLowVoltage){
	        ADCSRA |= (1<<ADEN)|(1<<ADSC);
			cnv.tmpForPrint=measurementCurrent;		  	
		   }else lcd.display=printLowVoltage;
  break;
  case 12: if(cnv.flagLowVoltage) lcd.display=printCapacity;  break;
  case 14: if(cnv.flagLowVoltage) lcd.display=printEnergy; break;
  case 16: cnv.prescaller=0; break;	 
 } cnv.prescaller++;     		
 
}        //**** Timer/Counter0 Overflow interrupt ****

         /***** ADC Conversion Complete interrupt *****/					
ISR(ADC_vect){ 

 uint rezult=ADC;
 switch(cnv.tmpForPrint){
  case measurementVoltage: 
       cnv.voltage = (rezult*5.00/1023)*2.8; lcd.display=printVoltage;
       if(cnv.voltage<constLowBattery){ 
		 cnv.flagLowVoltage=false; ADCSRA &= ~((1<<ADEN)|(1<<ADSC)); 
	   }else{           
         ADMUX |= (1<<MUX1); ADCSRA &= ~((1<<ADEN)|(1<<ADSC));        
	     cnv.energy = (cnv.voltage*cnv.capacity); 	              
	     cnv.flagLowVoltage=true; lcd.flagClear=true;
	   }
  break;																   
  case measurementCurrent: cnv.current = (rezult*5.00/1023)*14.1;             
         ADMUX &= ~(1<<MUX1); ADCSRA &= ~((1<<ADEN)|(1<<ADSC));  	    
		 cnv.capacity = (cnv.current*timeForConvert);	  
	     lcd.display=printCurrent;	   
  break; 
 } 
 
}       //***** ADC Conversion Complete interrupt *****
                                  /***** interrupt handler *****/


		/***** main *****/	     
int main(void){  /* старт програми */
	
 uch buffVoltage[]="U = ",
	 buffCurrent[]="I = ", 
	 buffCapacity[]="Cap = ",
	 buffEnergy[]="Enr =",
	 buffLowVoltage[]="Battery low",
	 len=0;
	    
		/***** while *****/	 
 while(true){	
   switch(lcd.display){
	  case printVoltage: len=sizeof(buffVoltage); lcd.display=none;
		cnv.rezult = (cnv.voltage*100);
		cnv.tmp1000 = cnv.rezult/1000;
		cnv.tmp100 = (cnv.rezult/100)%10;
		cnv.tmp10 = (cnv.rezult/10)%10;
		cnv.tmp1 = cnv.rezult%10;  
	  	lcd.lcdSetLine(lcdLine1); lcd.lcdPrint(buffVoltage,len);
	  	if(cnv.tmp1000) lcd.lcdPrintSymbol((cnv.tmp1000)+48);
	  	else lcd.lcdPrintSymbol(' ');
	  	lcd.lcdPrintSymbol((cnv.tmp100)+48);
	  	lcd.lcdPrintSymbol('.');
	  	lcd.lcdPrintSymbol((cnv.tmp10)+48);
	  	lcd.lcdPrintSymbol((cnv.tmp1)+48);
		lcd.lcdPrintSymbol(' ');
	  	lcd.lcdPrintSymbol('V'); 
	  break; 
	  case printCurrent: len=sizeof(buffCurrent); lcd.display=none;
		cnv.rezult = (cnv.current*100);
        cnv.tmp1000 = cnv.rezult/1000;
        cnv.tmp100 = (cnv.rezult/100)%10;
		cnv.tmp10 = (cnv.rezult/10)%10;
		cnv.tmp1 = cnv.rezult%10;  
	    lcd.lcdSetLine(lcdLine2); lcd.lcdPrint(buffCurrent,len);
	    if(cnv.tmp1000) lcd.lcdPrintSymbol((cnv.tmp1000)+48);
	    else lcd.lcdPrintSymbol(' ');
	    lcd.lcdPrintSymbol((cnv.tmp100)+48);
	    lcd.lcdPrintSymbol('.');
	    lcd.lcdPrintSymbol((cnv.tmp10)+48);
	    lcd.lcdPrintSymbol((cnv.tmp1)+48);
		lcd.lcdPrintSymbol(' ');
	    lcd.lcdPrintSymbol('A'); 
	  break;
	  case printCapacity: len=sizeof(buffCapacity); lcd.display=none;
		cnv.rezult = (cnv.capacity*100);
		cnv.tmp1000 = cnv.rezult/1000;
		cnv.tmp100 = (cnv.rezult/100)%10;
		cnv.tmp10 = (cnv.rezult/10)%10;
		cnv.tmp1 = cnv.rezult%10; 
		lcd.lcdSetLine(lcdLine3); lcd.lcdPrint(buffCapacity,len);
	    if(cnv.tmp1000) lcd.lcdPrintSymbol((cnv.tmp1000)+48);
	    else lcd.lcdPrintSymbol(' ');
		lcd.lcdPrintSymbol((cnv.tmp100)+48);
		lcd.lcdPrintSymbol('.');
		lcd.lcdPrintSymbol((cnv.tmp10)+48);
		lcd.lcdPrintSymbol((cnv.tmp1)+48);
		lcd.lcdPrintSymbol(' ');
	    lcd.lcdPrintSymbol('A');lcd.lcdPrintSymbol('h');	
	  break;
	  case printEnergy: len=sizeof(buffEnergy); lcd.display=none;
		 if(cnv.energy>1000){ cnv.rezult = (cnv.energy*10); lcd.flagPoint=true; }
		 else if(cnv.energy>100){ cnv.rezult = (cnv.energy*100); lcd.flagPoint=false; }
		 else if(cnv.energy<10) cnv.rezult = (cnv.energy*10000);
		 else cnv.rezult = (cnv.energy*1000);
		 cnv.tmp10000 = cnv.rezult/10000;
		 cnv.tmp1000 = (cnv.rezult/1000)%10;
		 cnv.tmp100 = (cnv.rezult/100)%10;
		 cnv.tmp10 = (cnv.rezult/10)%10;
		 cnv.tmp1 = cnv.rezult%10;
		 lcd.lcdSetLine(lcdLine4); lcd.lcdPrint(buffEnergy,len);		
		 lcd.lcdPrintSymbol((cnv.tmp10000)+48);
		 if(cnv.energy<10) lcd.lcdPrintSymbol('.');	
		 lcd.lcdPrintSymbol((cnv.tmp1000)+48);
		 if((cnv.energy>10)&&(cnv.energy<100)) lcd.lcdPrintSymbol('.');
		 lcd.lcdPrintSymbol((cnv.tmp100)+48);
		 if(cnv.energy>100) lcd.lcdPrintSymbol('.');
		 lcd.lcdPrintSymbol((cnv.tmp10)+48);
		 lcd.lcdPrintSymbol((cnv.tmp1)+48);
		 lcd.lcdPrintSymbol(' ');
		 lcd.lcdPrintSymbol('W');lcd.lcdPrintSymbol('h'); 
	  break;
	  case printLowVoltage: len=sizeof(buffLowVoltage); lcd.display=none;
	     if(lcd.flagClear){ lcd.clearDisplay(); lcd.flagClear=false; } 
		 lcd.lcdSetLine(lcdLine3); lcd.lcdPrint(buffLowVoltage,len);
	  break;
   }//end switch
    	 
 } //end while 
} //end main

