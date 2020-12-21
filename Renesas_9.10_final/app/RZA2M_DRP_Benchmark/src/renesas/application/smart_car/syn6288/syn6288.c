/*
 * syn6288.c
 *
 *  Created on: 2020年8月11日
 *      Author: 10094
 */

#include <syn6288.h>
#include<stdio.h>
#include "string.h"
#include "r_os_abstraction_api.h"
const voice voice_list[8]={
		{},
		{},
		{Stop,16,{'\xFD','\x00','\x0D','\x01','\x01','\xB5','\xBD','\xB4','\xEF','\xC4','\xBF','\xB5','\xC4','\xB5','\xD8','\xC4'}},
		{Stright,16,{'\xFD','\x00','\x0D','\x01','\x01','\xC7','\xB0','\xB7','\xBD','\xD6','\xB1','\xD0','\xD0','\x0D','\x0A','\xED'}},
		{Right,14,{'\xFD','\x00','\x0B','\x01','\x39','\xC7','\xB0','\xB7','\xBD','\xD3','\xD2','\xD7','\xAA','\xCF'}},
		{Left,14,{'\xFD','\x00','\x0B','\x01','\x39','\xC7','\xB0','\xB7','\xBD','\xD7','\xF3','\xD7','\xAA','\xEA'}},
		{Red,12, {'\xFD','\x00','\x09','\x01','\x01','\xBA','\xEC','\xB5','\xC6','\xCD','\xA3','\xBF'}},
		{Green,14, {'\xFD','\x00','\x0B','\x01','\x01','\xC2','\xCC','\xB5','\xC6','\xD0','\xD0','\x0D','\x0A','\x8C'}},
//		{People,16,{'\xFD','\x00','\x0B','\x01','\x01','\xD7','\xA2','\xD2','\xE2','\xD0','\xD0','\xC8','\xCB','\xB0'}},
};

//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[100];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  for(i=0;i< 5 + HZ_Length+1 ;i++){
	  fprintf(stdout,"%02X",Frame_Info[i]);
  }
  return;
//  printf("%s",Frame_Info);
//  USART3_SendString(Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  printf("%s",Info_data);
//  USART3_SendString(Info_data, Com_Len);
}

void Play_voice(uint8_t num){
	int i;
	voice temp=voice_list[num];
	for (i=0;i<temp.len;i++){
		printf("%c",temp.sign[i]);
//		R_OS_TaskSleep(1);
//    		printf("\xff");
	}
//	printf("\r\n");
}


