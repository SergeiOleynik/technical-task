/*
 *									 lcd.cpp
 *									 Created: 17.02.2024 
 */ 

#include "lcd.h"

LCD::LCD(){
 lcdDDR |= (1<<lcdSck)|(1<<lcdData)|(1<<lcdE)|(1<<lcdRS);	
 spiMasterTransmit(lcdFunctionSet,lcdSendCommand);
 spiMasterTransmit(lcdEntryModeInc,lcdSendCommand);
 spiMasterTransmit(lcdOn,lcdSendCommand);
 spiMasterTransmit(lcdClear,lcdSendCommand);	
}

void LCD::lcdPrint(uch* buff,uch length){
  for(uch i=0;i<length;i++) spiMasterTransmit(buff[i],lcdSendData);   
}

void LCD::lcdSetLine(uch lcdLine){
  uch line=0;	
  switch(lcdLine){
	case lcdLine1: line=lcdString1; break;
	case lcdLine2: line=lcdString2; break;
	case lcdLine3: line=lcdString3; break;
	case lcdLine4: line=lcdString4; break;   
  }
  spiMasterTransmit(line,lcdSendCommand);	
}

void LCD::lcdPrintSymbol(uch data){
   spiMasterTransmit(data,lcdSendData); 	
}
void LCD::clearDisplay(){
   spiMasterTransmit(lcdClear,lcdSendCommand);	
}

/* Program spi
 * input  parameter:  dataSpi
 * Spi Port:  spiSck - clock, spiData - data, spiSS - strobe					   
 */						 
void LCD::spiMasterTransmit(uch data,uch direction){
 	
 switch(direction){
  case lcdSendCommand: lcdRsLOW; break;
  case lcdSendData:    lcdRsHIGH; break;	 
 }	
 
 for(uch i = 0; i < 8; i++){
  if(data>=0x80) lcdDataHIGH;   
  else  lcdDataLOW;                	  			                
  lcdSckHIGH;                                           
  asm("nop"); asm("nop"); asm("nop"); 
  asm("nop"); asm("nop"); asm("nop"); 
  lcdSckLOW;			              
  data <<= 1;	            
  } 
  
  lcdDelay();
  lcdEHIGH;
  asm("nop"); asm("nop"); asm("nop");
  asm("nop"); asm("nop"); asm("nop");
  lcdELOW;
    
}
void LCD::lcdDelay(){
  _delay_ms(3);	
}
