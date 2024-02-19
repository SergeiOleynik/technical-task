/*
 *								Created: 17.02.2024 
 */


#ifndef __convert_H__
#define __convert_H__ 

#include "main.h"

typedef          unsigned char         uch;
typedef          unsigned int          uint;
typedef			 unsigned long		   ulg;
 
enum{
 
 measurementVoltage,
 measurementCurrent
  
};  

class CONVERT{	
	
 public:
   
 CONVERT(); 
 bool  flagLowVoltage=true;
 uch   tmp10000,
       tmp1000,
	   tmp100,
       tmp10,
       tmp1,
	   tmpForPrint,
	   prescaller;
 ulg   rezult;	   	   	   
 float voltage,
       current,
	   capacity,
	   energy;
	   
  
}; //convert

#endif //__convert_H__
