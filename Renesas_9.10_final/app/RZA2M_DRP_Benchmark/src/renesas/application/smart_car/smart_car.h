/*
 * smart_car.h
 *
 *  Created on: 2020年8月17日
 *      Author: 10094
 */

#ifndef RENESAS_APPLICATION_SMART_CAR_SMART_CAR_H_
#define RENESAS_APPLICATION_SMART_CAR_SMART_CAR_H_

#include "r_gpio_drv_api.h"
#include "iodefine.h"
#include "r_typedefs.h"
#include "r_os_abstraction_api.h"


#define COL     160
#define ROW     120

#define WHITE_CAR 255
#define BLACK_CAR 0

//寻找边线的相关参数
#define MidLine 82
#define effective_left 5
#define effective_right (COL-6)
#define StartRow (ROW-3)
#define LastRow 50

//队列有关指令
#define sign_normal 0
//开车停车指令，立即执行
#define sign_go    1
#define sign_stop  2
//识别标志信号,提前识别
#define sign_straight  	3
#define sign_right     	4
#define sign_left      	5
#define sign_traffic	6  //traffic_stop

//交通灯信号
#define traffic_green 	1
#define traffic_red 	2

//自身控制指令
#define turn_right  1
#define turn_left   2



#define ABS(x) ((x) > 0 ? (x) : -(x))

//some typedef
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned char uchar;
typedef unsigned char BYTE;
typedef int16_t int16;
typedef uint8_t uint8;

typedef enum
{
	Normal,
	Left,
	Right,
	Stright,
	Stop,
	Traffic_light,
	Green,
	Red,
//	People,
//
//	f_left,
//	f_right,
//	f_straight,

}Road_Situation;

extern uint8_t bin_image[COL *ROW];
extern Road_Situation road;


//extern  int Read_Bit(int_t handle ,st_r_drv_gpio_pin_rw_t pin);
#endif /* RENESAS_APPLICATION_SMART_CAR_SMART_CAR_H_ */
