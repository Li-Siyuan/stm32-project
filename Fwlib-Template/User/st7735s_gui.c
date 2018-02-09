/*
int main()
{
  	LED_Conf();
	  Site_t site     = {0, 0};                          
		LCD_ST7735S_Conf();
    LCD_ST7735S_str            (site,(u8*)"LCD   Init",FCOLOUR,BCOLOUR);
		LED1_ON;
		while(1);
}
*/


#include "st7735s_gui.h"
//#include "ASCII.h"
//extern unsigned char const ascii_8x16[1536];


/*!
 *  @brief      LCD???
 *  @since      v5.0
 */ 
void LCD_ST7735S_Conf(void)
{
    Site_t site = {0, 0};
    Size_t size ;

    LCD_INIT();                             //???LCD

    //LCD_SET_DIR(LCD_DIR+1);

    //?????????? ???? ??,????????,?????
    size.W = LCD_W;
    size.H = LCD_H;

    LCD_ST7735S_rectangle(site, size, BCOLOUR);     //?????
}

/*!
 *  @brief      ??????
 *  @param      site    ?????
 *  @param      size    ????
 *  @param      rgb565  ??
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        Size_t size = {50,60};  // W = 50 ,H = 60
                        LCD_ST7735S_rectangle(site, size, RED);
 */
void LCD_ST7735S_rectangle(Site_t site, Size_t size, u16 rgb565)
{
    u32 n, temp;

    LCD_PTLON(site, size);              //??

    temp = (u32)size.W * size.H;
    LCD_RAMWR();                        //???
    for(n = 0; n < temp; n++)
    {
        LCD_WR_DATA( rgb565 );          //???
    }
}

/*!
 *  @brief      ??
 *  @param      site    ?????
 *  @param      rgb565  ??
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_ST7735S_point(site, RED);
 */
void LCD_ST7735S_point(Site_t site, u16 rgb565)
{
    Size_t size = {1, 1};
    LCD_PTLON(site, size);
    LCD_RAMWR();                        //???
    LCD_WR_DATA(rgb565);                //???
}
void LCD_ST7735S_points          (Site_t *site,u32 point_num, u16 rgb565)                 //????
{
    while(point_num--)
    {
        LCD_ST7735S_point(site[point_num],rgb565);                 //??
    }
}

/*!
 *  @brief      ????
 *  @param      site    ?????
 *  @param      ascii   ??
 *  @param      Color   ????
 *  @param      bkColor ????
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_ST7735S_char(site,'0', BLUE,RED);
 */
void LCD_ST7735S_char(Site_t site, u8 ascii, u16 Color, u16 bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)

    u8 temp, t, pos;
    Size_t size = {8, 16};

    if(site.x > MAX_CHAR_POSX || site.y > MAX_CHAR_POSY)
    {
        return;
    }

    LCD_PTLON(site, size);

    LCD_RAMWR();                    //???

    for (pos = 0; pos < 16; pos++)
    {
        temp = ascii_8x16[((ascii-0x20)*16)+pos];

        for(t = 0; t < 8; t++)
        {

            if(temp & 0x80)
            {
                LCD_WR_DATA(Color);
            }
            else
            {
                LCD_WR_DATA(bkColor);
            }
            temp <<= 1;
        }
    }
    return;
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}

/*!
 *  @brief      ?????
 *  @param      site    ?????
 *  @param      Str     ?????
 *  @param      Color   ????
 *  @param      bkColor ????
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_ST7735S_str(site,"www.chuxue123.com", BLUE,RED);
 */
void LCD_ST7735S_str(Site_t site, u8 *Str, u16 Color, u16 bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)
    while(*Str != '\0')
    {
        if(site.x > MAX_CHAR_POSX )
        {
            //??
            site.x = 0;
            site.y += 16;
        }
        if(site.y > MAX_CHAR_POSY )
        {
            //??
            site.y = 0;
            site.x = 0;
        }

        LCD_ST7735S_char(site, *Str, Color, bkColor);
        site.x += 8;
        Str ++ ;
    }
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}

/*!
 *  @brief      ????
 *  @param      site    ?????
 *  @param      num     ??
 *  @param      Color   ????
 *  @param      bkColor ????
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_num(site,123, BLUE,RED);
 */
void LCD_num(Site_t site, u32 num, u16 Color, u16 bkColor)
{
    u32 res = num;
    u8 t = 0;
    Site_t sitetemp;
    sitetemp.y = site.y;

    if( num == 0 )
    {
        LCD_ST7735S_char(site, '0', Color, bkColor);
        return;
    }
    while( res )  /*??????t*/
    {
        res /= 10;
        t++;
    }

    while(num)
    {
        sitetemp.x = site.x + (8 * (t--) - 8);
        LCD_ST7735S_char(sitetemp, (num % 10) + '0', Color, bkColor);
        num /= 10 ;
    }
}

/*!
 *  @brief      ????(??????)
 *  @param      site            ?????
 *  @param      num             ??
 *  @param      max_num_bit     ?????
 *  @param      Color           ????
 *  @param      bkColor         ????
 *  @since      v5.0
*  Sample usage:        Site_t site = {10,20};   //x = 10 ,y = 20
                        LCD_num_BC(site,123,5, BLUE,RED);
 */
void LCD_num_BC(Site_t site, u32 num, u8 max_num_bit, u16 Color, u16 bkColor)
{
    u32 res = num;
    u8 t = 0;
    Site_t sitetemp;
    Size_t size;

    sitetemp.y = site.y;

    if( num == 0 )
    {
        LCD_ST7735S_char(site, '0', Color, bkColor);

        site.x += 8;
        size.H  = 16;
        size.W  = 8 * (max_num_bit - 1);
        LCD_ST7735S_rectangle(site, size, bkColor);

        return;
    }
    while( res )            /*??????t*/
    {
        res /= 10;
        t++;
    }
    if(t >= max_num_bit )    //??????
    {
        t = max_num_bit;
    }

    res = t;

    while( t != 0 )
    {
        sitetemp.x = site.x + (8 * (t--) - 8);
        LCD_ST7735S_char(sitetemp, (num % 10) + '0', Color, bkColor);
        num /= 10 ;
    }

    if(res != max_num_bit )
    {
        size.W = 8 * (max_num_bit - res);
        site.x += (8 * res);
        size.H  = 16;
        LCD_ST7735S_rectangle(site, size, bkColor);
    }
}

/*!
 *  @brief      ??????
 *  @param      site            ?????
 *  @param      size            ??????
 *  @param      img             ????
 *  @since      v5.0
 *  Sample usage:       Site_t site = {10,20};      //x = 10 ,y = 20
                        Size_t size = {320,240};    //W = 320,H = 240
                        LCD_Img_gray(site, size, img);
 */
void LCD_Img_gray(Site_t site, Size_t size, u8 *img)
{
    u32     total = (size.H * size.W);
    u16     imgtemp;
    u8     *pimg = (u8 *)img;

    LCD_PTLON(site, size);                      //??
    LCD_RAMWR();                                //???

    while(total--)
    {
        imgtemp     = (u16) * (pimg++);
        imgtemp = GRAY_2_RGB565(imgtemp);
        LCD_WR_DATA( imgtemp );               //?????
    }
}

/*!
 *  @brief      ????????
 *  @param      site            ?????
 *  @param      size            ??????
 *  @param      img             ????
 *  @param      imgsize         ????
 *  @since      v5.0
 *  Sample usage:       Site_t site = {10,20};          //x = 10 ,y = 20
                        Size_t size = {80,60};          //W = 80,H = 60
                        Size_t imgsize = {320,240};     //W = 320,H = 240
                        LCD_Img_gray_Z(site, size, img,imgsize);
 */
void LCD_Img_gray_Z(Site_t site, Size_t size, u8 *img, Size_t imgsize)
{

    u32 temp, tempY;
    u16 x, y;
    u16 X, Y;
    u8 *pimg = (u8 *)img;
    u16 rgb;

    LCD_PTLON(site, size);                      //??

    LCD_RAMWR();                                //???

    for(y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H   ) / size.H) ;
        tempY = Y * imgsize.W ;

        for(x = 0; x < size.W; x++)
        {
            X = ( x * imgsize.W  ) / size.W ;
            temp = tempY + X;
            rgb = GRAY_2_RGB565(pimg[temp]);    //
            LCD_WR_DATA(rgb);
        }
    }
}



void LCD_Img_Binary(Site_t site, Size_t size, u8 *img)
{
    u32     total = (size.H * size.W) / 8;
    u8     imgtemp;
    u8       bitindex;
    u8     *pimg = (u8 *)img;

    LCD_PTLON(site, size);                      //??

    LCD_RAMWR();                                //???

    while(total--)
    {
        imgtemp     = *(pimg++);
        bitindex    = 8;
        while(bitindex--)
        {
            if( imgtemp & (0x01 << bitindex) )
            {
                LCD_WR_DATA( BINARY_COLOR );      //?????
            }
            else
            {
                LCD_WR_DATA(  BINARY_BGCOLOR  );      //?????
            }
        }
    }
}

void LCD_Img_Binary_Z(Site_t site, Size_t size, u8 *img, Size_t imgsize)
{

    u32 temp, tempY;
    u16 x, y;
    u16 X, Y;
    u8 *pimg = (u8 *)img;

    LCD_PTLON(site, size);                      //??

    LCD_RAMWR();                                //???

    for(y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H  ) / size.H) ;
        tempY = Y * imgsize.W ;

        for(x = 0; x < size.W; x++)
        {
            X = (( x * imgsize.W  ) / size.W) ;
            temp = tempY + X;
            if( (pimg[temp>>3] & (1 << (7 - (temp & 0x07))) ) == 0  )
            {
                LCD_WR_DATA( BINARY_BGCOLOR );        //?????
            }
            else
            {
                LCD_WR_DATA( BINARY_COLOR );      //?????
            }
        }
    }
}

void LCD_wave(Site_t site,Size_t size,u8 *img,u8 maxval,u16 Color ,u16 bkColor)
{

    u8 h;
    u16 y = site.y + size.H - 1;

    LCD_ST7735S_rectangle(site, size,bkColor);
    site.x += size.W;
    img  += (size.W - 1);
    while(size.W --)
    {
        if(*img >= maxval )
        {
            h = size.H - 1;
        }
        else
        {
            h = ((*img )* size.H )/maxval;
        }
        site.y = y - h ;
        site.x--;
        img --;
        LCD_ST7735S_point(site, Color);
    }
}

void LCD_wave_display(Site_t site,Size_t size,u8 *img,u8 maxval,u16 Color)
{

    u8 h;
    u16 y = site.y + size.H - 1;

    site.x += size.W;
    img  += (size.W - 1);
    while(size.W --)
    {
        if(*img >= maxval )
        {
            h = size.H - 1;
        }
        else
        {
            h = ((*img )* size.H )/maxval;
        }
        site.y = y - h ;
        site.x--;
        img --;
        LCD_ST7735S_point(site, Color);
    }
}

