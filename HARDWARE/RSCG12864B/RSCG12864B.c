#include <RSCG12864B.H>
/***********************************************************************
*����ԭ��:void BUSY(void)
*��    ��:BUSY�ж�
************************************************************************/
void BUSY(void)
{
    while(GPIO_ReadInputDataBit(BUSYGPIO,BUSYPIN));
}
/***********************************************************************
*����ԭ��:void I2C_Start(void)
*��    ��:�ṩI2Cʱ�����ʼλ
************************************************************************/
void I2C_Start(void)
{
  
	delay_ms(100);
	SCLH;
	delay_us(50);
	SDAL;
	delay_us(50);
    SCLL;
	delay_us(50);
}

/***********************************************************************
*����ԭ��:void I2C_Stop(void)
*��    ��:�ṩI2Cʱ���ֹͣλ
************************************************************************/
void I2C_Stop(void)
{
	SDAL;
	delay_us(50);
	SCLH;
	delay_us(50);
	SDAH;
	delay_us(50);
	SCLL;
	delay_us(50);
}

/***********************************************************************
*����ԭ��:void I2C_Init(void)
*��    ��:�ṩI2C��ʼ�����ڵ�����������֮ǰ����Main�����ȵ��ô˺���
************************************************************************/
void I2C_Initial(void)
{
    GPIO_InitTypeDef IO; 
    IO.GPIO_Pin = SCLPIN;            
    IO.GPIO_Speed =GPIO_Speed_10MHz;
    IO.GPIO_Mode =GPIO_Mode_Out_PP; 
    GPIO_Init(SCLGPIO, &IO);   //SCL IO 
    IO.GPIO_Pin = SDAPIN; 
    GPIO_Init(SDAGPIO, &IO);   // SDA IO
    IO.GPIO_Pin = BUSYPIN;            
    IO.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(BUSYGPIO, &IO); //BUSY
	SCLL;
	I2C_Stop();
}

/************************************************************************
*����ԭ��:bit I2C_Clock(void)
*��    ��:�ṩI2Cʱ���ź�,������ʱ��Ϊ�ߵ�ƽ�ڼ�SDA��״̬
*************************************************************************/
void I2C_Clock(void)
{
	SCLH;
	delay_us(10);
	SCLL;
	delay_us(10);
}

/************************************************************************
*����ԭ��:I2C_Send(U8 address,U8 d)
*��    ��:����8λ���ݣ�������һ��Ӧ���ź�ACK.����յ�ACKӦ�𣬷���1������0
*************************************************************************/
void I2C_Send(U8 d)
{
	U8 i;
	//����8λ����
	for(i=0;i<8;i++)
	{
		if(d&0x80)
            SDAH;
        else
            SDAL;
		d=d<<1;
		I2C_Clock();
	}
	I2C_Clock();
}


/************************************************************************
*����ԭ��:void I2C_ACK(void)
*��    ��:�������Ϸ���һ��ACK�źţ�һ���������ݶ�ȡʱ
*************************************************************************/
void I2C_ACK(void)
{
	SDAL;
    I2C_Clock();
	SDAH;
}

/***********************************************************************
*����ԭ��:reset(U8 address)
*��λ
***********************************************************************/
void reset(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x01);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:clear(U8 address)
*����
***********************************************************************/
void clear(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x10);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:display_on(U8 address)
*����ʾ������Ĭ��״̬
***********************************************************************/
void display_on(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x11);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:display_off(U8 address)
*�ر���ʾ
***********************************************************************/
void display_off(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x12);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:brightness(U8 address,U8 n)
*��������
***********************************************************************/
void brightness(U8 address,U8 n)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x13);
	I2C_Send(n);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_pixel(U8 address,U8 x,U8 y)
*����һ����
***********************************************************************/
void draw_pixel(U8 address,U8 x,U8 y)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x30);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_pixel(U8 address,U8 x,U8 y)
*ɾ����
***********************************************************************/
void delete_pixel(U8 address,U8 x,U8 y)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x31);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*����ֱ��
***********************************************************************/
void draw_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x32);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*ɾ��һ��ֱ��
***********************************************************************/
void delete_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x33);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*���ƾ���
***********************************************************************/
void draw_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x34);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*ɾ������
***********************************************************************/
void delete_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x35);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*����������
***********************************************************************/
void draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x36);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*ɾ��������
***********************************************************************/
void delete_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x37);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_circle(U8 address,U8 x,U8 y,U8 r)
*���ƶ�λ�û���һ��Բ�
***********************************************************************/
void draw_circle(U8 address,U8 x,U8 y,U8 r)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x38);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Send(r);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_circle(U8 address,U8 x,U8 y,U8 r)
*ɾ��Բ
***********************************************************************/
void delete_circle(U8 address,U8 x,U8 y,U8 r)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x39);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Send(r);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*���ƶ�λ�û���һ�����Բ�
***********************************************************************/
void draw_fill_circle(U8 address,U8 x,U8 y,U8 r)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x3A);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Send(r);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:delete_fill_circle(U8 address,U8 x,U8 y,U8 r)
*���ƶ�λ��ɾ��һ�����Բ
***********************************************************************/
void delete_fill_circle(U8 address,U8 x,U8 y,U8 r)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x3B);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Send(r);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:inversely_area(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*ָ�����򷴰���ʾ
***********************************************************************/
void inversely_area(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x38);
	I2C_Send(x1);
	I2C_Send(y1);
	I2C_Send(x2);
	I2C_Send(y2);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:display_bmp(U8 address,U16 n)
*������ʾԤ��ͼƬ
***********************************************************************/
void display_bmp(U8 address,U16 n)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x3d);
	I2C_Send(n>>8);
	I2C_Send(n);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:display_bmp_block(U8 address,U16 n,U8 px,U8 py,U8 xU8 y,U8 w,U8 h)
*����ʾԤ��ͼƬ
***********************************************************************/
void display_bmp_block(U8 address,U16 n,U8 px,U8 py,U8 x,U8 y,U8 w,U8 h)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x3E);
	I2C_Send(n>>8);
	I2C_Send(n);
	I2C_Send(x);
	I2C_Send(y);
	I2C_Send(px);
	I2C_Send(py);
	I2C_Send(w);
	I2C_Send(h);
	I2C_Stop();
}


/***********************************************************************
*����ԭ��:cursor_p(U8 address,U8 x,U8 y)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
������x:0~127  y:0~7
***********************************************************************/
void cursor_p(U8 address,U8 x,U8 y)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
    I2C_Send(x);
	I2C_Send(y);
	I2C_Stop();
}
/***********************************************************************
*����ԭ��:spacing(U8 address,U8 xs,U8 ys)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
������xs:0~127  ys:0~63
***********************************************************************/
void x_spacing_p(U8 address,U8 xs)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2A);
    I2C_Send(xs);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_8x16_P(U8 address,U32 n)
*�ڵ�ǰλ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_8x16_p(U8 address,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2B);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_8x16_xy_p(U8 address,U8 x,U8 y,U32 n)
*��ָ��λ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_8x16_xy_p(U8 address,U8 x,U8 y,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x2B);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_8x16_p(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��8*16ASCII
***********************************************************************/
void print_string_8x16_p(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2B);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_8x16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��8*16ASCII
***********************************************************************/
void print_string_8x16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x2B);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_16_p(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_16_p(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2C);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x2C);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_5x7_P(U8 address,U32 n)
*�ڵ�ǰλ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_5x7_P(U8 address,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2D);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_5x7_xy_P(U8 address,U8 x,U8 y,U32 n)
*��ָ��λ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_5x7_xy_P(U8 address,U8 x,U8 y,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x2D);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_5x7_P(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��5*7ASCII
***********************************************************************/
void print_string_5x7_P(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x2D);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_5x7_xy_P(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��5*7ASCII
***********************************************************************/
void print_string_5x7_xy_P(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x29);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x2D);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

//---------������ʾ������������ͺ��������ص�λ��λ����---------------
/***********************************************************************
*����ԭ��:cursor(U8 address,U8 x,U8 y)
*�����ַ���ʾλ��
������x:0~127  y:0~63
***********************************************************************/
void cursor(U8 address,U8 x,U8 y)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
    I2C_Send(x);
	I2C_Send(y);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:spacing(U8 address,U8 xs,U8 ys)
*�����ַ����
������xs:0~127  ys:0~63
***********************************************************************/
void spacing(U8 address,U8 xs,U8 ys)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x21);
    I2C_Send(xs);
	I2C_Send(ys);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_5x7(U8 address,U32 n)
*�ڵ�ǰλ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_5x7(U8 address,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x24);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_6x12(U8 address,U32 n)
*�ڵ�ǰλ����ʾ�����ͱ���n
***********************************************************************/

void print_U32_6x12(U8 address,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x25);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_8x16(U8 address,U32 n)
*�ڵ�ǰλ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_8x16(U8 address,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x26);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_5x7_xy(U8 address,U8 x,U8 y,U32 n)
*��ָ��λ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_5x7_xy(U8 address,U8 x,U8 y,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x24);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_6x12_xy(U8 address,U8 x,U8 y,U32 n)
*��ָ��λ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_6x12_xy(U8 address,U8 x,U8 y,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x25);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_U32_8x16_xy(U8 address,U8 x,U8 y,U32 n)
*��ָ��λ����ʾ�����ͱ���n
***********************************************************************/
void print_U32_8x16_xy(U8 address,U8 x,U8 y,U32 n)
{
    U32 i,temp;
    U8 t=0;
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x26);
	if(n==0)
	{
        I2C_Send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                I2C_Send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                I2C_Send(temp%10+0x30);
            }
		}
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_12(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_12(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x27);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}
/***********************************************************************
*����ԭ��:print_string_16(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_16(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x28);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_12_xy(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x27);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}
/***********************************************************************
*����ԭ��:print_string_16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��ȫ�ǺͰ�ǻ����ʾ
***********************************************************************/
void print_string_16_xy(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x28);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_5x7(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��5*7ASCII
***********************************************************************/
void print_string_5x7(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x24);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_6x12(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��6*12ASCII
***********************************************************************/
void print_string_6x12(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x25);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_8x16(U8 address,U8 *ptr)
*�ڵ�ǰλ����ʾ�ַ���*ptr,֧��8*16ASCII
***********************************************************************/
void print_string_8x16(U8 address,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x26);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_5x7_xy(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��5*7ASCII
***********************************************************************/
void print_string_5x7_xy(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x24);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_6x12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��6*12ASCII
***********************************************************************/
void print_string_6x12_xy(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x25);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}

/***********************************************************************
*����ԭ��:print_string_8x16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*��ָ��λ����ʾ�ַ���*ptr,֧��8*16ASCII
***********************************************************************/
void print_string_8x16_xy(U8 address,U8 x,U8 y,U8 *ptr)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x20);
	I2C_Send(x);
	I2C_Send(y);
    I2C_Send(0x26);
	while(*ptr!='\0')
    {
        I2C_Send(*ptr++);             
    }
    I2C_Send(0x00);
	I2C_Stop();
}



