#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "sim900a.h"	
#include "touch.h"	
#include "usart2.h"	
#include "usart3.h"	
#include "usart4.h"	
#include "lcd_u4.h"
 #include "md5.h"
//ALIENTEK Mini STM32开发板扩展实验14
//ATK-SIM900A GSM模块测试实验-库函数版本
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 
 
 void uart0_debug_str(uint8_t* str,uint16_t len)//str8
{
	int i;
    DB_PR("--1str---debug_str:");
    for(i=0;i<len;i++)
        DB_PR("%c ",str[i]);
    DB_PR("\r\n");
}
 
void uart0_debug_data_h(uint8_t* data,uint16_t len)//hex8
{
	int i;
    DB_PR("---2s----debug_data:\n");
    for(i=0;i<len;i++)
        DB_PR("%02x ",data[i]);
    DB_PR("-----2e---------\r\n");
}
void uart0_debug_data_d(uint8_t* data,uint16_t len)//dec8
{
	int i;
    DB_PR("---3---debug_data:");
    for(i=0;i<len;i++)
        DB_PR("%02d ",data[i]);
    DB_PR("\r\n");
}
 
/*
 * 系统软件复位
 */
void Soft_Reset(void)
{
  __set_FAULTMASK(1);   /* 关闭所有中断*/  
  NVIC_SystemReset();   /* 系统复位 */
}
 

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
 
 

 int main(void)
 { 
	u8 key,fontok=0;  

// int n;
// char *str = "b5";//"12345.67";

// char tab[4]={0xf1,0x22,0x33,0x44};
// char tab1[4]={0};
// char a[16];
// char i;



	// int i;
	// unsigned char encrypt[] = "admin";//21232f297a57a5a743894a0e4a801fc3  
	// unsigned char decrypt[16];

	// MD5_CTX md5;


	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级  
	uart_init(115200);	 	//串口初始化为9600			 
//	LCD_Init();				//初始化液晶 
	LED_Init();         	//LED初始化	 
//	KEY_Init();				//按键初始化	  													    
 	USART2_Init(115200);	//初始化串口 4g
	USART3_Init(9600);		//初始化串口 lock
	USART4_Init(115200);	//初始化串口 lcd
	printf("\r\n-0-野火WF-GSM模块TCP收发例程\r\n");
	//Usart_SendString( UART4,"abcde\n");

// HexToStr(a,tab,4);
// printf("The str:%s\n",a);

// StrToHex(tab1,a,4);
// for(i=0;i<4;i++)
// {
// 	printf("%x",tab1[i]);
// }
// printf("\n");
// // n = atoi(str);
// // printf("n=%02x\n",n);


	// MD5Init(&md5);
	// MD5Update(&md5, encrypt, strlen((char *)encrypt));
	// MD5Final(&md5, decrypt);

	// // //Md5加密后的32位结果
	// // printf("加密前:%s\n加密后16位:", encrypt);
	// // for (i = 4; i<12; i++)//8*2
	// // {
	// // 	printf("%02x", decrypt[i]);  
	// // }

	// //Md5加密后的32位结果
	// printf("\n加密前:%s\n加密后32位:", encrypt);
	// for (i = 0; i<16; i++)
	// {
	// 	printf("%02x", decrypt[i]);  
	// }









	send_cmd_to_lcd_pic(0x0000);
	 
	 
	delay_ms(500); //500
	
	/* 关闭所有led灯	*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	 
	delay_ms(580); //500
	/* 关闭所有led灯	*/
	GPIO_SetBits(GPIOB,GPIO_Pin_0);	 


	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	delay_ms(1000); //500
	printf("\r\n-1-野火WF-GSM模块TCP收发例程\r\n");

	 
//	tp_dev.init();			//触摸屏初始化
// 	mem_init();				//初始化内存池	    
// 	exfuns_init();			//为fatfs相关变量申请内存  
//  	f_mount(fs[0],"0:",1); 	//挂载SD卡 
// 	f_mount(fs[1],"1:",1); 	//挂载FLASH.
//	key=KEY_Scan(0);  
//	if(key==KEY0_PRES)		//强制校准
//	{
//		LCD_Clear(WHITE);	//清屏
//		tp_dev.adjust();  	//屏幕校准  
//		LCD_Clear(WHITE);	//清屏
//	}
//	fontok=font_init();		//检查字库是否OK
//	if(fontok||key==KEY1_PRES)//需要更新字库（字库不存在/KEY1按下）			 
//	{
//		LCD_Clear(WHITE);		   	//清屏
// 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
//		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
//		while(SD_Initialize())		//检测SD卡
//		{
//			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
//			delay_ms(200);
//			LCD_Fill(60,70,200+60,70+16,WHITE);
//			delay_ms(200);		    
//		}								 						    
//		LCD_ShowString(60,70,200,16,16,"SD Card OK");
//		LCD_ShowString(60,90,200,16,16,"Font Updating...");
//		key=update_font(20,110,16);//更新字库
//		while(key)//更新失败		
//		{			 		  
//			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
//			delay_ms(200);
//			LCD_Fill(20,110,200+20,110+16,WHITE);
//			delay_ms(200);		       
//		} 		  
//		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
//		delay_ms(1500);	
//		LCD_Clear(WHITE);//清屏	       
//	}  
	sim900a_test();
}


