#include "stm32f10x.h"
#include "gpio.h"
#include "st7735s_lcd.h"
#include "usart.h"



u16  st7735s_h   = ST7735S_H;
u16  st7735s_w   = ST7735S_W;
u8   st7735s_dir = ST7735S_DIR_DEFAULT;

void LCD_ST7735S_wr_data(u8 data)
{
    LCD_ST7735S_RD_OUT = 1;
    LCD_ST7735S_RS_OUT = 1;
    LCD_ST7735S_CS_OUT = 0;

    LCD_ST7735S_P0(data);

    ST7735S_DELAY();

    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_WR_OUT = 1;
    LCD_ST7735S_CS_OUT = 1;
}

void LCD_ST7735S_wr_cmd(u8 cmd)
{
    LCD_ST7735S_RD_OUT = 1;
    LCD_ST7735S_RS_OUT = 0;
    LCD_ST7735S_CS_OUT = 0;

    LCD_ST7735S_P0(cmd);

    ST7735S_DELAY();
    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_WR_OUT = 1;
    LCD_ST7735S_CS_OUT = 1;
}

/*!
 *  @brief      LCD_ST7735S???
 *  @since      v5.0
 */
void    LCD_ST7735S_init()
{
    //u8 n ;
//��ʼ��PT0-7�,9-13  O0
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOF|ST7735S_WR_GPIO_CLK |ST7735S_RD_GPIO_CLK |ST7735S_CS_GPIO_CLK|ST7735S_RS_GPIO_CLK|ST7735S_RST_GPIO_CLK|ST7735S_DATA_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ST7735S_WR_GPIO_PIN;
	GPIO_Init(ST7735S_WR_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ST7735S_RD_GPIO_PIN;
	GPIO_Init(ST7735S_RD_GPIO_PORT ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  ST7735S_CS_GPIO_PIN;
	GPIO_Init( ST7735S_CS_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  ST7735S_RS_GPIO_PIN;
	GPIO_Init( ST7735S_RS_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  ST7735S_DATA_GPIO_PIN;
	GPIO_Init( ST7735S_DATA_GPIO_PORT,&GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = ST7735S_RST_GPIO_PIN;
	GPIO_Init(ST7735S_RST_GPIO_PORT ,&GPIO_InitStructure);
	
	GPIOA->ODR &= ~(1<<1);//�ӵؽ�
	GPIOF->ODR |= (1<<0);//��Դ��
	
    ST7735S_DELAY();
    LCD_ST7735S_RST_OUT = 1;

    //��ʼ������
    LCD_ST7735S_RST_OUT = 1;
    ST7735S_DELAY();
    LCD_ST7735S_RST_OUT = 0;
    ST7735S_DELAY();
    LCD_ST7735S_RST_OUT = 1;
    ST7735S_DELAY_MS(500);      //�ϵ���㹻ʱ��

    LCD_ST7735S_WR_8CMD(0x11);           //Sleep out   �˳�˯��ģʽ
    ST7735S_DELAY_MS(120);      //Delay 120ms
    //------------------------------------ST7735S Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    LCD_ST7735S_WR_8CMD(0xB1);     //In Normal Mode (Full Colors)    ȫ��ģʽ
    LCD_ST7735S_WR_8DATA(0x05);    //���� RTNA Set 1-line  Period  һ������
    LCD_ST7735S_WR_8DATA(0x3A);    //���� FPA: Front Porch
    LCD_ST7735S_WR_8DATA(0x3A);    //���� BPA: Back Porch
    //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
    //����fosc = 850kHz

    /* Set the frame frequency of the Idle mode.  */
    LCD_ST7735S_WR_8CMD(0xB2);     //In Idle Mode (8-colors)
    LCD_ST7735S_WR_8DATA(0x05);
    LCD_ST7735S_WR_8DATA(0x3A);
    LCD_ST7735S_WR_8DATA(0x3A);

    /* Set the frame frequency of the Partial mode/ full colors. */
    LCD_ST7735S_WR_8CMD(0xB3);
    LCD_ST7735S_WR_8DATA(0x05);
    LCD_ST7735S_WR_8DATA(0x3A);
    LCD_ST7735S_WR_8DATA(0x3A);
    LCD_ST7735S_WR_8DATA(0x05);
    LCD_ST7735S_WR_8DATA(0x3A);
    LCD_ST7735S_WR_8DATA(0x3A);

    //------------------------------------End ST7735S Frame Rate-----------------------------------------//
    LCD_ST7735S_WR_8CMD(0xB4);   //Display Inversion Control  ??????

    LCD_ST7735S_WR_8DATA(0x07);  //LCD_ST7735S_WR_8DATA(0x03); ???3,??7
    // ����λ�Ӹߵ��ף��ֱ���  full colors normal mode ?Idle mode ?
    // full Colors partial mode  �� �㷴ת ���з�ת 1Ϊ�з�ת

    LCD_ST7735S_WR_8CMD(0xC0);   //Power Control Setting  ��Դ��������
    LCD_ST7735S_WR_8DATA(0x28);
    LCD_ST7735S_WR_8DATA(0x08);
    LCD_ST7735S_WR_8DATA(0x84);
    LCD_ST7735S_WR_8CMD(0xC1);
    LCD_ST7735S_WR_8DATA(0XC0);
    LCD_ST7735S_WR_8CMD(0xC2);
    LCD_ST7735S_WR_8DATA(0x0C);
    LCD_ST7735S_WR_8DATA(0x00);
    LCD_ST7735S_WR_8CMD(0xC3);
    LCD_ST7735S_WR_8DATA(0x8C);
    LCD_ST7735S_WR_8DATA(0x2A);
    LCD_ST7735S_WR_8CMD(0xC4);
    LCD_ST7735S_WR_8DATA(0x8A);
    LCD_ST7735S_WR_8DATA(0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    LCD_ST7735S_WR_8CMD(0xC5);   //  VCOM ��ѹ����
    LCD_ST7735S_WR_8DATA(0x0C); //  -0.725
    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    LCD_ST7735S_WR_8CMD(0xE0);
    LCD_ST7735S_WR_8DATA(0x05);
    LCD_ST7735S_WR_8DATA(0x1A);
    LCD_ST7735S_WR_8DATA(0x0C);
    LCD_ST7735S_WR_8DATA(0x0E);
    LCD_ST7735S_WR_8DATA(0x3A);
    LCD_ST7735S_WR_8DATA(0x34);
    LCD_ST7735S_WR_8DATA(0x2D);
    LCD_ST7735S_WR_8DATA(0x2F);
    LCD_ST7735S_WR_8DATA(0x2D);
    LCD_ST7735S_WR_8DATA(0x2A);
    LCD_ST7735S_WR_8DATA(0x2F);
    LCD_ST7735S_WR_8DATA(0x3C);
    LCD_ST7735S_WR_8DATA(0x00);
    LCD_ST7735S_WR_8DATA(0x01);
    LCD_ST7735S_WR_8DATA(0x02);
    LCD_ST7735S_WR_8DATA(0x10);
    LCD_ST7735S_WR_8CMD(0xE1);
    LCD_ST7735S_WR_8DATA(0x04);
    LCD_ST7735S_WR_8DATA(0x1B);
    LCD_ST7735S_WR_8DATA(0x0D);
    LCD_ST7735S_WR_8DATA(0x0E);
    LCD_ST7735S_WR_8DATA(0x2D);
    LCD_ST7735S_WR_8DATA(0x29);
    LCD_ST7735S_WR_8DATA(0x24);
    LCD_ST7735S_WR_8DATA(0x29);
    LCD_ST7735S_WR_8DATA(0x28);
    LCD_ST7735S_WR_8DATA(0x26);
    LCD_ST7735S_WR_8DATA(0x31);
    LCD_ST7735S_WR_8DATA(0x3B);
    LCD_ST7735S_WR_8DATA(0x00);
    LCD_ST7735S_WR_8DATA(0x00);
    LCD_ST7735S_WR_8DATA(0x03);
    LCD_ST7735S_WR_8DATA(0x12);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//

    LCD_ST7735S_WR_8CMD(0x3A); //65k mode
    LCD_ST7735S_WR_8DATA(0x05);
    LCD_ST7735S_WR_8CMD(0x29); //����ʾ Display on
    LCD_ST7735S_WR_8CMD(0x2c); //

    LCD_ST7735S_dir(st7735s_dir);

}

/*!
   @brief      ����ST7735S GRAMָ��ɨ�跽��
 *  @param      option    ����ѡ��(����(��������������) Ϊ 0 ,90��Ϊ1,180��Ϊ2,270Ϊ3)
 *  @since      v5.0
 */
void LCD_ST7735S_dir(u8 option)
{
    if(option >= 4)
			while(1)
				printf("ERROR!!!   ");

    st7735s_dir = option;
    switch(option)
    {
    case 0:
        LCD_ST7735S_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ,MV ���н��� ,ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735S_WR_8DATA(0xc8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735S_WR_8CMD(0x2a);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x02);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x81);

        LCD_ST7735S_WR_8CMD(0x2B);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x03);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x82);

        st7735s_h   = ST7735S_H;
        st7735s_w   = ST7735S_W;
        break;
    case 1:
        LCD_ST7735S_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ,MV ���н��� ,ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735S_WR_8DATA(0xA8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735S_WR_8CMD(0x2a);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x03);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x82);

        LCD_ST7735S_WR_8CMD(0x2B);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x02);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x81);

        st7735s_h   = ST7735S_W;
        st7735s_w   = ST7735S_H;
        break;
    case 2:
        LCD_ST7735S_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ,MV ���н��� ,ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735S_WR_8DATA(0x08);//0xc8  0xA8 0x08 0x68

        LCD_ST7735S_WR_8CMD(0x2a);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x02);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x81);

        LCD_ST7735S_WR_8CMD(0x2B);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x03);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x82);

        st7735s_h   = ST7735S_H;
        st7735s_w   = ST7735S_W;
        break;
    case 3:
        LCD_ST7735S_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ,MV ���н��� ,ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735S_WR_8DATA(0x68);//0xc8  0xA8 0x08 0x68

        LCD_ST7735S_WR_8CMD(0x2a);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x03);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x82);

        LCD_ST7735S_WR_8CMD(0x2B);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x02);
        LCD_ST7735S_WR_8DATA(0x00);
        LCD_ST7735S_WR_8DATA(0x81);

        st7735s_h   = ST7735S_W;
        st7735s_w   = ST7735S_H;
        break;
    default:
        break;
    }

}

/*!
 *  @brief      ����ST7735S ����
 *  @param      site        ���Ͻ�����λ��
 *  @param      size        ������С
 *  @since      v5.0
 */
void LCD_ST7735S_ptlon(Site_t site, Size_t size)
{
    if(st7735s_dir&0x01)
    {
        site.x += 3;     //Һ����Ҫƫ��һ�£��������ܿ���������
        site.y += 2;
    }
    else
    {
        site.x += 2;     //Һ����Ҫƫ��һ�£��������ܿ���������
        site.y += 3;
    }
    LCD_ST7735S_WR_8CMD(0x2a);   //Partial Mode On  �ֲ�ģʽ
    LCD_ST7735S_WR_8DATA((u8)(site.x >> 8)); //��8λ Sx
    LCD_ST7735S_WR_8DATA((u8)site.x);   //�Ͱ�λ Sx
    LCD_ST7735S_WR_8DATA((u8)((site.x + size.W - 1) >> 8));
    LCD_ST7735S_WR_8DATA((u8)(site.x + size.W - 1));

    LCD_ST7735S_WR_8CMD(0x2B);//Row Address Set  �е�ַ����
    LCD_ST7735S_WR_8DATA((u8)(site.y >> 8));
    LCD_ST7735S_WR_8DATA((u8)site.y);
    LCD_ST7735S_WR_8DATA((u8)((site.y + size.H - 1) >> 8));
    LCD_ST7735S_WR_8DATA((u8)(site.y + size.H - 1));

}

/*!
 *  @brief      ��ȡ ST7735S �߶�
 *  @return     ILI9341 ??
 *  @since      v5.0
 */
u16 ST7735S_get_h()
{
    return st7735s_h;
}

/*!
 *  @brief      ��ȡ ST7735S ����
 *  @return     ILI9341 ����
 *  @since      v5.0
 */
u16 ST7735S_get_w()
{
    return st7735s_w;
}

/*!
 *  @brief      ��ȡ ST7735S ��ʾ����
 *  @return     ST7735S ����
 *  @since      v5.0
 */
u8 ST7735S_get_dir()
{
    return st7735s_dir;
}




