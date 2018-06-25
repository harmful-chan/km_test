#include <RSCG12864B.H>
/***********************************************************************
*º¯ÊýÔ­ÐÍ:void BUSY(void)
*¹¦    ÄÜ:BUSYÅÐ¶Ï
************************************************************************/
void BUSY(void)
{
    while(GPIO_ReadInputDataBit(BUSYGPIO,BUSYPIN));
}
/***********************************************************************
*º¯ÊýÔ­ÐÍ:void I2C_Start(void)
*¹¦    ÄÜ:Ìá¹©I2CÊ±ÐòµÄÆðÊ¼Î»
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
*º¯ÊýÔ­ÐÍ:void I2C_Stop(void)
*¹¦    ÄÜ:Ìá¹©I2CÊ±ÐòµÄÍ£Ö¹Î»
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
*º¯ÊýÔ­ÐÍ:void I2C_Init(void)
*¹¦    ÄÜ:Ìá¹©I2C³õÊ¼»¯£¬ÔÚµ÷ÓÃÆäËüº¯ÊýÖ®Ç°£¬ÔÚMainÖÐÊ×ÏÈµ÷ÓÃ´Ëº¯Êý
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
*º¯ÊýÔ­ÐÍ:bit I2C_Clock(void)
*¹¦    ÄÜ:Ìá¹©I2CÊ±ÖÓÐÅºÅ,²¢·µ»ØÊ±ÖÓÎª¸ßµçÆ½ÆÚ¼äSDAµÄ×´Ì¬
*************************************************************************/
void I2C_Clock(void)
{
	SCLH;
	delay_us(10);
	SCLL;
	delay_us(10);
}

/************************************************************************
*º¯ÊýÔ­ÐÍ:I2C_Send(U8 address,U8 d)
*¹¦    ÄÜ:·¢ËÍ8Î»Êý¾Ý£¬²¢ÇëÇóÒ»¸öÓ¦´ðÐÅºÅACK.Èç¹ûÊÕµ½ACKÓ¦´ð£¬·µ»Ø1£¬·ñÔò0
*************************************************************************/
void I2C_Send(U8 d)
{
	U8 i;
	//·¢ËÍ8Î»Êý¾Ý
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
*º¯ÊýÔ­ÐÍ:void I2C_ACK(void)
*¹¦    ÄÜ:Ïò×ÜÏßÉÏ·¢ËÍÒ»¸öACKÐÅºÅ£¬Ò»°ãÓÃÓÚÊý¾Ý¶ÁÈ¡Ê±
*************************************************************************/
void I2C_ACK(void)
{
	SDAL;
    I2C_Clock();
	SDAH;
}

/***********************************************************************
*º¯ÊýÔ­ÐÍ:reset(U8 address)
*¸´Î»
***********************************************************************/
void reset(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x01);
	I2C_Stop();
}

/***********************************************************************
*º¯ÊýÔ­ÐÍ:clear(U8 address)
*ÇåÆÁ
***********************************************************************/
void clear(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x10);
	I2C_Stop();
}

/***********************************************************************
*º¯ÊýÔ­ÐÍ:display_on(U8 address)
*´ò¿ªÏÔÊ¾£¬¿ª»úÄ¬ÈÏ×´Ì¬
***********************************************************************/
void display_on(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x11);
	I2C_Stop();
}

/***********************************************************************
*º¯ÊýÔ­ÐÍ:display_off(U8 address)
*¹Ø±ÕÏÔÊ¾
***********************************************************************/
void display_off(U8 address)
{
	I2C_Start();
    I2C_Send(address);
    I2C_Send(0x12);
	I2C_Stop();
}

/***********************************************************************
*º¯ÊýÔ­ÐÍ:brightness(U8 address,U8 n)
*ÁÁ¶ÈÉèÖÃ
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
*º¯ÊýÔ­ÐÍ:draw_pixel(U8 address,U8 x,U8 y)
*»æÖÆÒ»¸öµã
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
*º¯ÊýÔ­ÐÍ:delete_pixel(U8 address,U8 x,U8 y)
*É¾³ýµã
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
*º¯ÊýÔ­ÐÍ:draw_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*»æÖÆÖ±Ïß
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
*º¯ÊýÔ­ÐÍ:delete_line(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*É¾³ýÒ»ÌõÖ±Ïß
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
*º¯ÊýÔ­ÐÍ:draw_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*»æÖÆ¾ØÐÎ
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
*º¯ÊýÔ­ÐÍ:delete_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*É¾³ý¾ØÐÎ
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
*º¯ÊýÔ­ÐÍ:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*»æÖÆÌî³ä¾ØÐÎ
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
*º¯ÊýÔ­ÐÍ:delete_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*É¾³ýÌî³ä¾ØÐÎ
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
*º¯ÊýÔ­ÐÍ:draw_circle(U8 address,U8 x,U8 y,U8 r)
*ÔÚÖÆ¶¨Î»ÖÃ»æÖÆÒ»¸öÔ²ã
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
*º¯ÊýÔ­ÐÍ:delete_circle(U8 address,U8 x,U8 y,U8 r)
*É¾³ýÔ²
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
*º¯ÊýÔ­ÐÍ:draw_fill_rectangle(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*ÔÚÖÆ¶¨Î»ÖÃ»æÖÆÒ»¸öÌî³äÔ²ã
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
*º¯ÊýÔ­ÐÍ:delete_fill_circle(U8 address,U8 x,U8 y,U8 r)
*ÔÚÖÆ¶¨Î»ÖÃÉ¾³ýÒ»¸öÌî³äÔ²
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
*º¯ÊýÔ­ÐÍ:inversely_area(U8 address,U8 x1,U8 y1,U8 x2,U8 y2)
*Ö¸¶¨ÇøÓò·´°×ÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:display_bmp(U8 address,U16 n)
*ÕûÕÅÏÔÊ¾Ô¤´æÍ¼Æ¬
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
*º¯ÊýÔ­ÐÍ:display_bmp_block(U8 address,U16 n,U8 px,U8 py,U8 xU8 y,U8 w,U8 h)
*¿éÏÔÊ¾Ô¤´æÍ¼Æ¬
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
*º¯ÊýÔ­ÐÍ:cursor_p(U8 address,U8 x,U8 y)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
²ÎÊý£ºx:0~127  y:0~7
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
*º¯ÊýÔ­ÐÍ:spacing(U8 address,U8 xs,U8 ys)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
²ÎÊý£ºxs:0~127  ys:0~63
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
*º¯ÊýÔ­ÐÍ:print_U32_8x16_P(U8 address,U32 n)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_8x16_xy_p(U8 address,U8 x,U8 y,U32 n)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_string_8x16_p(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö8*16ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_8x16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö8*16ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_16_p(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_string_16_xy_p(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_U32_5x7_P(U8 address,U32 n)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_5x7_xy_P(U8 address,U8 x,U8 y,U32 n)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_string_5x7_P(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö5*7ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_5x7_xy_P(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö5*7ASCII
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

//---------ÒÔÏÂÏÔÊ¾º¯Êý×ø±ê×ÝÏòºÍºáÏò¶¼ÒÔÏñËØµãÎ»µ¥Î»¶¨Òå---------------
/***********************************************************************
*º¯ÊýÔ­ÐÍ:cursor(U8 address,U8 x,U8 y)
*ÉèÖÃ×Ö·ûÏÔÊ¾Î»ÖÃ
²ÎÊý£ºx:0~127  y:0~63
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
*º¯ÊýÔ­ÐÍ:spacing(U8 address,U8 xs,U8 ys)
*ÉèÖÃ×Ö·û¼ä¾à
²ÎÊý£ºxs:0~127  ys:0~63
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
*º¯ÊýÔ­ÐÍ:print_U32_5x7(U8 address,U32 n)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_6x12(U8 address,U32 n)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_8x16(U8 address,U32 n)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_5x7_xy(U8 address,U8 x,U8 y,U32 n)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_6x12_xy(U8 address,U8 x,U8 y,U32 n)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_U32_8x16_xy(U8 address,U8 x,U8 y,U32 n)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾³¤ÕûÐÍ±äÁ¿n
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
*º¯ÊýÔ­ÐÍ:print_string_12(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_string_16(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_string_12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_string_16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³ÖÈ«½ÇºÍ°ë½Ç»ìºÏÏÔÊ¾
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
*º¯ÊýÔ­ÐÍ:print_string_5x7(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö5*7ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_6x12(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö6*12ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_8x16(U8 address,U8 *ptr)
*ÔÚµ±Ç°Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö8*16ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_5x7_xy(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö5*7ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_6x12_xy(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö6*12ASCII
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
*º¯ÊýÔ­ÐÍ:print_string_8x16_xy(U8 address,U8 x,U8 y,U8 *ptr)
*ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®*ptr,Ö§³Ö8*16ASCII
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



