#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  

	  		  	
extern u32 RS485_RX_BUF[256]; 		//���ջ���,���64���ֽ�
extern u32 RS485_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define RS485_TX_EN		PBout(9)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART3_RX 	1			//0,������;1,����.



void RS485_Init(u32 bound);
void RS485_Send_Data(char *buf,u8 len);
void RS485_Receive_Data(char *buf,u32 *len);

int RS485_Com(u8 *send, u8 *recv);
int RS485_TXStr(u8 *send);
int RS485_RXStr(u8 *recv);
void RS485_TX(u8 send);

#endif	   
















