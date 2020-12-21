/*
 * TrackProcess.h
 *
 *  Created on: 2020年8月22日
 *      Author: Y
 */

#ifndef RENESAS_APPLICATION_SMART_CAR_TRACK_TRACKPROCESS_H_
#define RENESAS_APPLICATION_SMART_CAR_TRACK_TRACKPROCESS_H_

#include <smart_car.h>
#include "TrackType.h"
#include "findTrack.h"

extern uint8_t CenterMeanValue;//计算得到的中线值

void trackProcess(void);//赛道轨迹处理

#endif /* RENESAS_APPLICATION_SMART_CAR_TRACK_TRACKPROCESS_H_ */
