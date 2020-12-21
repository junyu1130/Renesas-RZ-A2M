/*
 * TrackProcess.c
 *
 *  Created on: 2020年8月22日
 *      Author: Y
 */
#include "TrackProcess.h"

uint8_t  CenterMeanValue=0;

static uint32_t  WeightSum=0;
static uint32_t  CenterSum=0;

static uint8_t Weight[120] = {
    0,0,0,0,0,0,0,0,0,0,              //0-9行
    0,0,0,0,0,0,0,0,0,0,              //0-19行
    0,0,0,0,0,0,0,0,0,0,              //20-29行
    0,0,0,0,0,0,0,0,0,0,              //30-39行
	0,0,0,0,0,0,0,0,0,0,              //40-49行
	0,0,0,0,0,0,0,0,0,0,              //50-59行
	0,0,0,0,0,1,1,1,1,1,              //60-69行
	1,1,1,1,1,1,1,1,1,1,              //70-79行
	2,2,2,2,2,2,2,2,2,2,              //80-89行
	2,2,2,2,2,2,2,2,2,2,              //90-99行
	2,2,2,2,2,1,1,1,1,1,              //100-109行
	1,1,1,1,1,0,0,0,0,0,              //110-119行
};

//赛道轨迹处理
void trackProcess(void)
{
	findTrack();//寻找边线，提取中线
	CrossProcess();//对分叉进行处理，得出舵机的控制中线

	CenterSum=0;
	CenterMeanValue=0;
	WeightSum=0;
	//平滑中线
	for(int i=StartRow; i>=LastRow; i--)
	{
		if(ABS(MiddleLine[i]-MiddleLine[i+1])>5 && i!=StartRow)
		{
			MiddleLine[i]=MiddleLine[i+1];
		}
		CenterSum+=MiddleLine[i]*Weight[i];
		WeightSum+=Weight[i];
	}
	if(WeightSum!=0)
	{
		CenterMeanValue=CenterSum/WeightSum;//算出加权平均后中线的值
	}
}



