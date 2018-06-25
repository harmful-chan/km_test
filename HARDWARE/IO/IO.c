#include "IO.h"
#include "delay.h"


//IO初始化
void IO_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);									    //使能PA端口时钟

//BUZ	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;												    //BUZ-->PA.4 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 									//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 									//IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 									//根据设定参数初始化GPIOA.5
 GPIO_ResetBits(GPIOA,GPIO_Pin_4);						 									//PA4 输出高
	
//7.2v电源控制	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;												    //P72-->PA.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 									//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 									//IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 									//根据设定参数初始化GPIOA.5
 GPIO_SetBits(GPIOA,GPIO_Pin_5);						 									//PA4 输出高	
	
//PLUSE	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 									 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 									 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);													     //根据设定参数初始化
 PI1=0;PI2=0;PI3=0;PI4=0;
	
//DIGITAL
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 									 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;											 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 									 //根据设定参数初始化
 //GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //输出低	
 
}





//蜂鸣器结束提醒
void Buz_Out_Over(void)
{
	BUZ=1;
	delay_ms(100);
	BUZ=0;
	delay_ms(100);
	BUZ=1;
	delay_ms(100);
	BUZ=0;
	delay_ms(100);
	BUZ=1;
	delay_ms(100);
	BUZ=0;
}


//蜂鸣器操作提醒
void Buz_Out(void)
{
	BUZ=1;
	delay_ms(500);
	BUZ=0;

}



//脉冲输出
 void PI_Out(void)
{
	PI1=0;PI2=0;PI3=0;PI4=0;//DI1=0;DI2=0;DI3=0;DI4=0;
	delay_ms(100);          //
	PI1=1;PI2=1;PI3=1;PI4=1;//DI1=1;DI2=1;DI3=1;DI4=1;
	delay_ms(100);          //
	PI1=0;PI2=0;PI3=0;PI4=0;//DI1=0;DI2=0;DI3=0;DI4=0;
	delay_ms(100);          //
	PI1=1;PI2=1;PI3=1;PI4=1;//DI1=1;DI2=1;DI3=1;DI4=1;
	delay_ms(100);          //
	PI1=0;PI2=0;PI3=0;PI4=0;//DI1=0;DI2=0;DI3=0;DI4=0;
	delay_ms(100);          //
	PI1=1;PI2=1;PI3=1;PI4=1;//DI1=1;DI2=1;DI3=1;DI4=1;	
    delay_ms(100);	
	PI1=0;PI2=0;PI3=0;PI4=0;//DI1=0;DI2=0;DI3=0;DI4=0;
	delay_ms(100);          
	PI1=1;PI2=1;PI3=1;PI4=1;
}

