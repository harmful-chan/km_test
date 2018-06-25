#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"


#ifdef EN_USART3_RX   	//���ʹ���˽���


//���ջ����� 	
u32 RS485_RX_BUF[256];  	//���ջ���,���256���ֽ�.
//���յ������ݳ���
u32 RS485_RX_CNT=0;   		  
  
u32 RS485_RX_CNT_100MS = 0; 
void USART3_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 			 
		res =USART_ReceiveData(USART3); 	    //��ȡ���յ�������
		if(RS485_RX_CNT<256)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
	}  											 
} 
#endif										 
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //PB9�˿�����  //EN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	           //PB10  //TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	   //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			   //PB11  //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);  //��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE); //ֹͣ��λ
 
	
 #ifdef EN_USART3_RX		  					//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound; //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); ; //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //ʹ�ܴ���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

 #endif

  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
 
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(char *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(char *buf,u32 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}



/*
* ���ͽ���ͬʱ����
* return :
*  >0 �����ַ���Ŀ
*   0 ���ͽ��ճɹ�
*  -1 ���� 0byte
*  -2 ���ͳ���255byte
*  -3 ����0byte
*  -4 ���ܳ���255byte
*/
int RS485_Com(u8 *send, u8 *recv)
{
	int ret = 0;
	
	/* �����ַ��� */
	ret = RS485_TXStr(send);
	if(ret == 0) 
		return -1;
	else if(ret > 255) 
		return -2;
	
	
	/* �����ַ��� */
	ret = RS485_RXStr(recv);
	if(ret == 0) 
		return -3;
	else if(ret > 255) 
		return -4;
	
	return ret;    //���ؽ����ַ�����
}
	
/*
* �����ַ�������'\0'��β
* ���� : >=0 �����ַ�����
*/
int RS485_TXStr(u8 *send)
{
	u16 sendCount = 0;
	RS485_TX_EN = 1;
	
	while(*send != '\0' && sendCount <= 255)
	{
		
		RS485_TX(*send++);
		sendCount++;
	}
	RS485_TX_EN = 0;
	return sendCount;    //���ͳ���
}

/*
* �����ַ���
* ���� : >=0 �����ַ�����
*          0 ����10ms���ܲ�������       
*/
int RS485_RXStr(u8 *recv)
{
	u16 recvCount = 0;
	u8 i;
	u16 nCount = 0;
	
	//RS485_TX_EN = 0;
	
	/* ��7�뱨�� */
	while(RS485_RX_CNT == 0 && nCount++ < 70)
	{
		delay_ms(100);
	}
		

	/* �жϽ����Ƿ���� */
	do
	{
		recvCount = RS485_RX_CNT;
		delay_ms(500);
	}while(recvCount != RS485_RX_CNT);
	RS485_RX_CNT = 0;
	
	for(i = 0; i < recvCount; i++)
		recv[i] = RS485_RX_BUF[i];
	recv[i] = '\0';    //��β��'0'
	
	//RS485_TX_EN = 1;

	
	return recvCount;
}

void RS485_TX(u8 send)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
	USART_SendData(USART3,send);	
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
}
