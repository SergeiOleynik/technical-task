/*
 *								 lcd.h
 *								 Created: 17.02.2024 
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "main.h"

typedef          unsigned char         uch;
typedef          unsigned int          uint;

enum{
	lcdSendData,
	lcdSendCommand,
	lcdLine1,
	lcdLine2,
	lcdLine3,
	lcdLine4,
	printVoltage,
	printCurrent,
	printCapacity,
	printEnergy,
	none,
	printLowVoltage	
};

#define			lcdPORT				PORTB
#define			lcdDDR				DDRB
#define			lcdSck				0
#define			lcdData				1
#define			lcdE				2
#define			lcdRS				3

#define			lcdSckLOW			lcdPORT &= ~(1<<lcdSck)
#define			lcdSckHIGH			lcdPORT |=  (1<<lcdSck)

#define			lcdDataLOW			lcdPORT &= ~(1<<lcdData)
#define			lcdDataHIGH			lcdPORT |=  (1<<lcdData)

#define			lcdELOW				lcdPORT &= ~(1<<lcdE)
#define			lcdEHIGH			lcdPORT |=  (1<<lcdE)

#define			lcdRsLOW			lcdPORT &= ~(1<<lcdRS)
#define			lcdRsHIGH			lcdPORT |=  (1<<lcdRS)

#define         lcdFunctionSet      0x3C          
#define         lcdEntryModeInc     0x06          // set Increment
#define         lcdOn               0x0C          // lcd on
#define         lcdClear            0x01          // clear lcd

#define         lcdString1          0x80
#define         lcdString2          0xC0
#define         lcdString3          0x90
#define         lcdString4          0xD0

class LCD{
	
 public:
 
 LCD(); 
 bool flagClear=true,
	  flagPoint=true;
 uch  display; 
 void lcdPrint(uch*,uch length);
 void lcdSetLine(uch lcdLine);
 void lcdPrintSymbol(uch data);
 void lcdSetPosition(uch position);
 void clearDisplay();

 private:
 void spiMasterTransmit(uch data,uch direction);
 void lcdDelay(); 

};

#endif /* LCD_H_ */