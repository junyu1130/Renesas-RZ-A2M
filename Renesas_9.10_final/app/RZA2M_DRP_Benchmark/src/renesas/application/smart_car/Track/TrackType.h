#ifndef __TRACKTYPE_H__
#define __TRACKTYPE_H__

#include <smart_car.h>

extern uint8_t turn_flag;//控制舵机
extern uint8_t sign;//识别到的信号
extern uint8_t RCross_flag;
extern uint8_t LCross_flag;
extern uint8_t LRCross_flag;

typedef struct 
{
 unsigned char InflectionPointRow;//拐点的行坐标
 unsigned char InflectionPointCol;//拐点的列坐标
 unsigned char InflectionPointFlag;//拐点的标志位
 unsigned char ScanInflectionPointStart;//扫描拐点的起始行
 unsigned char ScanInflectionPointEnd;//扫描拐点结束行
 unsigned char InflectionPointNumber;//拐点的个数
 
}InflectionPoint;//建立拐点结构体

typedef struct 
{
  unsigned char CrossLR;//十字
  unsigned char CrossL;//左岔路
  unsigned char CrossR;//右岔路
}CrossType;//建立十字类型的结构体

extern int   RightEdge[ROW];
extern int   LeftEdge[ROW];
extern int   MiddleLine[ROW];
extern int   Width[ROW];
extern int   LeftLose;
extern int   RightLose;
extern int   AllLose ;

void CrossProcess(void);

#endif
