#include "delay.h"
#include "usart4.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "usart.h"  
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

//串口发送缓存区 	
__align(8) u8 USART4_TX_BUF[USART4_MAX_SEND_LEN]; 	//发送缓冲,最大USART4_MAX_SEND_LEN字节
#ifdef USART4_RX_EN   								//如果使能了接收   	  
//串口接收缓存区 	
u8 USART4_RX_BUF[USART4_MAX_RECV_LEN]; 				//接收缓冲,最大USART4_MAX_RECV_LEN个字节.


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART4_RX_STA=0;   

void UART4_IRQHandler(void)
{
	u8 res;	    
if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(UART4);				 
		if(USART4_RX_STA<USART4_MAX_RECV_LEN)		//还可以接收数据
		{
		  TIM_SetCounter(TIM2,0);//计数器清空      
			if(USART4_RX_STA==0)TIM2_Set(1);	 	//使能定时器4的中断 
			USART4_RX_BUF[USART4_RX_STA++]=res;		//记录接收到的值	 
			// USART_SendData(UART4, res);
			// DB_PR("res=%d",res);
		}else 
		{
			USART4_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  											 
}   
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void USART4_Init(u32 bound)
{ 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

 	USART_DeInit(UART4);  //复位串口1
		 //USART4_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PA2
   
    //USART4_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化PA3
	
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(UART4, &USART_InitStructure); //初始化串口	2
  
// 	//波特率设置
//  //	USART4->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
// 	//USART4->CR1|=0X200C;  	//1位停止,无校验位.
// 	USART_DMACmd(USART4,USART_DMAReq_Tx,ENABLE);  	//使能串口2的DMA发送
// 	UART_DMA_Config(DMA1_Channel7,(u32)&USART4->DR,(u32)USART4_TX_BUF);//DMA1通道7,外设为串口2,存储器为USART4_TX_BUF 
	USART_Cmd(UART4, ENABLE);                    //使能串口 
	
#ifdef USART4_RX_EN		  	//如果使能了接收
	//使能接收中断
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	TIM2_Init(79,7199);		//30ms     10ms中断   99
	USART4_RX_STA=0;		//清零
	TIM2_Set(0);			//关闭定时器4
#endif	 									  	
}
// //串口2,printf 函数
// //确保一次发送数据不超过USART4_MAX_SEND_LEN字节
// void u3_printf(char* fmt,...)  
// {  
// 	va_list ap;
// 	va_start(ap,fmt);
// 	vsprintf((char*)USART4_TX_BUF,fmt,ap);
// 	va_end(ap);
// 	while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
// 	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART4_TX_BUF)); 	//通过dma发送出去
// }
//定时器4中断服务程序		    
void TIM2_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART4_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志    
		TIM2_Set(0);			//关闭TIM2  
	}   
}
//设置TIM2的开关
//sta:0，关闭;1,开启;
void TIM2_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM2,0);//计数器清空
		TIM_Cmd(TIM2, ENABLE);  //使能TIMx	
	}else TIM_Cmd(TIM2, DISABLE);//关闭定时器4
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM2_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能//TIM2时钟使能    
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}


void spear_uart_send_datas(uint8_t* str, uint16_t len)
{
	DB_PR("\r\n");
	while(len--)
	{
//		USART_SendData(USART1, *str);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//		str++;
		//UartSend(*str++);
		DB_PR("%02x",*str++);
	}
	DB_PR("\r\n");
}

void debug_uart4_send_datas(uint8_t* str, uint16_t len)
{
	DB_PR("----u4 rcv-----\r\n");
	while(len--)
	{
//		USART_SendData(USART1, *str);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
//		str++;
		DB_PR("%02x",*str++);
	}
	DB_PR("-----u4 end----\r\n");
}


//1
void uart4_send_datas(uint8_t* str, uint8_t len)
{
	while(len--)
	{
		//delay_ms(50);  
		USART_SendData(UART4, *str);
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); 
		str++;
	}
}








#endif		 
///////////////////////////////////////USART4 DMA发送配置部分//////////////////////////////////	   		    
// //DMA1的各通道配置
// //这里的传输形式是固定的,这点要根据不同的情况来修改
// //从存储器->外设模式/8位数据宽度/存储器增量模式
// //DMA_CHx:DMA通道CHx
// //cpar:外设地址
// //cmar:存储器地址    
// void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
// {
// 	DMA_InitTypeDef DMA_InitStructure;
//  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
//   DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设ADC基地址
// 	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
// 	DMA_InitStructure.DMA_BufferSize = 0;  //DMA通道的DMA缓存的大小
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
// 	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器	
// } 
// //开启一次DMA传输
// void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
// {
// 	DMA_Cmd(DMA_CHx, DISABLE );  //关闭 指示的通道        
// 	DMA_SetCurrDataCounter(DMA_CHx,len);//DMA通道的DMA缓存的大小	
// 	DMA_Cmd(DMA_CHx, ENABLE);           //开启DMA传输
// }	   
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 





















