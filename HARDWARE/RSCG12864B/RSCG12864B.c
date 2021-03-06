#include <RSCG12864B.H>
/***********************************************************************
*函数原型:void BUSY(void)
*功    能:BUSY判断
************************************************************************/
void BUSY(void)
{
    while(GPIO_ReadInputDataBit(BUSYGPIO,BUSYPIN));
}
/***********************************************************************
*函数原型:void I2C_Start(void)
*功    能:提供I2C时序的起始位
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
*函数原型:void I2C_Stop(void)
*功    能:提供I2C时序的停止位
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
*函数原型:void I2C_Init(void)
*功    能:提供I2C初始化，在调用其它函数之前，在Main中首先调用此函数
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
*函数原型:bit I2C_Clock(void)
*功    能:提供I2C时钟信号,并返回时钟为高电平期间SDA的状态
*************************************************************************/
void I2C_Clock(void)
{
	SCLH;
	delay_us(10);
	SCLL;
	delay_us(10);
}

/************************************************************************
*函数原型:I2C_Send(U8 address,U8 d)
*功    能:发送8位数据，并请求一个应答信号ACK.如果收到ACK应答，返回1，否则0
*************************************************************************/
void I2C_Send(U8 d)
{
	U8 i;
	//发送8位数据
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
*函数原型:void I2C_ACK(void)
*功    能:向总线上发送一个ACK信号，一般用于数据读取时
*************************************************************************/
void I2C_ACK(void)
{
	SDAL;
    I2C_Clock();
	SDAH;
}

/***********************************************************************
*函数原型:reset(U8 address)
*复位
***********************************************************************/
void reset(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x01);
	I2C_Stop();
}

/***********************************************************************
*函数原型:clear(U8 address)
*清屏
***********************************************************************/
void clear(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x10);
	I2C_Stop();
}

/***********************************************************************
*函数原型:display_on(U8 address)
*打开显示，开机默认状态
***********************************************************************/
void display_on(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x11);
	I2C_Stop();
}

/***********************************************************************
*函数原型:display_off(U8 address)
*关闭显示
***********************************************************************/
void display_off(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x12);
	I2C_Stop();
}

/***********************************************************************
*函数原型:brightness(U8 address,U8 n)
*亮度设置
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
*函数原型:draw_pixel(U8 address,U8 x,U8 y)
*绘制一个点
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
*函数原型:delete_pixel(U8 address,U8 x,U8 y)
*删除点
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
*函数原型:draw_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*绘制直线
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
*函数原型:delete_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*删除一条直线
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
*函数原型:draw_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*绘制矩形
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
*函数原型:delete_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*删除矩形
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
*函数原型:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*绘制填充矩形
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
*函数原型:delete_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*删除填充矩形
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
*函数原型:draw_circle(U8 address,U8 x,U8 y,U8 r)
*在制定位置绘制一个圆�
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
*函数原型:delete_circle(U8 address,U8 x,U8 y,U8 r)
*删除圆
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
*函数原型:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*在制定位置绘制一个填充圆�
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
*函数原型:delete_fill_circle(U8 address,U8 x,U8 y,U8 r)
*在制定位置删除一个填充圆
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
*函数原型:inversely_area(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*指定区域反白显示
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
*函数原型:display_bmp(U8 address,U16 n)
*整张显示预存图片
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
*函数原型:display_bmp_block(U8 address,U16 n,U8 px,U8 py,U8 xU8 y,U8 w,U8 h)
*块显示预存图片
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
*函数原型:cursor_p(U8 address,U8 x,U8 y)
*在当前位置显示字符串*ptr,支持全角和半角混合显示
参数：x:0~127  y:0~7
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
*函数原型:spacing(U8 address,U8 xs,U8 ys)
*在当前位置显示字符串*ptr,支持全角和半角混合显示
参数：xs:0~127  ys:0~63
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
*函数原型:print_U32_8x16_P(U8 address,U32 n)
*在当前位置显示长整型变量n
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
*函数原型:print_U32_8x16_xy_p(U8 address,U8 x,U8 y,U32 n)
*在指定位置显示长整型变量n
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
*函数原型:print_string_8x16_p(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持8*16ASCII
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
*函数原型:print_string_8x16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持8*16ASCII
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
*函数原型:print_string_16_p(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_string_16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_U32_5x7_P(U8 address,U32 n)
*在当前位置显示长整型变量n
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
*函数原型:print_U32_5x7_xy_P(U8 address,U8 x,U8 y,U32 n)
*在指定位置显示长整型变量n
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
*函数原型:print_string_5x7_P(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持5*7ASCII
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
*函数原型:print_string_5x7_xy_P(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持5*7ASCII
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

//---------以下显示函数坐标纵向和横向都以像素点位单位定义---------------
/***********************************************************************
*函数原型:cursor(U8 address,U8 x,U8 y)
*设置字符显示位置
参数：x:0~127  y:0~63
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
*函数原型:spacing(U8 address,U8 xs,U8 ys)
*设置字符间距
参数：xs:0~127  ys:0~63
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
*函数原型:print_U32_5x7(U8 address,U32 n)
*在当前位置显示长整型变量n
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
*函数原型:print_U32_6x12(U8 address,U32 n)
*在当前位置显示长整型变量n
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
*函数原型:print_U32_8x16(U8 address,U32 n)
*在当前位置显示长整型变量n
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
*函数原型:print_U32_5x7_xy(U8 address,U8 x,U8 y,U32 n)
*在指定位置显示长整型变量n
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
*函数原型:print_U32_6x12_xy(U8 address,U8 x,U8 y,U32 n)
*在指定位置显示长整型变量n
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
*函数原型:print_U32_8x16_xy(U8 address,U8 x,U8 y,U32 n)
*在指定位置显示长整型变量n
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
*函数原型:print_string_12(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_string_16(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_string_12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_string_16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持全角和半角混合显示
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
*函数原型:print_string_5x7(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持5*7ASCII
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
*函数原型:print_string_6x12(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持6*12ASCII
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
*函数原型:print_string_8x16(U8 address,U8 *ptr)
*在当前位置显示字符串*ptr,支持8*16ASCII
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
*函数原型:print_string_5x7_xy(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持5*7ASCII
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
*函数原型:print_string_6x12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持6*12ASCII
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
*函数原型:print_string_8x16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*在指定位置显示字符串*ptr,支持8*16ASCII
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



