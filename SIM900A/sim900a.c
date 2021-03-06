#include "sim900a.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "lcd.h" 	  
#include "flash.h" 	 
#include "touch.h" 	 
#include "malloc.h"
#include "string.h"    
#include "text.h"		
#include "usart2.h" 
#include "usart3.h" 
#include "usart4.h" 
#include "lcd_u4.h"
#include "ff.h"

#include "cJSON.h"
#include "md5.h"
#include "iwdg.h"

#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-SIM900A GSM/GPRS模块驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////	
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;

void delay_xs(u16 xms)
{	 	
	int i=0;
	for(i=0; i<xms; i++)
	{
   		delay_ms(1000);
		// DB_PR("-----------\n");
	}
	DB_PR("-----delay_xs =%d------\n",xms);

}



// u8 UTF8toUnicode(u8 *ch, u16 *_unicode)
// {
//     u8 *p = NULL ,n = 0;
//     u16 e = 0; 
//     p = ch;
//     if(1)//p == NULL
//     {
//             if(*p >= 0xfc)
//             {
//                     /*6:<11111100>*/
//                     e  = (p[0] & 0x01) << 30;
//                     e |= (p[1] & 0x3f) << 24;
//                     e |= (p[2] & 0x3f) << 18;
//                     e |= (p[3] & 0x3f) << 12;
//                     e |= (p[4] & 0x3f) << 6;
//                     e |= (p[5] & 0x3f);
//                     n = 6;
//             }
//             else if(*p >= 0xf8) 
//             {
//                     /*5:<11111000>*/
//                     e = (p[0] & 0x03) << 24;
//                     e |= (p[1] & 0x3f) << 18;
//                     e |= (p[2] & 0x3f) << 12;
//                     e |= (p[3] & 0x3f) << 6;
//                     e |= (p[4] & 0x3f);
//                     n = 5;
//             }
//             else if(*p >= 0xf0)
//             {
//                     /*4:<11110000>*/
//                     e = (p[0] & 0x07) << 18;
//                     e |= (p[1] & 0x3f) << 12;
//                     e |= (p[2] & 0x3f) << 6;
//                     e |= (p[3] & 0x3f);
//                     n = 4;
//             }
//             else if(*p >= 0xe0)
//             {
//                     /*3:<11100000>*/
//                     e = (p[0] & 0x0f) << 12;
//                     e |= (p[1] & 0x3f) << 6;
//                     e |= (p[2] & 0x3f);
//                     n = 3;
//             }
//             else if(*p >= 0xc0) 
//             {
//                     /*2:<11000000>*/
//                     e = (p[0] & 0x1f) << 6;
//                     e |= (p[1] & 0x3f);
//                     n = 2;
//             }
//             else
//             {
//                     e = p[0];           
//                     n = 1;
//             }
//             *_unicode = e;
//     }
//     return n;
// }













u32 cjson_to_struct_info_qrcode(char *text)//kaiji
{
	u32 reg_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson对象

	cJSON * item2 = NULL;//cjson对象
	
	cJSON * item3 = NULL;//cjson对象
	uint8_t buff_t[256]={0};
	int i=0;
	u16 index_m=0;


	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //截取有效json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);

	DB_PR("\n----2----text=\n%s\n",text);


    root = cJSON_Parse(text);     
    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
		reg_status =1;//add

        DB_PR("%s\n", "有格式的方式打印Json:");           
        // DB_PR("%s\n\n", cJSON_Print(root));
        // DB_PR("%s\n", "无格式方式打印json：");
        // DB_PR("%s\n\n", cJSON_PrintUnformatted(root));

		//---------------------
		DB_PR("\n%s\n", "--1--一步一步的获取------------ 键值对:");
		DB_PR("%s\n", "获取result下的cjson对象:");
		item = cJSON_GetObjectItem(root, "result");//
		DB_PR("--1--%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
		DB_PR("--2--%s\n", item->valuestring);

		// DB_PR("%s\n", cJSON_Print(item));




		memset(buff_t,0,256);
		memcpy(buff_t, item->valuestring,strlen( item->valuestring));
		index_m = strlen(buff_t);
		DB_PR("--index_m=%d--\n", index_m);
		buff_t[index_m]=0xff;
		buff_t[index_m+1]=0xff;
		
		uart0_debug_data_h(buff_t,256);

		send_cmd_to_lcd_bl_len(0x2000,(uint8_t*)buff_t,128+4);//gekou 33 +3





		// send_cmd_to_lcd_pic(0x0003);
    
    }

    cJSON_Delete(root);
    return reg_status;

}

char regst_key[60]={0};

u32 cjson_to_struct_info_register(char *text)
{
	u32 reg_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson对象

	cJSON * item2 = NULL;//cjson对象
	
	cJSON * item3 = NULL;//cjson对象
	int i=0;


	u32 reg_active_code =0;
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[256]={0};
	char* buff_t3="b5";
	int my_bl_ts=0;
	//单片机测试
	// uint8_t buff_t3[256]={0xb5 ,0xa5 ,0xc6 ,0xac ,0xbb ,0xfa ,0xb2 ,0xe2 ,0xca ,0xd4 ,0x32};
	
	int company_id=0;
	char* url_t="https://iot.xintiangui.com/web_wechat/download_app?cid=";//guding



	char regst_key_post[300];
	u16 index_m=0;

	// const char needle[10] = "\r\n";
	// char *ret;
	
	// //设备注册成功,请激活柜子
	// char* str_tmp="{\"status\":0,\"msg\":\"abc\",\"result\":{\"active_code\":87993541}}";
	
	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //截取有效json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');
    // char *index=strstr(text,"{\"post_data\":{");
    // bzero(text, sizeof(text));
    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);
	// 	memset(text,0,89);
	// 	memcpy(text,index,89);
	// 	text[89]='\0';

	// DB_PR("---strlen(text)= %d\n", strlen(text));
	// DB_PR("---89= %02x\n", text[89]);
	// DB_PR("---90= %02x\n", text[90]);
 
	// for(i=89;i<strlen(text);i++)
	// {
	// 	text[i]=0;
	// }


//    ret = strstr(text, needle);
	DB_PR("\n----2----text=\n%s\n",text);










	// DB_PR("str_tmp=%s",str_tmp);


	root = cJSON_Parse(text);     
	if (!root) 
	{
		DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	else
	{
		DB_PR("%s\n", "有格式的方式打印Json:");           
		// DB_PR("%s\n\n", cJSON_Print(root));
		// DB_PR("%s\n", "无格式方式打印json：");
		// DB_PR("%s\n\n", cJSON_PrintUnformatted(root));




		DB_PR("%s\n", "获取status下的cjson对象");
		item = cJSON_GetObjectItem(root, "status");
		// DB_PR("%s\n", cJSON_Print(item));
		DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
		DB_PR("%d\n", item->valueint);
		reg_status = item->valueint;
		DB_PR("reg_status=%d\n", reg_status);





		if(0== reg_status)
		{
			//---------------------
			DB_PR("\n%s\n", "--1--一步一步的获取------------ 键值对:");
			DB_PR("%s\n", "获取result下的cjson对象:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "获取active_code下的cjson对象");
			item = cJSON_GetObjectItem(item, "active_code");
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("--1--%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("--2--%u\n", item->valueint);
			reg_active_code= item->valueint;
			DB_PR("reg_active_code=%u\n", reg_active_code);

			// reg_active_code = 12345608;//-------------
			// reg_active_code = 0x12345078;//-------------
			ltoa(reg_active_code,(char*)buff_t,10);
			uart0_debug_data_h(buff_t,strlen(buff_t));
			send_cmd_to_lcd_bl_len(0x1010,(uint8_t*)buff_t,32+4);//gekou 33 +3

			send_cmd_to_lcd_pic(0x0002);



			//获取http数据------------
		}
		else if(2== reg_status)
		{
			//-----------company------------
			DB_PR("%s\n", "获取result下的cjson对象:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "获取 company 下的cjson对象");
			item2 = cJSON_GetObjectItem(item, "company");

			DB_PR("%s\n", "获取 service_tel 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "service_tel");
			DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%s\n", item->valuestring);

			memset(buff_t,0,256);
			memcpy(buff_t,item->valuestring,8* sizeof(item->valuestring));
			DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			send_cmd_to_lcd_bl_len(0x1150,(uint8_t*)buff_t,32+4);//gekou 33 +3




			DB_PR("%s\n", "获取 register_key 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "register_key");
			DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%s\n", item->valuestring);

			memset(buff_t,0,256);
			memcpy(regst_key,item->valuestring,strlen((item->valuestring)));//8* sizeof(item->valuestring)
			DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			DB_PR("strlen((item->valuestring)=%d\n", strlen((item->valuestring)));
			DB_PR("regst_key=%s\n", regst_key);//-----------------------------------------------
			// send_cmd_to_lcd_bl_len(0x1150,(uint8_t*)buff_t,32+4);//gekou 33 +3









			// //----------todo utf8 gbk-----------
			// DB_PR("%s\n", "获取 property 下的cjson对象");
			// item = cJSON_GetObjectItem(item2, "property");
			// DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			// DB_PR("%s\n", item->valuestring);

			// memset(buff_t,0,256);
			// memcpy(buff_t,item->valuestring,8* sizeof(item->valuestring));
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			// uart0_debug_data_h(buff_t,strlen((char*)buff_t));

			// //---------------------
			// DB_PR("%s\n", "获取 tag 下的cjson对象");
			// item = cJSON_GetObjectItem(item2, "tag");
			// DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			// DB_PR("%s\n", item->valuestring);

			// uart0_debug_data_h(item->valuestring,strlen((char*)(item->valuestring)));

			// // memset(buff_t,0,256);
			// memcpy(buff_t+8* sizeof(item->valuestring),item->valuestring,8* sizeof(item->valuestring));
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));



			//---------------------

			//---------------------
			DB_PR("%s\n", "获取 tag 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "company_name_gbk");
			DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%s\n", item->valuestring);

			uart0_debug_data_h(item->valuestring,strlen((char*)(item->valuestring)));

			memset(buff_t,0,256);
			memset(buff_t2,0,256);
			memcpy(buff_t ,item->valuestring,strlen((char*)(item->valuestring)));
			DB_PR("------sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			DB_PR("------strlen((char*)(item->valuestring)=%d\n", strlen((char*)(item->valuestring)));
			uart0_debug_data_h(buff_t,strlen((char*)(buff_t)));
			for (i = 0; i <strlen(buff_t)/3+strlen(buff_t)%3 -1; i++)//-----to
			{
				StrToHex(buff_t2+i,(char*)(buff_t+i*3),1);
				// buff_t2[i] = atoi((const char*)buff_t+i*3);		
				DB_PR("buff_t2[%d]=%02x\n",i, buff_t2[i]);			
			}

			send_cmd_to_lcd_bl_len(0x1020,(uint8_t*)buff_t2,32*7+4);//gekou 33 +3




			// utf8_to_gbk(tmp->valuestring, gbk);--------------------








			//-----------company------------
			DB_PR("%s\n", "获取result下的cjson对象:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "获取 company 下的cjson对象");
			item2 = cJSON_GetObjectItem(item, "company");

			DB_PR("%s\n", "获取 company_id 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "company_id");
			DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%d\n", item->valueint);
			company_id = item->valueint;

			
			memset(buff_t,0,256);
			sprintf(buff_t,"%s%d",url_t,company_id);

			index_m = strlen(buff_t);
			DB_PR("--index_m=%d--\n", index_m);
			buff_t[index_m]=0xff;
			buff_t[index_m+1]=0xff;

			
			uart0_debug_data_h(buff_t,256);

			// //-----------ads------------
			// DB_PR("%s\n", "获取result下的cjson对象:");
			// item = cJSON_GetObjectItem(root, "result");//
			// // DB_PR("%s\n", cJSON_Print(item));

			// DB_PR("%s\n", "获取 ads 下的cjson对象");
			// item2 = cJSON_GetObjectItem(item, "ads");

			// item3 = cJSON_GetArrayItem(item2, 0);



			// DB_PR("%s\n", "获取 url 下的cjson对象");
			// item = cJSON_GetObjectItem(item3, "url");
			// DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			// DB_PR("%s\n", item->valuestring);

			// memset(buff_t,0,256);
			// memcpy(buff_t,item->valuestring,strlen(item->valuestring));
			// // sprintf(buff_t,"%sffff",buff_t);
			// buff_t[strlen(item->valuestring)]=0xff;
			// buff_t[strlen(item->valuestring)+1]=0xff;
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));

			send_cmd_to_lcd_bl_len(0x3000,(uint8_t*)buff_t,128+4);//gekou 33 +3






//-----------------------------------------------------------
			// delay_ms(1000); //500
			// USART2_RX_STA=0;
			// DB_PR("...a-9-1...\n");
			//52
			sim900a_send_cmd("AT+QHTTPURL=52,80","CONNECT",800);// != GSM_TRUE) return GSM_FALSE;//"OK"
			DB_PR("...a-9...\n");


			
			// sim900a_send_cmd_tou_data("http://express.admin.modoubox.com/web_wechat/deliver/qrcode","OK",800);
			sim900a_send_cmd_tou_data("https://iot.xintiangui.com/web_wechat/deliver/qrcode","OK",800);
			DB_PR("...a-10...\n");


			// //USART2_RX_STA =0;
			// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
			// DB_PR("...a-11-1...\n");



			// delay_ms(1000); //500
			for(i=0;i<5;i++)
			{
				// delay_ms(100); //500
				//USART2_RX_STA =0;  86
				DB_PR("-------i=%d---------\n",i);
				memset(regst_key_post,0,sizeof(regst_key_post));
				sprintf(regst_key_post,"AT+QHTTPPOST=%d,80,80",46+strlen(regst_key));
				//sim900a_send_cmd(regst_key_post,"CONNECT",500);// != GSM_TRUE) return GSM_FALSE;//"OK"
				if(0==sim900a_send_cmd(regst_key_post,"CONNECT",1200))//>6s
				{
					DB_PR("...a-10-1...\n");
				}
				else
				{
					DB_PR("...a-10-2 err...\n");
					continue;
				}
				
				
				DB_PR("...a-11...\n");

				//delay_ms(1000); //500




				memset(regst_key_post,0,sizeof(regst_key_post));
				//Content-Type:application/x-www-form-urlencoded\r\n
				//Content-Type=application/x-www-form-urlencoded
				sprintf(regst_key_post,"from=cabinet&register_key=%s&type=qrcode_content",regst_key);//

				// sprintf(regst_key_post,"from=cabinet&register_key=%s&type=qrcode_content",regst_key);
				// DB_PR("strlen(regst_key_post)=%d\n",strlen(regst_key_post));
				uart0_debug_str(regst_key_post,strlen(regst_key_post));
				// uart0_debug_data_h(regst_key_post,strlen(regst_key_post));
				//sim900a_send_cmd(regst_key_post,"OK",1500);
				// sim900a_send_cmd("from=cabinet&register_key=register:7c772404a1fda38b4f0a42b8f013ae2&type=qrcode_content","OK",12000);
				if(0==sim900a_send_cmd_tou_data(regst_key_post,"+QHTTPPOST:",900))
				{
					DB_PR("...a-11-1...\n");

				}
				else
				{
					DB_PR("...a-11-2  err...\n");
					continue;
				}
				DB_PR("...a-12...\n");

				
				//delay_ms(1000); //500
				//delay_ms(1000); //500
				// delay_xs(30);

				//reg_status3 = sim_at_response_https(1);//检查GSM模块发送过来的数据,及时上传给电脑
				if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD:",900))// != GSM_TRUE) return GSM_FALSE;//"OK"
				{ 
					DB_PR("...a-13...\n");
					// if(USART2_RX_STA&0X8000)		//接收到一次数据了
					{ 
						// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
						DB_PR("%s",USART2_RX_BUF);	//发送到串口

						if(1==cjson_to_struct_info_qrcode((char*)USART2_RX_BUF))
						{
							DB_PR("...a-13-2...\n");
							break;
						}
						else
						{
							reg_status =0xf1;//add
							DB_PR2("...a-13-2 e  ...\n");
						}
						
						// USART2_RX_STA=0;
					}
					USART2_RX_STA=0;//----------------
					memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);

					// cJSON_Delete(root);
					// return reg_status;
				} 
				else
				{
					DB_PR("...a-13-2 err...\n");
					continue;
				}

				// send_cmd_to_lcd_pic(0x0003);//---------------
			}

		}


	}

	cJSON_Delete(root);
	return reg_status;

}







// u16 sim_at_response_https(u8 mode)
// {
// 	u16 reg_status2=0x000f;
// 	if(USART2_RX_STA&0X8000)		//接收到一次数据了
// 	{ 
// 		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
// 		DB_PR("%s",USART2_RX_BUF);	//发送到串口


// 		reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
		
// 		//cjson_dbg();

// 		if(mode)
// 			USART2_RX_STA=0;
// 	} 
// 	return reg_status2;
// }




int captcha_id=0;


u32 cjson_to_struct_info_overtime_pay(char *text)
{
	u32 my_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson对象

	cJSON * item2 = NULL;//cjson对象
	
	cJSON * item3 = NULL;//cjson对象
	int i=0;


	u32 reg_active_code =0;
	uint8_t buff_t[256]={0};
	//uint8_t buff_t2[256]={0};

	char* buff_t3="b5";
	int my_bl_ts=0;
	//单片机测试
	// uint8_t buff_t3[256]={0xb5 ,0xa5 ,0xc6 ,0xac ,0xbb ,0xfa ,0xb2 ,0xe2 ,0xca ,0xd4 ,0x32};
	
	int company_id=0;
	char* url_t="https://iot.xintiangui.com/web_wechat/download_app?cid=";

	char qhttp_post_req[150]={0};

	char regst_key_post[300];
	u16 index_m=0;

	int fee_money=0;

	// const char needle[10] = "\r\n";
	// char *ret;
	
	// //设备注册成功,请激活柜子
	// char* str_tmp="{\"status\":0,\"msg\":\"abc\",\"result\":{\"active_code\":87993541}}";
	
	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //截取有效json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');
    // char *index=strstr(text,"{\"post_data\":{");
    // bzero(text, sizeof(text));
    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);
	// 	memset(text,0,89);
	// 	memcpy(text,index,89);
	// 	text[89]='\0';

	// DB_PR("---strlen(text)= %d\n", strlen(text));
	// DB_PR("---89= %02x\n", text[89]);
	// DB_PR("---90= %02x\n", text[90]);
 
	// for(i=89;i<strlen(text);i++)
	// {
	// 	text[i]=0;
	// }


//    ret = strstr(text, needle);
	DB_PR("\n----2----text=\n%s\n",text);










	// DB_PR("str_tmp=%s",str_tmp);


    root = cJSON_Parse(text);     
    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
        DB_PR("%s\n", "有格式的方式打印Json:");           
        // DB_PR("%s\n\n", cJSON_Print(root));
        // DB_PR("%s\n", "无格式方式打印json：");
        // DB_PR("%s\n\n", cJSON_PrintUnformatted(root));





		DB_PR("%s\n", "获取money fee 下的cjson对象");
		item = cJSON_GetObjectItem(root, "status");
		DB_PR("-----------%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
		DB_PR("%d\n", item->valueint);
		my_status = item->valueint;//status ==0

		//////if(my_status ==0)
		{
			//-----------company------------
			DB_PR("%s\n", "获取result下的cjson对象:");
			item2 = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "获取 qrcode 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "qrcode");
			DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%s\n", item->valuestring);
			


			memset(buff_t,0,256);
			strcpy(buff_t,item->valuestring);
			// sprintf(buff_t,"%s%d",url_t,company_id);

			index_m = strlen(buff_t);
			DB_PR("--index_m=%d--\n", index_m);
			buff_t[index_m]=0xff;
			buff_t[index_m+1]=0xff;

			
			uart0_debug_data_h(buff_t,256);


			send_cmd_to_lcd_bl_len(0x4000,(uint8_t*)buff_t,128+4);//gekou 33 +3






			DB_PR("%s\n", "获取money fee 下的cjson对象");
			item = cJSON_GetObjectItem(item2, "fee");
			DB_PR("-----------%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
			DB_PR("%d\n", item->valueint);
			fee_money = item->valueint;


			memset(buff_t,0,256);
			// itoa((int)(fee_money),(char*)(buff_t) ,10);

			DB_PR(" ((double)fee_money)/100=%f\n", ((double)fee_money)/100);
			sprintf(buff_t,"%.2f", ((double)fee_money)/100 ); 

			send_cmd_to_lcd_bl_len(0x1960,(uint8_t*)buff_t,32+4);//gekou 33 +3

			

			memset(buff_t,0,256);
			mtimer_flag =2;
			daojishi_time=30;
			TIM5_Set(1);//------------------
			sprintf((char*)buff_t, "%d", daojishi_time);
			DB_PR("-------daojishi_time  buff_t=%s--------\n",buff_t);
			send_cmd_to_lcd_bl_len(0x1950,buff_t,10+4);


			send_cmd_to_lcd_pic(0x0007);//-------pay chaoshsi yemian--------
			daojishi_ongo_flag =1;//-------------------
			DB_PR2("----1-7---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
		}


    }



    cJSON_Delete(root);
    return my_status;

}











int  heart_beart_idx =0;//0 or 1





u32 cjson_to_struct_info_tcp_rcv_overtime_pay_success(char *text)
{
	u32 ret_status=0x000f;
	char *index;
	//cJSON * root = NULL;
	//cJSON * item = NULL;//cjson???ó
	char regst_key_post[300];
//	cJSON * item2 = NULL;//cjson???ó
//	cJSON * item3 = NULL;//cjson???ó
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[40]={0};
	int i=0;
	u16 index_m=0;
	int16_t guimen_gk_temp =0;
	
	int size=0;

	char qhttp_post_req[150]={0};

	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //???????§json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);

	DB_PR("\n----2----text=\n%s\n",text);

	if(strstr(text,"stc:overtime_pay_success"))
	{
		DB_PR2("\n----------stc:overtime_pay_success------------\n");

		send_cmd_to_lcd_pic(0x0008);
		// daojishi_time=2;
		// TIM5_Set(1);

		daojishi_ongo_flag =0;
		ret_status =1;
		DB_PR("...stc:overtime_pay_success...\n");
	}
	else
	{
		DB_PR("...stc:overtime_pay_    fail...\n");
	}
	


	printf("ret_status=%d\n",ret_status);


    // root = cJSON_Parse(text);     
    // DB_PR("\n----3----\n");

    // if (!root) 
    // {
    //     DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    // }
    // else
    // {

	// 	//---------------------
	// 	DB_PR("%s\n", "获取type下的cjson对象");
	// 	item = cJSON_GetObjectItem(root, "type");//
	// 	DB_PR("--1--%s:", item->string);   //??????cjson???ó???á???????????????±??????
	// 	DB_PR("--2--%s\n", item->valuestring);
	// 	// reg_status = item->valueint;
	// 	// DB_PR("%s\n", cJSON_Print(item));
		




	// 	daojishi_ongo_flag =0;
	// 	DB_PR("----1-8c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
	// 	// if(NULL!=strstr(item->valuestring, "stc:overtime_pay_success"))
	// 	if(0==strcmp("stc:overtime_pay_success",item->valuestring))
	// 	{
	// 		send_cmd_to_lcd_pic(0x0008);
	// 		// daojishi_time=2;
	// 		// TIM5_Set(1);

	// 		ret_status =1;
	// 		DB_PR("...stc:overtime_pay_success...\n");
	// 	}
	// 	else
	// 	{
	// 		DB_PR("...stc:overtime_pay_    fail...\n");
	// 	}
		


	// 	printf("ret_status=%d\n",ret_status);
		
		
    //         //  uart0_debug_data_h(buff_t,256);
    //         // send_cmd_to_lcd_bl_len(0x2000,(uint8_t*)buff_t,128+4);//gekou 33 +3

    // }



    // cJSON_Delete(root);
    return ret_status;

}

u32 my_ret_status;

u32 cjson_to_struct_info_tcp_rcv(char *text,u8 clear_mode)
{
	u32 reg_status=0x000f;
	char *index;
	char index_2[1024]={0};
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson???ó
	char regst_key_post[300];
//	cJSON * item2 = NULL;//cjson???ó
//	cJSON * item3 = NULL;//cjson???ó
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[40]={0};
	int i=0;
	u16 index_m=0;
	int16_t guimen_gk_temp =0;
	
	int size=0;

	char qhttp_post_req[150]={0};

	char* temp_cjson=NULL;

	int cjson_len=0;
	u16 ret_value1=0xf1;

	//DB_PR2("---0---reg_status=%x---------\n",reg_status);
    if( text == NULL)
    {
        DB_PR2("\n----1 err----text=\n%s\n",text);
		// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
		// USART2_RX_STA =0;		
		reg_status = 0xffff;
		DB_PR2("---1---reg_status=%x---------\n",reg_status);
        return reg_status;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

	DB_PR("\n\n************************tcp_rcv sssss************************\n");


    //???????§json
    // DB_PR2("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR2("------NULL----4444----------\n");
		// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
		// USART2_RX_STA =0;	
		reg_status = 0xffff;
		DB_PR2("---2---reg_status=%x---------\n",reg_status);
        return reg_status;
    }
	//index_2 = index;
    strcpy(index_2,index);

	DB_PR("\n----2----index_2=\n%s\n",index_2);


	if(clear_mode == 0)
	{
		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
		USART2_RX_STA =0;	
	}






    root = cJSON_Parse(index_2);     


    DB_PR("\n----3----\n");

    if (!root) 
    {
        DB_PR2("Error before: [%s]\n",cJSON_GetErrorPtr());
		// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
		// USART2_RX_STA =0;	
    }
    else
    {
			IWDG_Feed();
			DB_PR2("\n----TCP JSON IS ON----\n");
			temp_cjson =cJSON_PrintUnformatted(root);
			DB_PR("---------%s---------\n", "无格式方式打印json：");
			DB_PR("---------\n%s\n------------\n\n",temp_cjson );
			
			cjson_len = strlen(temp_cjson);
			// if(temp_cjson)
			{
				DB_PR("create js string is %s\n",temp_cjson);
				free(temp_cjson);//------------must have---------------
			}
			DB_PR("---------cjson_len= %d---------\n\n",cjson_len );
			DB_PR("---------strlen(index_2)= %d------\n\n",strlen(index_2) );


			
			
			//---------------------
			DB_PR("%s\n", "获取type下的cjson对象");
			item = cJSON_GetObjectItem(root, "type");//
			DB_PR("--1--%s:", item->string);   //??????cjson???ó???á???????????????±??????
			DB_PR("--2--%s\n", item->valuestring);
			// reg_status = item->valueint;
			// DB_PR("%s\n", cJSON_Print(item));
			

			// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
			// USART2_RX_STA =0;
			if(0==strcmp("stc:restart",item->valuestring))
			{
				//---------------------
				DB_PR("----------tcp will restart---------\n");   
				Soft_Reset();//
			}
			// else if(0==strcmp("stc:overtime_pay_success",item->valuestring))
			// {
			// 	daojishi_ongo_flag =0;
			// 	send_cmd_to_lcd_pic(0x0008);
			// 	daojishi_time=2;
			// 	TIM5_Set(1);

			// 	DB_PR("...stc:overtime_pay_success...\n");
			// }
			else if(0==strcmp("stc:opendoor",item->valuestring))
			{
				// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);//-----------------
				// USART2_RX_STA=0;
				reg_status =1;
				my_ret_status = 1;
				DB_PR2("---3a---reg_status=%x---------\n",reg_status);
				//---------------------
				DB_PR("----------tcp opendoor---------\n");   
				DB_PR("\n%s\n", "--2--一步一步的获取 door_number 键值对:");


				DB_PR("----2-1---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
				if(1== daojishi_ongo_flag)
				{
					
					// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);//-----------------
					// USART2_RX_STA=0;

					for(i=0;i<15;i++)//30
					{
						IWDG_Feed();
						DB_PR("------i=%d----\n",i);   
						delay_ms(100); //500
						// if(USART2_RX_STA&0X8000)		//接收到一次数据了
						DB_PR("----2-2---USART2_RX_STA=%d\n",USART2_RX_STA);
						if(USART2_RX_STA>0)
						{
							delay_ms(5);
							// DB_PR("--------USART2_RX_BUF=sssssssssssss\n-------");
							// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
							DB_PR("--------timeout dbg1--------4G_UART_RCV=---------------------\r\n");
							// uart0_debug_data_h(data_rx_t,len_rx_t);
							DB_PR("%s",USART2_RX_BUF);	//发送到串口


							if(1==cjson_to_struct_info_tcp_rcv_overtime_pay_success((char*)USART2_RX_BUF))
							{
								daojishi_ongo_flag =0;
								DB_PR("----1-9c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
								DB_PR("--------pay success dbg2 ok---------------------\r\n");
								// USART2_RX_STA=0;
								break;
							}
							else
							{
								DB_PR("--------timeout dbg3 err---------------------\r\n");
							}
							

							DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");
						} 
						else
						{
							DB_PR("----------USART2_RX_STA =0----------\n\n");
						}
						

					}
					
					DB_PR("----my dbg-----i=%d---------\n",i);   
					if(i==30)//没有收到支付成功
					{
						DB_PR("----my dbg3 timeout-----i=%d---------\n",i);   
						
						DB_PR("----1-10c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
						// daojishi_time=30;
						// TIM5_Set(1);		

						// return //------------	
					}
					// delay_ms(500); //500
					// daojishi_ongo_flag =0;//---------can not-----------
					
				}//2-1



				IWDG_Feed();
				// heart_beart_idx++;
				// DB_PR2("-2-heart_beart_idx=%d\r\n",heart_beart_idx);


				DB_PR("%s\n", "获取 door_number 下的cjson对象");
				item = cJSON_GetObjectItem(root, "door_number");
				// DB_PR("%s\n", cJSON_Print(item));
				DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
				DB_PR("%d\n", item->valueint);
				guimen_gk_temp = item->valueint;
				DB_PR2("--------------open lock-----------guimen_gk_temp=%d\n", guimen_gk_temp);


				item = cJSON_GetObjectItem(root, "order_ary");
				// DB_PR("%s\n", cJSON_Print(item));
				item = cJSON_GetObjectItem(item, "data");
				//DB_PR("%s\n", cJSON_Print(item));//---------------------------
				size = cJSON_GetArraySize(item);
				DB_PR("--------size=%d-----------\n",size);
				// fprintf(stdout, "key: %s:", "value2");

				for (i = 0; i < size; ++i) {
					cJSON* tmp = cJSON_GetArrayItem(item, i);
					buff_t[i] = tmp->valueint;
					// fprintf(stdout, " %f,", tmp->valuedouble);
					DB_PR("buff_t[%d]=%02x\n",i, buff_t[i]);
				}


				

				RS485_TX_EN();
				DB_PR("buff_t=");
				uart0_debug_data_h(buff_t, size);
				// uart_write_bytes(UART_NUM_LOCK, (const char *) tx_Buffer2, 11);
				Usart_SendArray( USART3,buff_t, size);//open door-------------------------------
				RS485_RX_EN();

				DB_PR2("\n******************************lock open*******************************\n\n");  
				// buff_t2[0] = guimen_gk_temp/100 +0x30;
				// buff_t2[1] = guimen_gk_temp%100/100 +0x30;
				// buff_t2[2] = guimen_gk_temp%10 +0x30;



				
				IWDG_Feed();




				if(0==daojishi_ongo_flag) //1qujianma,kuaidiyuan    2-2 chaoshi
				{
					qujianma_wait_tcp_flag =0;
					DB_PR("\n----------no daojishi yemian-----------\n");  
					itoa((int)(guimen_gk_temp),(char*)(buff_t2) ,10);
					// send_cmd_to_lcd_bl(0x1650,buff_t2);
					send_cmd_to_lcd_bl_len(0x1650,(uint8_t*)buff_t2,32+4);
					send_cmd_to_lcd_pic(0x0006); //kaimen ok

					// daojishi_ongo_flag =0;
					DB_PR("----1-11c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
					daojishi_time=5;
					TIM5_Set(1);
				}
				else//kuai diyuan toujian
				{
					// send_cmd_to_lcd_pic(0x0001);
					// delay_ms(500); 
					// send_cmd_to_lcd_pic(0x0003);  
					// daojishi_ongo_flag =0;
					DB_PR("\n----------daojishi ongo, kuai diyuan toujian-----------\n");  
				}
				
				printf("-----daojishi_time=%d-----\n",daojishi_time);
				DB_PR("----2-2---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
				

			}
			else if(0==strcmp("stc:overtime_pay",item->valuestring))
			{
				reg_status =1;
				my_ret_status = 1;
				DB_PR2("---3b---reg_status=%x---------\n",reg_status);
				send_cmd_to_lcd_pic(0x000a); //------------------
				daojishi_ongo_flag =0;
				DB_PR("----1-12c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
                daojishi_time=30;
                TIM5_Set(0);


				DB_PR("%s\n", "获取 captcha_id 下的cjson对象");
				item = cJSON_GetObjectItem(root, "captcha_id");
				// DB_PR("%s\n", cJSON_Print(item));
				DB_PR("%s:", item->string);   //看一下cjson对象的结构体中这两个成员的意思
				DB_PR("%d\n", item->valueint);
				captcha_id = item->valueint;
				DB_PR("-----captcha_id=%d\n", captcha_id);


				IWDG_Feed();

                //--------http----------------
				//-----------add  panduan qujianma pingbi?  todo
                DB_PR("...a-0-0...\n");
                // delay_ms(1000); //500
                delay_ms(1000); //500
                sim900a_send_cmd_tou_data("+++",0,0);//AT  
                // sim900a_send_cmd("+++","OK",3000);//AT
                DB_PR("...a-0-1...\n");
                delay_ms(1000); //500
                // delay_ms(1000); //500
	
				// heart_beart_idx++;
				// DB_PR2("-3-heart_beart_idx=%d\r\n",heart_beart_idx);
				for(i=0;i<2;i++)
				{
					IWDG_Feed();
					DB_PR("-------i=%d---------\n",i);
					//----------------------------
					sim900a_send_cmd("AT+QHTTPURL=70,80","CONNECT",600);// != GSM_TRUE) return GSM_FALSE;//"OK"
					DB_PR("...a-9...\n");


					//2-4
					sim900a_send_cmd_tou_data("http://xintian.modoubox.com/api_cabinet/Deliverorder/getOvertimeQrcode","OK",600);
					DB_PR("...a-10...\n");

					IWDG_Feed();

					//USART2_RX_STA =0;  86
					// memset(regst_key_post,0,sizeof(regst_key_post));
					memset(regst_key_post,0,sizeof(regst_key_post));
					sprintf(regst_key_post,"captcha_id=%d&register_key=%s",captcha_id,regst_key);//
					uart0_debug_str(regst_key_post,strlen(regst_key_post));

					DB_PR("strlen(regst_key_post)=%d\n",strlen(regst_key_post));

					// //USART2_RX_STA =0;
					// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
					// DB_PR("...a-11-1...\n");





					// delay_ms(100); //500


					sprintf(qhttp_post_req,"AT+QHTTPPOST=%d,80,80",strlen(regst_key_post));
					// sim900a_send_cmd(qhttp_post_req,"CONNECT",15000);// != GSM_TRUE) return GSM_FALSE;//"OK"
					if(0==sim900a_send_cmd(qhttp_post_req,"CONNECT",800))
					{
						DB_PR("...a-10-1...\n");
					}
					else
					{
						DB_PR("...a-10-2 err...\n");
						continue;
					}
					// sim900a_send_cmd("AT+QHTTPPOST=99,80,80","CONNECT",125000);
					DB_PR("...a-11...\n");

					IWDG_Feed();
					// #define POST_DATA_OPENDOOR "code=12345678&type=get_by_code&from=code-user&register_key=register:7c772404a1fda38b4f0a42b8f013ae2"
					uart0_debug_data_h(regst_key_post,strlen(regst_key_post));
					if(0==sim900a_send_cmd_tou_data(regst_key_post,"+QHTTPPOST:",500))
					{
						DB_PR("...a-11-1...\n");
						// if(NULL!=strstr(USART2_RX_BUF,"+QHTTPPOST:"))

					}
					else
					{
						DB_PR("...a-11-2  err...\n");
						continue;
					}
					// sim900a_send_cmd(POST_DATA_OPENDOOR,"OK",1000);
					
					DB_PR("...a-12...\n");

					IWDG_Feed();
					// delay_ms(50);

					//reg_status3 = sim_at_response_https(1);//检查GSM模块发送过来的数据,及时上传给电脑
					// if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD",500))
					if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD:",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
					{ 
						// delay_ms(100);
						DB_PR("...a-13...\n");
						// if(USART2_RX_STA&0X8000)		//接收到一次数据了
						{ 
							// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
							DB_PR("%s",USART2_RX_BUF);	//发送到串口

							//send_cmd_to_lcd_pic(0x0007);
							ret_value1=cjson_to_struct_info_overtime_pay((char*)USART2_RX_BUF);
							if((0x000f!=ret_value1)&&(0xffff!=ret_value1))//
							{
								qujianma_wait_tcp_flag =0;
								DB_PR("...a-13-1-1 ok...\n");
								break;
							}
							else
							{
								DB_PR("...a-13-2-2 err...\n");
								//
							}
							
							// USART2_RX_STA=0;
						}
						// cJSON_Delete(root);
						// return reg_status;
						// break;
					} 
					else
					{
						DB_PR("...a-13-2 err...\n");
						continue;
					}
				}

				DB_PR("----zhifu timeout?---i=%d---------\n",i);
				if((i==2)&&( (0x000f==ret_value1)||(0xffff==ret_value1) || (0xf1==ret_value1) )   )
				{
					DB_PR("...b-http timeout...\n");
					send_cmd_to_lcd_pic(0x0001);
					delay_ms(1000); 
					send_cmd_to_lcd_pic(0x0003);
				}
				else
				{
					DB_PR("...b-ok...\n");
				}




                // delay_ms(1000); //500
				// sim900a_send_cmd("AT+QISWTMD=0,2","CONNECT",1);
				// sim900a_send_cmd("AT+QISWTMD=0,2",0,0);
				// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
				// USART2_RX_STA=0;
				if(0==sim900a_send_cmd("AT+QISWTMD=0,2","CONNECT",300))
				{
					DB_PR2("...c-14-2 ...\n");
					DB_PR2("..c1.USART2_RX_STA=%d...\n",USART2_RX_STA);
					DB_PR2("-c1--USART2_RX_BUF----\n%s\n---------",USART2_RX_BUF);	
				}
				else
				{
					DB_PR2("...c-14-2 e...\n");
				}
			}
			else if(0==strcmp("stc:heartbeat",item->valuestring))
			{
				//---------------------
				DB_PR("----------stc:heartbeat---------\n");   
				heart_beart_idx++;
				DB_PR2("heart_beart_idx=%d\r\n",heart_beart_idx);

				IWDG_Feed();
			}
			else
			{
				DB_PR("------tcp other-------\n");
			}
			
			

			DB_PR("----2-----cjson_len= %d---------\n\n",cjson_len );
			DB_PR("----2-----strlen(index_2)= %d------\n\n",strlen(index_2) );
			//{"type":"stc:heartbeat","time":1606975970061}
			if(strlen(index_2)==cjson_len)
			{
				DB_PR("\n----1-1==   always----\n");
			}
			else//>
			{
				DB_PR("\n----1-2!=----\n");
				// cJSON_Delete(root);
				// cjson_to_struct_info_tcp_rcv(index_2);
				cjson_to_struct_info_tcp_rcv(index_2+cjson_len, clear_mode);
				
				DB_PR("\n----2----strlen(index_2+cjson_len)=%d\n",strlen(index_2+cjson_len));
				DB_PR("\n----2----index_2+cjson_len=\n%s\n",index_2+cjson_len);
			}

    }
	DB_PR("\n************************tcp_rcv eeeeee************************\n\n");


    cJSON_Delete(root);


    return reg_status;
}

void sim_at_response(u8 mode)
{
	int ret_status2=0;
	// if(USART2_RX_STA&0X8000)		//-------sim_at_response----------
	if(USART2_RX_STA!=0)//--------------------
	{ 
		delay_ms(15);//>10ms 确保数据接收完 20 not
		DB_PR("USART2_RX_BUF=sssssssssssss\n");
		// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		DB_PR("--4G_UART_RCV=--\r\n");
        // uart0_debug_data_h(data_rx_t,len_rx_t);
		DB_PR("TCP RCV---\n%s\n----",USART2_RX_BUF);	//发送到串口

		// USART2_RX_STA =0;//------------------
		ret_status2 = cjson_to_struct_info_tcp_rcv((char*)USART2_RX_BUF, 0);
		if((0x000f==ret_status2)||(0xffff==ret_status2))
		{
			memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
			USART2_RX_STA =0;			
		}


		// reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
		
		//cjson_dbg();

		// if(mode)
		// 	USART2_RX_STA=0;

		DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A 各项测试(拨号测试、短信测试、GPRS测试)共用代码

//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	// if(USART2_RX_STA&0X8000)		//接收到一次数据了
	if(USART2_RX_STA!=0)
	{ 
		// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);

		if((u8*)strx)
		{
			DB_PR("-------1a len=%d--------\r\n",USART2_RX_STA);
			DB_PR("-------1a buff=\n%s--------\r\n",USART2_RX_BUF);
			DB_PR("-------1b ack=\n%s--------\r\n",str);
		}
		else
		{
			// DB_PR("-------waiting--------\r\n");
		}
		
	} 
	// DB_PR("-------2 (u8*)strx=%d--------\r\n\n\n",(u8*)strx);
	return (u8*)strx;
}




//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd_tou_data(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s",cmd);//发送命令
		DB_PR("send to 4G data=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			
			// if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			if(USART2_RX_STA!=0)
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				// USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}

	// USART2_RX_STA = 0;	//标记接收完成
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
	// TIM4_Set(0);			//关闭TIM4  

	delay_ms(20);
	return res;
} 


//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s\r\n",cmd);//发送命令		
		DB_PR("sendto 4G cmd=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			// if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			if(USART2_RX_STA!=0)
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				// USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}

	// USART2_RX_STA = 0;	//标记接收完成
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
	// TIM4_Set(0);			//关闭TIM4  

	// delay_ms(20);
	return res;
} 

u8 sim900a_send_cmd_noclean(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	// USART2_RX_STA=0;
	// memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);

	// if((u32)cmd<=0XFF)
	// {
	// 	while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
	// 	USART2->DR=(u32)cmd;
	// }
	// else 
	{
		u2_printf("%s\r\n",cmd);//发送命令		
		DB_PR("sendto 4G cmd=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			// if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			if(USART2_RX_STA!=0)
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				// USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}

	// USART2_RX_STA = 0;	//标记接收完成
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
	// TIM4_Set(0);			//关闭TIM4  

	// delay_ms(20);
	return res;
} 


#if 0
//add
u8 sim900a_send_cmd_tou_data_noclean(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);

	TIM4_Init(4999,7199);		//500ms    10ms中断  99
	// TIM4_Init(1499,7199);		//150ms    10ms中断  99
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除更新中断请求位 add
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断 add
	TIM4_Set(0);			//关闭定时器4





	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s\r\n",cmd);//发送命令		
		DB_PR("sendto 4G cmd=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}

	// USART2_RX_STA = 0;	//标记接收完成
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
	// TIM4_Set(0);			//关闭TIM4  



	TIM4_Init(499,7199);		//50ms    10ms中断  99
	// TIM4_Init(1499,7199);		//150ms    10ms中断  99
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除更新中断请求位 add
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断 add
	TIM4_Set(0);			//关闭定时器4


	return res;
} 


//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd_noclean(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN);

	TIM4_Init(4999,7199);		//50ms    10ms中断  99
	// TIM4_Init(1499,7199);		//150ms    10ms中断  99
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除更新中断请求位 add
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断 add
	TIM4_Set(0);			//关闭定时器4

	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s\r\n",cmd);//发送命令		
		DB_PR("sendto 4G cmd=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}

	// USART2_RX_STA = 0;	//标记接收完成
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
	// TIM4_Set(0);			//关闭TIM4  



	TIM4_Init(499,7199);		//50ms    10ms中断  99
	// TIM4_Init(1499,7199);		//150ms    10ms中断  99
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除更新中断请求位 add
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断 add
	TIM4_Set(0);			//关闭定时器4

	return res;
} 
#endif

//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
//unicode gbk 转换函数
//src:输入字符串
//dst:输出(uni2gbk时为gbk内码,gbk2uni时,为unicode字符串)
//mode:0,unicode到gbk转换;
//     1,gbk到unicode转换;
void sim900a_unigbk_exchange(u8 *src,u8 *dst,u8 mode)
{
	u16 temp; 
	u8 buf[2];
	if(mode)//gbk 2 unicode
	{
		while(*src!=0)
		{
			if(*src<0X81)	//非汉字
			{
				temp=(u16)ff_convert((WCHAR)*src,1);
				src++;
			}else 			//汉字,占2个字节
			{
				buf[1]=*src++;
				buf[0]=*src++; 
				temp=(u16)ff_convert((WCHAR)*(u16*)buf,1); 
			}
			*dst++=sim900a_hex2chr((temp>>12)&0X0F);
			*dst++=sim900a_hex2chr((temp>>8)&0X0F);
			*dst++=sim900a_hex2chr((temp>>4)&0X0F);
			*dst++=sim900a_hex2chr(temp&0X0F);
		}
	}else	//unicode 2 gbk
	{ 
		while(*src!=0)
		{
			buf[1]=sim900a_chr2hex(*src++)*16;
			buf[1]+=sim900a_chr2hex(*src++);
			buf[0]=sim900a_chr2hex(*src++)*16;
			buf[0]+=sim900a_chr2hex(*src++);
 			temp=(u16)ff_convert((WCHAR)*(u16*)buf,0);
			if(temp<0X80){*dst=temp;dst++;}
			else {*(u16*)dst=swap16(temp);dst+=2;}
		} 
	}
	*dst=0;//添加结束符
} 




typedef enum{
    GSM_TRUE,
    GSM_FALSE,
    
}gsm_res_e;










char IMEI_cRes[100] = {0};	
char deviceid_decrypt_c[33];
u8 sim900a_gprs_test(void)
{
	unsigned char deviceid_decrypt[16]={0};
	char deviceid_decrypt_c2[50]={0};
	const u8 *port="8086";	//端口固定为8086,当你的电脑8086端口被其他程序占用的时候,请修改为其他空闲端口
	u8 mode=0;				//0,TCP连接;1,UDP连接
	u8 key;
	u8 timex=0; 
	long timex_t=0; 
	long timex_t2=0; 
	u16 timex_t3=0; 
	u16 timex_t4=0; 
	// u8 ipbuf[16]; 		//IP缓存
	u8 iplen=0;			//IP长度 

	char tcp_ip[20]={0};
	char tcp_ip2[16]={0};
	char at_tcp_ip[64]={0};
	char *ptr =NULL;
	const char needle[10] = "AT+CGSN\r\r\n";
	char *ret;


	int i;
	// unsigned char IMEI_cRes[] = "admin";//21232f297a57a5a743894a0e4a801fc3  


	MD5_CTX md5;

	u16 reg_status3=0x000f;

//	sim900a_gprs_ui();	//加载主界面
//	Show_Str(30+72,90,200,16,(u8*)modetbl[mode],16,0);	//显示连接方式	
//	Show_Str(30+40,130,200,16,(u8*)port,16,0);			//显示端口 	






// 	//GSM_CLEAN_RX();
// //	if(gsm_cmd("AT+CIPCLOSE=1\r","OK",200) != GSM_TRUE)
// 	sim900a_send_cmd("AT+QICLOSE=0\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-1...\n");

// 	//GSM_CLEAN_RX();
// 	sim900a_send_cmd("AT+QIDEACT=1\r\n","OK",400) ;//!= GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a0...\n");
	DB_PR2("\r\n-1-心甜智能柜\r\n");

chengxu_start_1:
	while(1)
	{
		delay_ms(1000); //500
		if(0==sim900a_send_cmd("AT","OK", 100) )//!= GSM_TRUE) return GSM_FALSE;
		{
			DB_PR("...a0-1...\n");
			break;
		}
		else
		{
			DB_PR("...a0-2 wait...\n");		
		}
	}

	
	DB_PR2("\r\n-2-心甜智能柜\r\n");



///////////////////////////////////////////////////////////////////
	delay_xs(4); 

chengxu_start_2:
	//GSM_CLEAN_RX();  SIM READY?    2s
	if(0==sim900a_send_cmd("AT+CPIN?","+CPIN: READY", 200) )//!= GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a1-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a1-2 err...\n");		
		delay_xs(1); 
		Soft_Reset();
	}

	

	DB_PR("\r\n-2-1-心甜智能柜\r\n");


	// //GSM_CLEAN_RX();
	// sim900a_send_cmd("AT+CSQ\r\n","+CSQ:", 150);// != GSM_TRUE) return GSM_FALSE;
	// DB_PR("...a2...\n");
	
	//
	//GSM_CLEAN_RX();+CREG: 0,1   todo
	delay_ms(100);
	if(0==sim900a_send_cmd("AT+CREG?","OK", 200))// != GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a2-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a2-2 err...\n");		
		Soft_Reset();
	}
	
	delay_ms(100);
	//GSM_CLEAN_RX();+CGREG: 0,1
	if(0==sim900a_send_cmd("AT+CGREG?","OK", 150))// != GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a3-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a3-2 err...\n");		
	}
	
	



//----------------http-----------------------
	
	delay_ms(50);
	sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1","OK",200);
	// sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-1...\n");
	
	delay_ms(50);
	sim900a_send_cmd("AT+QIACT?","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-2...\n");
	
	delay_ms(50);
	sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1","OK", 200);
	//sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\","" ,"" ,1\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-3...\n");


	delay_ms(200);
	if(0==sim900a_send_cmd("AT+QIACT=1","OK",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
	{
		DB_PR("--1--\n%s\n-----\n",USART2_RX_BUF);
		DB_PR2("...a-4-1-1...\n");
	}
	else
	{
		delay_ms(2000);
		DB_PR2("...a-4-1-2 e...\n");
		DB_PR2("--2--\n%s\n-----\n",USART2_RX_BUF);
		if(0==sim900a_send_cmd("AT+QIDEACT=1","OK",400) )
		{
			DB_PR2("...a-4-1-2a...\n\n\n\n");
			goto chengxu_start_2;

		}
		else
		{
			DB_PR2("...a-4-1-2b err...\n\n\n\n\n\n\n");
			Soft_Reset();
		}

	}
	








	

	
	sim900a_send_cmd("AT+QIACT?","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-4-2-1...\n");



	// if(0==sim900a_send_cmd("AT+QPING=1,\"iot.dev.modoubox.com\"","+QPING:",500))
	// {
	// 	delay_ms(1000); //500
	// 	delay_ms(1000); //500
	// 	DB_PR("...b-1...\n");
	// 	DB_PR("%s",USART2_RX_BUF);	//发送到串口
	// }
	// else
	// {
	// 	DB_PR("...b-2 err...\n");
	// }
	



// +QIURC: "dnsgip",0,1,600

// +QIURC: "dnsgip","39.98.243.128"

	//---------287e9v3367.zicp.vip---------
	// AT+QIDNSGIP=1,"www.baidu.com" iot.dev.modoubox.com   express_tcp.xintiangui.com
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"www.baidu.com\"","+QIURC:",2000))//300
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"287e9v3367.zicp.vip\"","+QIURC:",800))
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"iot.dev.modoubox.com\"","+QIURC:",800))
	if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"express.tcp.xintiangui.com\"","+QIURC:",800))
	{
		delay_ms(50);
		DB_PR("...a4-3-1...\n");
		// if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{ 
			// DB_PR("USART2_RX_BUF=sssssssssssss\n");
			// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
			DB_PR("--4G_UART_RCV=--\r\n");
			// uart0_debug_data_h(data_rx_t,len_rx_t);
			DB_PR("USART2_RX_STA=%d\n",USART2_RX_STA);	//发送到串口
			DB_PR("USART2_RX_BUF=\n%s\n",USART2_RX_BUF);
			if((USART2_RX_STA) !=113)//(USART2_RX_STA&0x7FFF)
			{
				DB_PR("--IP chongxinhuoqu 1 e=--\r\n");
			}

			ptr = strrchr(USART2_RX_BUF, ',');
			if (ptr)
				DB_PR("The character ',' is at position: %s\n", ptr);
			else
			{
				DB_PR("The character was not found\n");
				DB_PR("--IP chongxinhuoqu 2 e=--\r\n");
				goto chengxu_start_2;
			}

			sprintf(tcp_ip,"%s",ptr+1);//39.98.243.128"

			memcpy(tcp_ip2,tcp_ip,strlen(tcp_ip)-2);
			DB_PR("tcp_ip2 =%s\n", tcp_ip2);

			uart0_debug_data_h(tcp_ip2,strlen(tcp_ip2));


			// ret_string_ip = strstr(haystack, needle);
			// cjson_to_struct_info_tcp_rcv((char*)USART2_RX_BUF);
			// DB_PR("tcp ip=\n%s\n");
			// reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
			//cjson_dbg();

			// if(mode)
			USART2_RX_STA=0;

			DB_PR("eeeeeeeeeeeee-4G\n\n");
		} 
	}
	else
	{
		// send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a4-3-2 err...\n");		
		goto chengxu_start_2;
	}









	sim900a_send_cmd("AT+QHTTPCFG=\"sslctxid\",1","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-5...\n");
	
	//1
	sim900a_send_cmd("AT+QSSLCFG=\"sslversion\",1,4","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-6...\n");

	//0x0005
	sim900a_send_cmd("AT+QSSLCFG=\"ciphersuite\",1,0xFFFF","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-7...\n");

	//0
	sim900a_send_cmd("AT+QSSLCFG=\"seclevel\",1,0","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-8...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"cacert\",1,\"RAM:cacert.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-1...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"clientcert\",1,\"RAM:clientcert.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-2...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"clientkey\",1,\"RAM:clientkey.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-3...\n");








// 	sim900a_send_cmd("AT+QHTTPURL=49,80\r\n","CONNECT",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-9...\n");

// 	sim900a_send_cmd("https://cabinet.u-xuan.com/api/control_app/status","OK",200);
// 	DB_PR("...a-10...\n");

// 	sim900a_send_cmd("AT+QHTTPPOST=73,80,80\r\n","CONNECT",8000);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-11...\n");

// #define http_upload_data     "device_id=00e5c9c6e22e66e2d32c22ef2cdb2a41&sim_iccid=898604641919c0808317"
//		sim900a_send_cmd("device_id=1417862573f5bbb40f299df52bc6e70e","OK",12000);
//
// 	sim900a_send_cmd(http_upload_data,"OK",200);
	// DB_PR("...a-12...\n");







chengxu_start_3:

	DB_PR("...a-12...\n");
	USART2_RX_STA =0;
	//imei
	if(sim900a_send_cmd("AT+CGSN","OK",500)==0)//查询产品序列号
	{ 
		delay_ms(100);//----------must----------
		// p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");//查找回车
		// p1[0]=0;//加入结束符 
		// sprintf((char*)p,"序列号:%s",USART2_RX_BUF+2);
 		DB_PR("---USART2_RX_BUF+2=%s\n", USART2_RX_BUF+2); 

		// if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{ 
			// DB_PR("USART2_RX_BUF=sssssssssssss\n");
			// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
			DB_PR("--4G_UART_RCV=--\r\n");
			// uart0_debug_data_h(data_rx_t,len_rx_t);
			DB_PR("---%s----\n",USART2_RX_BUF);	//发送到串口
			// DB_PR("---USART2_RX_STA&0x7FFF = %d----\n",USART2_RX_STA&0x7FFF); //33
        	uart0_debug_data_h(USART2_RX_BUF,USART2_RX_STA);

			// USART2_RX_STA=0;
			// DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");

			if((USART2_RX_STA) !=33)//(USART2_RX_STA&0x7FFF)
			{
				DB_PR("--IMEI chongxinhuoqu 1=--\r\n");
				goto chengxu_start_3;
			}



			
			ret = strstr(USART2_RX_BUF, needle);
			DB_PR("子字符串是： ----%s---\n", ret);
			if(ret == NULL)
			{
				DB_PR("--IMEI chongxinhuoqu 2=--\r\n");
				goto chengxu_start_3;
			}


			memset(IMEI_cRes,0,sizeof(IMEI_cRes));
			// uart0_debug_data_h2(USART2_RX_BUF,strlen(USART2_RX_BUF));
			// str_test(USART2_RX_BUF+2, IMEI_cRes);
			// memcpy(IMEI_cRes,USART2_RX_BUF+10,15);

			memcpy(IMEI_cRes,ret+10,15);
			DB_PR2("---111----%s------\n", IMEI_cRes); 




			MD5Init(&md5);
			MD5Update(&md5, IMEI_cRes, strlen((char *)IMEI_cRes));
			MD5Final(&md5, deviceid_decrypt);

			// //Md5加密后的32位结果
			// DB_PR("加密前:%s\n加密后16位:", IMEI_cRes);
			// for (i = 4; i<12; i++)//8*2
			// {
			// 	DB_PR("%02x", deviceid_decrypt[i]);  
			// }

			//Md5加密后的32位结果
			DB_PR("\n加密前:%s\n加密后32位:", IMEI_cRes);

			memset(deviceid_decrypt_c,0,sizeof(deviceid_decrypt_c));
			for (i = 0; i<16; i++)
			{
				DB_PR("%02x", deviceid_decrypt[i]); 
				// ltoa((int)(deviceid_decrypt[i]),(char*)(deviceid_decrypt_c+2*i) ,16);
				
				itoa((int)(deviceid_decrypt[i]/16),(char*)(deviceid_decrypt_c+i*2) ,16);
				itoa((int)(deviceid_decrypt[i]%16),(char*)(deviceid_decrypt_c+i*2+1) ,16);
				
			}
			DB_PR("\n-----------deviceid_decrypt_c=%s\n",deviceid_decrypt_c);
			deviceid_decrypt_c[32]=0;
			// uart0_debug_data_h2(deviceid_decrypt_c,32);

		} 

		
		// Show_Str(x,y+150,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	else
	{
		DB_PR("...a-12-3 err...\n");
		DB_PR("---USART2_RX_BUF=%s---\n", USART2_RX_BUF); 
		DB_PR("--IMEI chongxinhuoqu=--\r\n");
		goto chengxu_start_3;
	}
	
	// if(sim900a_send_cmd("AT+CNUM","+CNUM",500)==0)			//查询本机号码
	// { 
	// 	// p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
	// 	// p2=(u8*)strstr((const char*)(p1+2),"\"");
	// 	// p2[0]=0;//加入结束符
	// 	// sprintf((char*)p,"本机号码:%s",p1+2);
	// 	// DB_PR("-------------p=%s----------------",p);
	// 	// Show_Str(x,y+170,200,16,p,16,0);
	// 	// str_test(USART2_RX_BUF, IMEI_cRes);
	// 	DB_PR("---USART2_RX_BUF----%s---\n", USART2_RX_BUF); 
	// 	uart0_debug_data_h2(USART2_RX_BUF,strlen(USART2_RX_BUF));
	// 	DB_PR("---222---%s\n", IMEI_cRes); 

	// 	USART2_RX_STA=0;		
	// }
	// myfree(p); 













	
	DB_PR("...a-13...\n");

	sprintf(deviceid_decrypt_c2,"device_id=%s",deviceid_decrypt_c);
	DB_PR("--------------------deviceid_decrypt_c2=%s----------------------\n",deviceid_decrypt_c2);

	while(1)
	{
		sim900a_send_cmd("AT+QHTTPURL=49,80","CONNECT",800);
		// sim900a_send_cmd("AT+QHTTPURL=49,80\r\n","CONNECT",800);// != GSM_TRUE) return GSM_FALSE;//"OK"
		DB_PR("...a-9...\n");

		
		// sim900a_send_cmd_tou_data("https://iot.dev.modoubox.com/api/control_app/status","OK",800);
		sim900a_send_cmd_tou_data("https://iot.xintiangui.com/api/control_app/status","OK",800);
		DB_PR("...a-10...\n");


		// //USART2_RX_STA =0;
		// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
		// DB_PR("...a-11-1...\n");

		// delay_ms(100); //500



		//USART2_RX_STA =0;
		if(0==sim900a_send_cmd("AT+QHTTPPOST=42,80,80","CONNECT",1200))// != GSM_TRUE) return GSM_FALSE;//"OK"
		{
			DB_PR("...a-10-1...\n");
		}
		else
		{
			DB_PR("...a-10-2 err...\n");
			continue;
		}
		

		// delay_ms(100); //500
		// if(0==sim900a_send_cmd_tou_data("device_id=00e5c9c6e22e66e2d32c22ef2cdb2a42","OK",1000))//test
		if(0==sim900a_send_cmd_tou_data(deviceid_decrypt_c2,"+QHTTPPOST:",500))
		{
			DB_PR("...a-11-1...\n");
			// if(NULL!=strstr(USART2_RX_BUF,"+QHTTPPOST:"))

		}
		else
		{
			DB_PR("...a-11-2  err...\n");
			continue;
		}
		DB_PR("...a-12...\n");



		//reg_status3 = sim_at_response_https(1);//检查GSM模块发送过来的数据,及时上传给电脑
		if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD:",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
		{
		
			// if(USART2_RX_STA&0X8000)		//接收到一次数据了
			{ 
				// USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
				DB_PR("%s",USART2_RX_BUF);	//发送到串口

				DB_PR("...bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb...\n");
				reg_status3 = cjson_to_struct_info_register((char*)USART2_RX_BUF);//http2
				if(reg_status3 == 2)
				{
					DB_PR("...a-14   reg ok...\n");
					break;
				}
				//cjson_dbg();

				// if(mode)
				USART2_RX_STA=0;
			} 
		
		

		}

		delay_xs(10); 
	
	}






	// sim900a_send_cmd("AT+QISWTMD=1,2\r\n","OK",2000);

//-------------TCP-------------



	//TCP 2
	//UNINET CMNET
	//GSM_CLEAN_RX();
	//--------
	// sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n","OK", 100);// != GSM_TRUE) return GSM_FALSE;
	// DB_PR("...1...\n");
	
	//GSM_CLEAN_RX();
	// if(0==sim900a_send_cmd("AT+QIACT=1","OK", 100))// != GSM_TRUE) return GSM_FALSE;
	// {
	// 	DB_PR("...a-4-1-1 aa...\n");
	// }
	// else
	// {
	// 	DB_PR("...a-4-1-2 bb err...\n");
	// }

	DB_PR("...2...\n");
	
	
	
	
	USART2_RX_STA =0;
	//GSM_CLEAN_RX();register:53988b31ffdb2e7db9c9429b84f0f84  register:6ef6d30c49a08134897c6f2cc297063
	
	// express_tcp.xintiangui.com

	// sprintf(at_tcp_ip,"AT+QIOPEN=1,0,\"TCP\",%s,12310,0,2",tcp_ip2);
	sprintf(at_tcp_ip,"AT+QIOPEN=1,0,\"TCP\",%s,8091,0,2",tcp_ip2);
	DB_PR("-----------at_tcp_ip =%s--------------\r\n",at_tcp_ip);	
	
	//  if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"103.46.128.21\",12310,0,2","CONNECT", 1000)==0)//192.168.10.111
	//  if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"39.98.243.128\",8091,0,2",  "CONNECT", 1000)==0)
	// if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"47.94.2.173\",8091,0,2",  "CONNECT", 1000)==0)
	if(sim900a_send_cmd(at_tcp_ip,"CONNECT", 1000)==0)
	{
		DB_PR("----conn-----\r\n");
		send_cmd_to_lcd_pic(0x0003);//---------------kaiji
		daojishi_ongo_flag =0;
		DB_PR("----1-14c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
		daojishi_time=30;
		TIM5_Set(0);

		delay_ms(500); //500
			// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
		sim900a_send_cmd_tou_data(regst_key,0,0) ;
		// sim900a_send_cmd("register:43981c0ecf4dfadb2d9cc878c874ab8",0,0) ;
			// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);

	}
	else
	{
		DB_PR("----disconn-----\r\n");
		send_cmd_to_lcd_pic(0x0001);
		if(0==sim900a_send_cmd("AT+QICLOSE=0","OK",200))
		{
			DB_PR("----AT+QICLOSE ok-----\r\n");
		}
		if(0==sim900a_send_cmd("AT+QIDEACT=1","OK",200) )
		{
			DB_PR("----AT+QIDEACT ok-----\r\n");
		}
		goto chengxu_start_2;

	}
	

	//	//检测是否建立连接
	//	while(gsm_cmd_check("CONNECT") != GSM_TRUE)//OK
	//	{		
	//		if(++testConnect >200)//最长等待20秒   150s----------
	//		{
	//			return GSM_FALSE;
	//		}
	//		GSM_DELAY(100); 		
	//	}


	delay_ms(500); //500
	// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
	sim900a_send_cmd_tou_data("iot",0,0);	// cabinet_heartbeat
	// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);
	

	DB_PR2("\r\n-3-心甜智能柜\r\n");


	// IWDG_Init((4*4),(625*4));    //与分频数为64,重载值为625,溢出时间为  =64s	 
	IWDG_Init(7,4094);//26s 

// //  	// sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//关闭连接
// // 	// sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//关闭移动场景 
// // 	// if(sim900a_send_cmd("AT+CGCLASS=\"B\"","OK",1000))return 1;				//设置GPRS移动台类别为B,支持包交换和数据交换 
// // 	// if(sim900a_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000))return 2;//设置PDP上下文,互联网接协议,接入点等信息
// // 	// if(sim900a_send_cmd("AT+CGATT=1","OK",500))return 3;					//附着GPRS业务
// // 	// if(sim900a_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))return 4;	 	//设置为GPRS连接模式
// // 	// if(sim900a_send_cmd("AT+CIPHEAD=1","OK",500))return 5;	 				//设置接收数据显示IP头(方便判断数据来源)
// // 	// ipbuf[0]=0; 		
	
// 	USART4_RX_STA=0;
// 	delay_ms(100);
	
	while(1)
	{

		timex++;
		timex_t++;
		timex_t2++;
		timex_t3++;
		if(timex==50)
		{
			timex=0;
			LED0=!LED0;
		}

		if(timex_t3==300)//3s
		{
			timex_t3=0;
			IWDG_Feed();
			DB_PR("------------feed dog ongo-----------\n");	
		}

		if(timex_t==3000)//1min   6000
		{
			timex_t =0;
			// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
			sim900a_send_cmd_tou_data("iot",0,0);	
			// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);
			DB_PR("------------heart-----------\n");	
		}
		if(timex_t2==9500)//1min  30000/60=500=5min
		{
			timex_t2 =0;
			DB_PR("2-xintiao jc-heart_beart_idx=%d\r\n",heart_beart_idx);
			if(0==heart_beart_idx)
			{
				DB_PR("2-xintiao err\r\n");
				send_cmd_to_lcd_pic(0x0001);
				power_down_reset_ec20();
				// at_mode_go();
				// tcp_http_init();
				goto chengxu_start_1;
			}
			else
			{
				DB_PR("2-xintiao ok\r\n");
			}
			heart_beart_idx =0;
		}


		if(qujianma_wait_tcp_flag!=0)
		{
			DB_PR("2-qujianma_wait_tcp_flag wait=%d\r\n",qujianma_wait_tcp_flag);
			timex_t4++;
		}
		
		if((qujianma_wait_tcp_flag!=0)&&(timex_t4==1100))//11s
		{
			DB_PR("2-qujianma_wait_tcp_flag timeout ok\r\n");
			qujianma_wait_tcp_flag=0;
			timex_t4 =0;
			send_cmd_to_lcd_pic(0x0001);
			delay_ms(500); //500
			send_cmd_to_lcd_pic(0x0003);
		}

		delay_ms(10);
		

		sim_at_response(1);//2 检查GSM模块发送过来的数据,及时上传给电脑
		lcd_at_response(1);//4

		lock_at_response(1);
	}
	return 0;
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A GSM/GPRS主测试控制部分

//测试界面主UI
void sim900a_mtest_ui(u16 x,u16 y)
{
	u8 *p,*p1,*p2; 
	p=mymalloc(50);//申请50个字节的内存
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	Show_Str_Mid(0,y,"ATK-SIM900A 测试程序",16,240);  
	Show_Str(x,y+25,200,16,"请选择:",16,0); 				    	 
	Show_Str(x,y+45,200,16,"KEY0:拨号测试",16,0); 				    	 
	Show_Str(x,y+65,200,16,"KEY1:短信测试",16,0);				    	 
	Show_Str(x,y+85,200,16,"WK_UP:GPRS测试",16,0);
	POINT_COLOR=BLUE; 	
	USART2_RX_STA=0;
	if(sim900a_send_cmd("AT+CGMI","OK",200)==0)				//查询制造商名称
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");
		p1[0]=0;//加入结束符
		sprintf((char*)p,"制造商:%s",USART2_RX_BUF+2);
		Show_Str(x,y+110,200,16,p,16,0);
		USART2_RX_STA=0;		
	} 
	if(sim900a_send_cmd("AT+CGMM","OK",200)==0)//查询模块名字
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n"); 
		p1[0]=0;//加入结束符
		sprintf((char*)p,"模块型号:%s",USART2_RX_BUF+2);
		Show_Str(x,y+130,200,16,p,16,0);
		USART2_RX_STA=0;		
	} 
	if(sim900a_send_cmd("AT+CGSN","OK",200)==0)//查询产品序列号
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");//查找回车
		p1[0]=0;//加入结束符 
		sprintf((char*)p,"序列号:%s",USART2_RX_BUF+2);
		Show_Str(x,y+150,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	if(sim900a_send_cmd("AT+CNUM","+CNUM",200)==0)			//查询本机号码
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
		p2=(u8*)strstr((const char*)(p1+2),"\"");
		p2[0]=0;//加入结束符
		sprintf((char*)p,"本机号码:%s",p1+2);
		Show_Str(x,y+170,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	myfree(p); 
}
//GSM信息显示(信号质量,电池电量,日期时间)
//返回值:0,正常
//    其他,错误代码
u8 sim900a_gsminfo_show(u16 x,u16 y)
{
	u8 *p,*p1,*p2;
	u8 res=0;
	p=mymalloc(50);//申请50个字节的内存
	POINT_COLOR=BLUE; 	
	USART2_RX_STA=0;
	if(sim900a_send_cmd("AT+CPIN?","OK",200))res|=1<<0;	//查询SIM卡是否在位 
	USART2_RX_STA=0;  
	if(sim900a_send_cmd("AT+COPS?","OK",200)==0)		//查询运营商名字
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\""); 
		if(p1)//有有效数据
		{
			p2=(u8*)strstr((const char*)(p1+1),"\"");
			p2[0]=0;//加入结束符			
			sprintf((char*)p,"运营商:%s",p1+1);
			Show_Str(x,y,200,16,p,16,0);
		} 
		USART2_RX_STA=0;		
	}else res|=1<<1;
	if(sim900a_send_cmd("AT+CSQ","+CSQ:",200)==0)		//查询信号质量
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),":");
		p2=(u8*)strstr((const char*)(p1),",");
		p2[0]=0;//加入结束符
		sprintf((char*)p,"信号质量:%s",p1+2);
		Show_Str(x,y+20,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<2;
	if(sim900a_send_cmd("AT+CBC","+CBC:",200)==0)		//查询电池电量
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
		p2=(u8*)strstr((const char*)(p1+1),",");
		p2[0]=0;p2[5]=0;//加入结束符
		sprintf((char*)p,"电池电量:%s%%  %smV",p1+1,p2+1);
		Show_Str(x,y+40,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<3; 
	if(sim900a_send_cmd("AT+CCLK?","+CCLK:",200)==0)		//查询电池电量
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\"");
		p2=(u8*)strstr((const char*)(p1+1),":");
		p2[3]=0;//加入结束符
		sprintf((char*)p,"日期时间:%s",p1+1);
		Show_Str(x,y+60,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<4; 
	myfree(p); 
	return res;
} 













