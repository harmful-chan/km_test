#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  

	  		  	
extern u32 RS485_RX_BUF[256]; 		//接收缓冲,最大64个字节
extern u32 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN		PBout(9)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART3_RX 	1			//0,不接收;1,接收.



void RS485_Init(u32 bound);
void RS485_Send_Data(char *buf,u8 len);
void RS485_Receive_Data(char *buf,u32 *len);

int RS485_Com(u8 *send, u8 *recv);
int RS485_TXStr(u8 *send);
int RS485_RXStr(u8 *recv);
void RS485_TX(u8 send);

#endif	   
















