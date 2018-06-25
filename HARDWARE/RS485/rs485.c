#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"


#ifdef EN_USART3_RX   	//如果使能了接收


//接收缓存区 	
u32 RS485_RX_BUF[256];  	//接收缓冲,最大256个字节.
//接收到的数据长度
u32 RS485_RX_CNT=0;   		  
  
u32 RS485_RX_CNT_100MS = 0; 
void USART3_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 			 
		res =USART_ReceiveData(USART3); 	    //读取接收到的数据
		if(RS485_RX_CNT<256)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
	}  											 
} 
#endif										 
//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //PB9端口配置  //EN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	           //PB10  //TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	   //复用推挽
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			   //PB11  //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);  //复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE); //停止复位
 
	
 #ifdef EN_USART3_RX		  					//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound; //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

    USART_Init(USART3, &USART_InitStructure); ; //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(USART3, ENABLE);                    //使能串口 

 #endif

  RS485_TX_EN=0;			//默认为接收模式
 
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(char *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	
	RS485_TX_EN=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(char *buf,u32 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}



/*
* 发送接收同时进行
* return :
*  >0 接受字符数目
*   0 发送接收成功
*  -1 发送 0byte
*  -2 发送超过255byte
*  -3 接受0byte
*  -4 接受超过255byte
*/
int RS485_Com(u8 *send, u8 *recv)
{
	int ret = 0;
	
	/* 发送字符串 */
	ret = RS485_TXStr(send);
	if(ret == 0) 
		return -1;
	else if(ret > 255) 
		return -2;
	
	
	/* 接受字符串 */
	ret = RS485_RXStr(recv);
	if(ret == 0) 
		return -3;
	else if(ret > 255) 
		return -4;
	
	return ret;    //返回接收字符长度
}
	
/*
* 发送字符串，以'\0'结尾
* 返回 : >=0 发送字符长度
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
	return sendCount;    //发送长度
}

/*
* 接收字符串
* 返回 : >=0 接受字符长度
*          0 超过10ms接受不到数据       
*/
int RS485_RXStr(u8 *recv)
{
	u16 recvCount = 0;
	u8 i;
	u16 nCount = 0;
	
	//RS485_TX_EN = 0;
	
	/* 超7秒报错 */
	while(RS485_RX_CNT == 0 && nCount++ < 70)
	{
		delay_ms(100);
	}
		

	/* 判断接收是否结束 */
	do
	{
		recvCount = RS485_RX_CNT;
		delay_ms(500);
	}while(recvCount != RS485_RX_CNT);
	RS485_RX_CNT = 0;
	
	for(i = 0; i < recvCount; i++)
		recv[i] = RS485_RX_BUF[i];
	recv[i] = '\0';    //结尾补'0'
	
	//RS485_TX_EN = 1;

	
	return recvCount;
}

void RS485_TX(u8 send)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
	USART_SendData(USART3,send);	
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
}
