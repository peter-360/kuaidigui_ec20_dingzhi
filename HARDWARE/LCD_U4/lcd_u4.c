#include "delay.h"
#include "usart4.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	   
#include "lcd_u4.h"
#include "led.h"
#include <stdio.h>
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口2驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
//for main debug, 0 or 1


//1
// char *itoa(int value, char *string, int radix)
// {
//     int     i, d;
//     int     flag = 0;
//     char    *ptr = string;

//     /* This implementation only works for decimal numbers. */
//     if (radix != 10)
//     {
//         *ptr = 0;
//         return string;
//     }

//     if (!value)
//     {
//         *ptr++ = 0x30;
//         *ptr = 0;
//         return string;
//     }

//     /* if this is a negative value insert the minus sign. */
//     if (value < 0)
//     {
//         *ptr++ = '-';

//         /* Make the value positive. */
//         value *= -1;
//     }

//     for (i = 10000; i > 0; i /= 10)
//     {
//         d = value / i;

//         if (d || flag)
//         {
//             *ptr++ = (char)(d + 0x30);
//             value -= (d * i);
//             flag = 1;
//         }
//     }

//     /* Null terminate the string. */
//     *ptr = 0;

//     return string;

// } /* NCL_Itoa */




//2

 /*整形转字符型*/
 char *itoa(int value, char *string, int radix)
 {
 	char tmp[33];
 	char *tp = tmp;
 	int i;
 	unsigned v;
 	int sign;
 	char *sp;

 	if (radix > 36 || radix <= 1)
 	{
 	//__set_errno(EDOM);
 	return 0;
 	}

 	sign = (radix == 10 && value < 0);
 	if (sign)
 	v = -value;
 	else
 	v = (unsigned)value;
 	while (v || tp == tmp)
 	{
 	i = v % radix;
 	v = v / radix;
 	if (i < 10)
 	*tp++ = i+'0';
 	else
 	*tp++ = i + 'a' - 10;
 	}

 	if (string == 0)
 	string = (char *)malloc((tp-tmp)+sign+1);
 	sp = string;

 	if (sign)
 	*sp++ = '-';
 	while (tp > tmp)
 	*sp++ = *--tp;
 	*sp = 0;
 	return string;
 }



 /*长整形转字符型*/
 char *ltoa(long value, char *string, int radix)
 {
 	char tmp[33];
 	char *tp = tmp;
 	long i;
 	unsigned long v;
 	int sign;
 	char *sp;

 	if (radix > 36 || radix <= 1)
 	{
 	//__set_errno(EDOM);
 	return 0;
 	}

 	sign = (radix == 10 && value < 0);
 	if (sign)
 	v = -value;
 	else
 	v = (unsigned long)value;
 	while (v || tp == tmp)
 	{
 	i = v % radix;
 	v = v / radix;
 	if (i < 10)
 	*tp++ = i+'0';
 	else
 	*tp++ = i + 'a' - 10;
 	}

 	if (string == 0)
 	string = (char *)malloc((tp-tmp)+sign+1);
 	sp = string;

 	if (sign)
 	*sp++ = '-';
 	while (tp > tmp)
 	*sp++ = *--tp;
 	*sp = 0;
 	return string;
 }

/*
* FILE:        lib/crtdll/stdlib/itoa.c
* PURPOSE:     converts a integer to ascii
* PROGRAMER:   
* UPDATE HISTORY:
*              1995: Created
*              1998: Added ltoa Boudewijn Dekker
*/

// // itoa函数原型
// // #include <crtdll/errno.h>
// // #include <crtdll/stdlib.h>
// // #include <crtdll/internal/file.h>
// /*整形转字符型*/
// char *itoa(int value, char *string, int radix)
// {
// char tmp[33];
// char *tp = tmp;
// int i;
// unsigned v;
// int sign;
// char *sp;

// if (radix > 36 || radix <= 1)
// {
// //__set_errno(EDOM);
// return 0;
// }

// sign = (radix == 10 && value < 0);
// if (sign)
// v = -value;
// else
// v = (unsigned)value;
// while (v || tp == tmp)
// {
// i = v % radix;
// v = v / radix;
// if (i < 10)
// *tp++ = i+'0';
// else
// *tp++ = i + 'a' - 10;
// }

// if (string == 0)
// string = (char *)malloc((tp-tmp)+sign+1);
// sp = string;

// if (sign)
// *sp++ = '-';
// while (tp > tmp)
// *sp++ = *--tp;
// *sp = 0;
// return string;
// }

// /*长整形转字符型*/
// char *ltoa(long value, char *string, int radix)
// {
// char tmp[33];
// char *tp = tmp;
// long i;
// unsigned long v;
// int sign;
// char *sp;

// if (radix > 36 || radix <= 1)
// {
// //__set_errno(EDOM);
// return 0;
// }

// sign = (radix == 10 && value < 0);
// if (sign)
// v = -value;
// else
// v = (unsigned long)value;
// while (v || tp == tmp)
// {
// i = v % radix;
// v = v / radix;
// if (i < 10)
// *tp++ = i+'0';
// else
// *tp++ = i + 'a' - 10;
// }

// if (string == 0)
// string = (char *)malloc((tp-tmp)+sign+1);
// sp = string;

// if (sign)
// *sp++ = '-';
// while (tp > tmp)
// *sp++ = *--tp;
// *sp = 0;
// return string;
// }
// /*无符号长整形转字符型*/
// char *_ultoa(unsigned long value, char *string, int radix)
// {
// char tmp[33];
// char *tp = tmp;
// long i;
// unsigned long v = value;
// char *sp;

// if (radix > 36 || radix <= 1)
// {
// //__set_errno(EDOM);
// return 0;
// }


// while (v || tp == tmp)
// {
// i = v % radix;
// v = v / radix;
// if (i < 10)
// *tp++ = i+'0';
// else
// *tp++ = i + 'a' - 10;
// }
// if (string == 0)
// string = (char *)malloc((tp-tmp)+1);
// sp = string;

// while (tp > tmp)
// *sp++ = *--tp;
// *sp = 0;
// return string;
// }

// /*字符串转整形*/

// //#include "iostream.h"
// int atoi(const char * s)
// {
//     int n = 0;

//     while(!(*s >= '0' && *s <= '9'))
//         s++;

//     while(*s >= '0' && *s <= '9')
//     {
//         n *= 10;
//         n += *s - '0';
//         s++;
//     }
//     return n;
// }





//3
// void  ltoa(long num, char* str, int radix)
// {
//     int i = 0;
// 	int j = 0;
//     long sum;
//     unsigned long num1 = num;  //如果是负数求补码，必须将他的绝对值放在无符号位中在进行求反码
//     char str1[33] = { 0 };
//     if (num<0) {              //求出负数的补码
//         num = -num;
//         num1 = ~num;
//         num1 += 1;
//     }
//     if (num == 0) {             
//         str1[i] = '0';
        
//         i++;
//     }
//     while(num1 !=0) {                      //进行进制运算
//         sum = num1 % radix;
//         str1[i] = (sum > 9) ? (sum - 10) + 'a' : sum + '0';
//         num1 = num1 / radix;
//         i++;
//     }
//     i--;
    
//     for (i; i >= 0; i--) {               //逆序输出 
//         str[i] = str1[j];
//         j++;
//     }
    
// }



// void  itoa(int num, char* str, int radix)
// {
//     int i = 0;
// 	int j = 0;
//     int sum;
//     int num1 = num;  //如果是负数求补码，必须将他的绝对值放在无符号位中在进行求反码
//     char str1[33] = { 0 };
//     if (num<0) {              //求出负数的补码
//         num = -num;
//         num1 = ~num;
//         num1 += 1;
//     }
//     if (num == 0) {             
//         str1[i] = '0';
        
//         i++;
//     }
//     while(num1 !=0) {                      //进行进制运算
//         sum = num1 % radix;
//         str1[i] = (sum > 9) ? (sum - 10) + 'a' : sum + '0';
//         num1 = num1 / radix;
//         i++;
//     }
//     i--;
    
//     for (i; i >= 0; i--) {               //逆序输出 
//         str[i] = str1[j];
//         j++;
//     }

// }



void HexToStr(char *pbDest,char *pbSrc,int nlen)
{
char ddl,ddh;
int i;
for(i=0;i<nlen;i++)
{
ddh = 48 +pbSrc[i] / 16;
ddl = 48 +pbSrc[i] % 16;
if(ddh>57) ddh = ddh+7;
if(ddl>57) ddl = ddl+7;
pbDest[i*2] = ddh;
pbDest[i*2+1] =ddl;
}

pbDest[nlen*2] = '\0';
}

void StrToHex(char *pbDest, char *pbSrc, int nLen)
{

char h1,h2;
char s1,s2;
int i;

for (i=0; i<nLen; i++)
{

h1 = pbSrc[2*i];
h2 = pbSrc[2*i+1];

s1 = toupper(h1) - 0x30;
if (s1 > 9) 
s1 -= 7;

s2 = toupper(h2) - 0x30;
if (s2 > 9) 
s2 -= 7;

pbDest[i] = s1*16 + s2;
}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART4_RX_STA;
//     1,清零USART4_RX_STA;
void lcd_at_response(u8 mode)
{
	uint8_t data_rx_t[USART4_MAX_RECV_LEN] = {0};
	uint16_t len_rx_t= 0;
	uint16_t bl_addr=0;//bianliang lcd
	// int32_t guimen_gk_temp =0;


	if(USART4_RX_STA&0X8000)		//接收到一次数据了
	{ 
        USART4_RX_BUF[USART4_RX_STA&0X7FFF]=0;//添加结束符 -------------
        // printf("USART4_RX_BUF=%s\n",USART4_RX_BUF);	//发送到串口
        len_rx_t=USART4_RX_STA&0x7FFF;
        printf("len_rx_t=%x",len_rx_t);	//asdUSART4_RX_STA=8003

        //Usart_SendByte
        memcpy(data_rx_t,USART4_RX_BUF,len_rx_t);
        uart0_debug_data_h(data_rx_t,len_rx_t);

        if(mode)
            USART4_RX_STA=0;


        if((0x5A == data_rx_t[0])
            &&(0xA5 == data_rx_t[1])
            &&((len_rx_t-3) == data_rx_t[2]))
            {
                bl_addr = (data_rx_t[4]<<8) + data_rx_t[5];
                DB_PR("-----.bl_addr=%04x\r\n",bl_addr);
                switch (data_rx_t[3])
                {
                    case 0x83:
                        switch (bl_addr)
                        {
                            case 0x1600://
                                DB_PR("--11111111--\r\n");
                                break;
                            default:
                                DB_PR("--default in--\r\n");
                                break;
                        }
                    default:
                        DB_PR("--default out--\r\n");
                        break;

                }
            }


	} 
}










void send_cmd_to_lcd_pic(uint16_t temp)//图片
{
    uint8_t tx_Buffer[50]={0};  
    uint16_t crc16_temp=0;

    printf("-----pic-----.\r\n");
    tx_Buffer[0] = 0x5A;
    tx_Buffer[1] = 0xA5;

    tx_Buffer[2] = 0x07;//len
    tx_Buffer[3] = 0x82;

    tx_Buffer[4] = 0x00;
    tx_Buffer[5] = 0x84;

    tx_Buffer[6] = 0x5A;
    tx_Buffer[7] = 0x01;

        
    tx_Buffer[8] = temp/256;
    tx_Buffer[9] = temp%256;
    printf("temp-pic:0x%04x\r\n",temp);

    // //crc
    // crc16_temp = CRC16(tx_Buffer+3, TX4_PIC_LEN - 5);
    // printf("tx CRC16 result:0x%04X\r\n",crc16_temp);

    // tx_Buffer[10] = crc16_temp&0xff;
    // tx_Buffer[11] = (crc16_temp>>8)&0xff;
    
    //uart4_send_datas(tx_Buffer, TX4_PIC_LEN);
	Usart_SendArray( UART4,tx_Buffer,TX4_PIC_LEN);

    uart0_debug_data_h(tx_Buffer, TX4_PIC_LEN);
}




// //数组
// void send_cmd_to_lcd_bl_len(uint16_t opCode, uint8_t* buff_temp,uint16_t data_len)//变量
// {
//     uint8_t tx_Buffer[256]={0};  
//     uint16_t crc16_temp=0;
//     //xiao
//     tx_Buffer[0] = 0x5A;
//     tx_Buffer[1] = 0xA5;
//     tx_Buffer[2] = data_len;//len  
//     tx_Buffer[3] = 0x82;

//     tx_Buffer[4] = opCode/256;
//     tx_Buffer[5] = opCode%256;//dizhi

//     DB_PR("-------data_len=%d--------\r\n",data_len);
//     for (int i = 0; i < data_len ; i++) //data_len-2
// 	{
//         tx_Buffer[6+i] = buff_temp[i];
//         //DB_PR("0x%.2X ", (uint8_t)buff_temp[i]);
//     }
//     DB_PR("\r\n");


//     // //crc
//     // crc16_temp = CRC16(tx_Buffer+3, data_len-2);
//     // DB_PR("tx CRC16 result:0x%04X\r\n",crc16_temp);

//     // tx_Buffer[3+ data_len-2 ] = crc16_temp&0xff;
//     // tx_Buffer[3+ data_len-2 +1] = (crc16_temp>>8)&0xff;
//     // DB_PR("---------debug1---------\r\n");
//     Usart_SendArray( UART4, tx_Buffer, 3+ data_len);
//     DB_PR("---------debug2---------\r\n");
//     // uart0_debug_data_h( tx_Buffer, 3+ data_len);
// }




//数组
void send_cmd_to_lcd_bl_len(uint16_t opCode, uint8_t* buff_temp,uint16_t data_len)//变量
{
	int i;
    uint8_t tx_Buffer[256]={0};  
    uint16_t crc16_temp=0;
    //xiao
    tx_Buffer[0] = 0x5A;
    tx_Buffer[1] = 0xA5;
    tx_Buffer[2] = data_len-3;//len  
    tx_Buffer[3] = 0x82;

    tx_Buffer[4] = opCode/256;
    tx_Buffer[5] = opCode%256;//dizhi

    DB_PR("-------data_len=%d--------\r\n",data_len-3);
    for (i = 0; i < data_len-3 ; i++) //data_len-2
		{
				tx_Buffer[6+i] = buff_temp[i];
				//DB_PR("0x%.2X ", (uint8_t)buff_temp[i]);
		}
    DB_PR("\r\n");


    // //crc
    // crc16_temp = CRC16(tx_Buffer+3, data_len-2);
    // DB_PR("tx CRC16 result:0x%04X\r\n",crc16_temp);

    // tx_Buffer[3+ data_len-2 ] = crc16_temp&0xff;
    // tx_Buffer[3+ data_len-2 +1] = (crc16_temp>>8)&0xff;
    // DB_PR("---------debug1---------\r\n");
    Usart_SendArray( UART4, tx_Buffer, data_len);
    DB_PR("---------debug2---------\r\n");
    uart0_debug_data_h( tx_Buffer, data_len);
}


//2个字节
void send_cmd_to_lcd_bl(uint16_t opCode, uint16_t temp)//变量
{
    uint8_t tx_Buffer[50]={0};  
    uint16_t crc16_temp=0;
    //xiao
    tx_Buffer[0] = 0x5A;
    tx_Buffer[1] = 0xA5;
    tx_Buffer[2] = 0x05;//len
    tx_Buffer[3] = 0x82;

    tx_Buffer[4] = opCode/256;
    tx_Buffer[5] = opCode%256;//dizhi

    tx_Buffer[6] = temp/256;
    tx_Buffer[7] = temp%256;
    DB_PR("temp-bl:0x%04d\r\n",temp);
    //crc
    // crc16_temp = CRC16(tx_Buffer+3, TX1_LEN_BL -5);
    // DB_PR("tx CRC16 result:0x%04X\r\n",crc16_temp);

    // tx_Buffer[8] = crc16_temp&0xff;
    // tx_Buffer[9] = (crc16_temp>>8)&0xff;
    Usart_SendArray( UART4, tx_Buffer, TX4_BL_LEN);

    // uart0_debug_data_h(tx_Buffer, TX1_LEN_BL);
}










uint8_t ComputXor(uint8_t *InData, uint16_t Len)
{
	uint8_t Sum = 0;
	uint16_t i;
	for(i = 0; i < Len; i++)
	{
		Sum ^= InData[i];	
	}
	return Sum;
}


void send_cmd_to_lock(uint8_t board_addr, uint8_t lock_addr)//变量
{
    uint8_t tx_Buffer2[50]={0};  
    uint8_t bcc_temp=0;
    memcpy(tx_Buffer2,"star",4);
    tx_Buffer2[4]= 0x8A;//m_data.opcode;
    tx_Buffer2[5]= (uint8_t)board_addr;//m_data.board_addr;
    tx_Buffer2[6]= (uint8_t)lock_addr;//m_data.lock_addr;
    tx_Buffer2[7]= 0x11;//guding
    bcc_temp = ComputXor(tx_Buffer2+4,4);
    tx_Buffer2[8]= bcc_temp;
    memcpy(tx_Buffer2+9,"endo",4);

    tx_Buffer2[13]='\0';

    RS485_TX_EN();

    DB_PR("tx_Buffer2=");
    // uart0_debug_data_h(tx_Buffer2, 13);
    // uart_write_bytes(UART_NUM_LOCK, (const char *) tx_Buffer2, 13);
    Usart_SendArray( USART3,tx_Buffer2, 13);
    RS485_RX_EN();

}


//2 2.1
void send_cmd_to_lock_all(uint8_t board_addr)//变量//uint8_t opcode, 
{
    uint8_t tx_Buffer2[50]={0};  
    uint8_t bcc_temp=0;
    memcpy(tx_Buffer2,"star",4);
    tx_Buffer2[4]= 0x90;//opcode;//m_data.opcode;0x90 0x91
    tx_Buffer2[5]= (uint8_t)board_addr;//m_data.board_addr;
    bcc_temp = ComputXor(tx_Buffer2+4,2);
    tx_Buffer2[6]= bcc_temp;
    memcpy(tx_Buffer2+7,"endo",4);

    tx_Buffer2[11]='\0';

    RS485_TX_EN();

    DB_PR("tx_Buffer2=");
    // uart0_debug_data_h(tx_Buffer2, 11);
    // uart_write_bytes(UART_NUM_LOCK, (const char *) tx_Buffer2, 11);
    Usart_SendArray( USART3,tx_Buffer2, 11);
    RS485_RX_EN();

}







