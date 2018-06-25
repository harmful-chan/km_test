#ifndef __IO_H
#define __IO_H	 
#include "sys.h"


#define BUZ PAout(4)
#define P72 PAout(5) 

#define PI1  PAout(9) 
#define PI2  PAout(10)	
#define PI3  PAout(11)
#define PI4  PAout(12)	

#define DI1  PBout(12) 
#define DI2  PBout(13)	
#define DI3  PBout(14)
#define DI4  PBout(15)	

void IO_Init(void);
void Buz_Out(void);
void Buz_Out_Over(void);
void PI_Out(void);
void DI_Out(void);



		 				    
#endif
