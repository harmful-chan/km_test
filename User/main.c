#include "config.h" 

#if defined(NWE_PROESS) && !defined(OLD_PROESS)
#include "funn.h"
#include "rs485.h"
#include "adc.h"
#include "IO.h"
#include "display.h"

void Check_Page1(void);
void Check_Page2(void);
void Check_Page3(void);
void Check_Page4(void);


int main(void)
{			

	
	/*��ʼ��*/ 
	delay_init();	    													     			  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	SystemInit ();
	IO_Init();
	Adc_Init();		  								                 
	RS485_Init(9600);	   							               	
	DPY_Init();
	
	/* 1----Test Power----4 */
	Check_Page1();
	
	/* 2-Test Statmeter O-4 */
	Check_Page2();
	
	/* 3-Test Statmeter T-4 */
	Check_Page3();
	
	/* 4--Error Message---4 */
	Check_Page4();

	while(1)
	{
		
	}
				
}

//++++++++++++++++++++++++++++++++�������++++++++++++++++++++++++++++++++++++++++++
/*
* ��ʾ���棺
*     Power 7.2V       [OK]
*     Power 3.3V       [OK]
*     Wait 3s ...      [OK] 
* ����˵����
*     �������� [OK] ��Ϊ [Fail]
*/
void Check_Page1(void)
{
	PUT_MSG_TIL("1)-   Test Power  -(4");
	Check_72Vand33V();
	PUT_MSG_CLEAR();
}

/*
* ��ʾ���棺
*     RS485 Com        [OK]
*     Init PADC
*     Check PAD        [OK]
*     Power   5V       [OK]
*     Read Clock       [OK]
* ����˵����
*     �������� [OK] ��Ϊ [Fail]
*/
void Check_Page2(void)
{
	PUT_MSG_TIL("2)-  Test Stat x1 -(4");
	Check_Rs485andSetPass();
	Check_5VandPassValue();
	Check_ReadClock();
	PUT_MSG_CLEAR();
}

/*
* ��ʾ���棺
*     Reboot           [OK]
*     Read Clock       [OK]
*     Device Sleep     [OK]    
* ����˵����
*     �������� [OK] ��Ϊ [Fail]
*/
void Check_Page3(void)
{
	PUT_MSG_TIL("3)-  Test Stat x2 -(4");
	Check_Reboot();
	Check_ReadClockAgain();
	Check_Sleep();
	PUT_MSG_CLEAR();
}

/*
* ��ʾ���棺
*     [���ɴ�����Ϣ]    
* ����˵����
*     �������� [OK] ��Ϊ [Fail]
*/
void Check_Page4(void)
{
	PUT_MSG_TIL("4)- Error Message -(4");
	Check_ErrorMsg();
	Buz_Out_Over();
}



 
#elif defined(OLD_PROESS) && !defined(NWE_PROESS)
#include "usart.h"	 
#include "rs485.h"
#include "adc.h"
#include "string.h"
#include "IO.h"
#include "delay.h"
#include "stm32f10x_adc.h"
#include "RSCG12864B.h"
			
int main(void)
{	
	char *p=NULL;
	u16 adcx1;
	u16 adcx2; 
	u16 adcx3;
	u8 ERROR=0;
	u32 len;
	u8 Over=0;
	u8 i=0;
	char tx_rs485buf1[42]={"666666#Q:7777000000000000P1:0P2:0P3:0P4:0#"};	       //��������ͨ��������������ָ��
	char tx_rs485buf2[9]= {"222222#Z#"}; 							               //��ѯ����ָ��
	char tx_rs485buf3[10]={"666666#C?#"};									           //ʱ���ѯָ��
	char tx_rs485buf4[11]={"666666#G:1#"};								           //���̽���͹���ָ��ر�A1��A2ͨ��ָ��
	char rx_rs485buf[256];  
	char Time1[22];
	char Time2[22];
	char A[50];
	char P[50];
	char D[50];
	
	/*��ʼ��*/ 
	delay_init();	    													     			  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	IO_Init();
	Adc_Init();		  								                 
	RS485_Init(9600);	   							               	
	SystemInit ();
	I2C_Initial();
	delay_ms(1000);
	brightness(0,0xff);
	clear(0x00);
	//print_string_5x7_xy(0x00,0,55,"start");
	print_string_5x7_xy(0x00,2,55,">>>");
	print_string_5x7_xy(0x00,2,43,"10%");
	print_string_5x7_xy(0x00,85,43,"TESTING");

	/*ADC��ⱻ����Դģ����*/		
	//7.2v��Դ���	
	adcx1=Get_Adc_Average(ADC_Channel_1,10);	
	if(adcx1<0xe39&&adcx1>0xdd5);
	else{ print_string_5x7_xy(0x00,50,43,"7.2V ");ERROR=1;}
	//3.3v��Դ���		
	adcx3=Get_Adc_Average(ADC_Channel_3,10);	
	if(adcx3<0x83e&&adcx3>0x7c2);
	else{ print_string_5x7_xy(0x00,75,43,"3.3V");ERROR=1;}
	if(1 == ERROR)													
	{				
		print_string_5x7_xy(0x00,102,43,"OVER");	
		print_string_5x7_xy(0x00,28,55,">>>>>>>>>>>>>>>>");         //ֱ���˳�����
		Buz_Out_Over();									   					   	            //����������ʾ����	
		while(1);
	}
		for(i=0;i<3;i++)
	{
		delay_ms(1000);
	}
	clear(0x00);
	print_string_5x7_xy(0x00,2,43,"20%");
	print_string_5x7_xy(0x00,85,43,"TESTING");
	print_string_5x7_xy(0x00,2,55,">>>");
	print_string_5x7_xy(0x00,15,55,">>");
	RS485_Send_Data(tx_rs485buf1,42);					     						    //��������ͨ��������������ָ��

	while(1)
	{
		RS485_Receive_Data(rx_rs485buf,&len);
		if(len)																   				  				  //���յ�������
		{				
			switch(Over)
			{
				case 0:
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"30%");
					print_string_5x7_xy(0x00,85,43,"TESTING");
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					p=strstr(rx_rs485buf,"Q:7777000000000000;P1:0;P2:0;P3:0;P4:0");
					if(p == NULL)
					{
						print_string_5x7_xy(0x00,75,43,"485");
						print_string_5x7_xy(0x00,102,43,"OVER");	
						print_string_5x7_xy(0x00,28,55,">>>>>>>>>>>>>>>>"); //ֱ���˳�����
						Buz_Out_Over();									   					       //����������ʾ����	
						Over=4;
						break;
					}
					memset(rx_rs485buf,0,sizeof(rx_rs485buf));            //��ջ���
					PI_Out(); 
					RS485_Send_Data(tx_rs485buf2,9);           				   //���ͻ�ȡ���ݵ�ָ��
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"40%");
					print_string_5x7_xy(0x00,85,43,"TESTING");
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					print_string_5x7_xy(0x00,41,55,">>");
					delay_ms(100);
					adcx2=Get_Adc_Average(ADC_Channel_2,10);   				   //adc����5v
					if(adcx2<0xc5d&&adcx2>0xbe0);
					else{ print_string_5x7_xy(0x00,50,39,"5V ");}
					Over=1;
					break;        
				case 1:
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"50%");
					print_string_5x7_xy(0x00,85,43,"TESTING");
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					print_string_5x7_xy(0x00,41,55,">>");
					print_string_5x7_xy(0x00,55,55,">>");					 
					//ģ����
					p=strstr(rx_rs485buf,"A1:0.37");
					if(p != NULL);
					else{ strcat(A,"  A1   ");}
					p=strstr(rx_rs485buf,"A2:0.37");
					if(p != NULL);
					else{ strcat(A,"A2   ");}
					p=strstr(rx_rs485buf,"A3:0.14");
					if(p != NULL);
					else{ strcat(A,"A3   ");}
					p=strstr(rx_rs485buf,"A4:0.14");
					if(p != NULL);
					else{ strcat(A,"A4");}			
					//������
					p=strstr(rx_rs485buf,"P1:00000004");
					if(p == NULL)
					{ strcat(P,"  P1   ");}
					p=strstr(rx_rs485buf,"P2:00000004");
					if(p == NULL)
					{ strcat(P,"P2   ");}
					p=strstr(rx_rs485buf,"P3:00000004");
					if(p == NULL)
					{ strcat(P,"P3   ");}
					p=strstr(rx_rs485buf,"P4:00000004");
					if(p == NULL)
					{ strcat(P,"P4");}					
					//������
					p=strstr(rx_rs485buf,"K1:0");				
					if(p == NULL)
					{ strcat(D,"  D1   ");}
					p=strstr(rx_rs485buf,"K2:0");
					if(p == NULL)
					{ strcat(D,"D2   ");}
					p=strstr(rx_rs485buf,"K3:0");
					if(p == NULL)
					{ strcat(D,"D3   ");}
					p=strstr(rx_rs485buf,"K4:0");
					if(p == NULL)
					{ strcat(D,"D4");}
					memset(rx_rs485buf,0,sizeof(rx_rs485buf));    //��ջ���
					RS485_Send_Data(tx_rs485buf3,10);             //���͵�һ�β�ѯʱ��ָ��
					Over=2;
					break;			
				case 2:
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"60%");
					print_string_5x7_xy(0x00,85,43,"TESTING");
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					print_string_5x7_xy(0x00,41,55,">>");
					print_string_5x7_xy(0x00,55,55,">>");
					print_string_5x7_xy(0x00,67,55,">>");
					//�����һ��ʱ�䣬�ϵ�����
					p=strstr(rx_rs485buf,"C:");
					if(p != NULL)
					{strcat(Time1,rx_rs485buf);}
					P72=0;																	       //�Ͽ�7.2v����
					delay_ms(1000);
					delay_ms(1000);
					P72=1;																	       //�ָ�7.2v����
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"70%");
					print_string_5x7_xy(0x00,85,43,"TESTING");
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					print_string_5x7_xy(0x00,41,55,">>");
					print_string_5x7_xy(0x00,55,55,">>");
					print_string_5x7_xy(0x00,67,55,">>");
					print_string_5x7_xy(0x00,80,55,">>");
					for(i=0;i<5;i++)
					{
						delay_ms(1000);
					}
					clear(0x00);
					print_string_5x7_xy(0x00,2,43,"80%");
					print_string_5x7_xy(0x00,85,43,"TESTING");	
					print_string_5x7_xy(0x00,2,55,">>>");
					print_string_5x7_xy(0x00,15,55,">>");
					print_string_5x7_xy(0x00,28,55,">>");
					print_string_5x7_xy(0x00,41,55,">>");
					print_string_5x7_xy(0x00,55,55,">>");
					print_string_5x7_xy(0x00,67,55,">>");
					print_string_5x7_xy(0x00,80,55,">>");						
					print_string_5x7_xy(0x00,93,55,">>");
					memset(rx_rs485buf,0,sizeof(rx_rs485buf));     //��ջ���
					RS485_Send_Data(tx_rs485buf3,10);              //���͵ڶ��β�ѯʱ��ָ��
					Over=3;
					break;		
				case 3:
					p=strstr(rx_rs485buf,"C:");
					if(p != NULL)
					{
						strcat(Time2,rx_rs485buf);
						if(!strcmp(Time1,Time2))
						{	print_string_5x7_xy(0x00,75,43,"TIME");}
						RS485_Send_Data(tx_rs485buf4,11);					    //���ͽ���͹���ָ��
						Buz_Out_Over();
						clear(0x00);
						print_string_5x7_xy(0x00,2,43,"100%");
						print_string_5x7_xy(0x00,2,55,">>>");
						print_string_5x7_xy(0x00,15,55,">>");
						print_string_5x7_xy(0x00,28,55,">>");
						print_string_5x7_xy(0x00,41,55,">>");
						print_string_5x7_xy(0x00,55,55,">>");
						print_string_5x7_xy(0x00,67,55,">>");
						print_string_5x7_xy(0x00,80,55,">>");						
						print_string_5x7_xy(0x00,93,55,">>");
						print_string_5x7_xy(0x00,0,2,A);
						print_string_5x7_xy(0x00,0,16,P);
						print_string_5x7_xy(0x00,0,30,D);
						print_string_5x7_xy(0x00,102,43,"OVER");							
						print_string_5x7_xy(0x00,106,55,">>>");
						Over=4;
						break;
					}	
				default : 
					break; 
			}				
		}
	}					
} 

#else
  #error "<main.c> You have define NEW_PROESS and OLD_PROESS at the same time !"
#endif
