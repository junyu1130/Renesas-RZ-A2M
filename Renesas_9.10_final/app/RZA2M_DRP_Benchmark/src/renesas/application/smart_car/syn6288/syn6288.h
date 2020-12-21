/*
 * syn6288.h
 *
 *  Created on: 2020年8月11日
 *      Author: 10094
 */

#include <smart_car.h>
#ifndef USER_PROG_SMART_CAR_SYN6288_SYN6288_H_
#define USER_PROG_SMART_CAR_SYN6288_SYN6288_H_

typedef struct  {
	Road_Situation r;
	u8 len;
	char sign[16];
} voice;

void SYN_FrameInfo(u8 Music, u8 *HZdata);
void YS_SYN_Set(u8 *Info_data);
void Play_voice(uint8_t num);
#endif /* USER_PROG_SMART_CAR_SYN6288_SYN6288_H_ */
