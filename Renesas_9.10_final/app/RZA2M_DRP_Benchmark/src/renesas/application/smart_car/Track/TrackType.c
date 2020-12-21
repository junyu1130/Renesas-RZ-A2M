#include "TrackType.h"
#include "stdio.h"

InflectionPoint  InflectionPointL;//建立左边拐点结构体变量
InflectionPoint  InflectionPointR;//建立右边拐点结构体变量

CrossType Cross;//建立十字结构体变量

//十字用到的变量
uint8_t RCross_flag = 0;
uint8_t LCross_flag = 0;
uint8_t LRCross_flag = 0;
uint8_t turn_flag = 0;
int count = 0;
uint8_t sign = 0;
unsigned char LeftScanCol[COL];
unsigned char RightScanCol[COL];
uint8_t traffic_en = 0;

//对右边线进行操作，如果存在多个拐点寻找最近的拐点
//prt:传入一个结构体变量的地址
//Start：扫描的起始行
//End ：扫描的结束行
static void FindInflectionPointR(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //初始化结构体变量
    prt->ScanInflectionPointStart=Start;//设置扫描的起始行
    prt->ScanInflectionPointEnd=End;//设置扫描的结束行
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    for(i=prt->ScanInflectionPointStart; i>=prt->ScanInflectionPointEnd; i--) //不能扫描太远，否则会误判
    {
        if(RightEdge[i]!=COL-1&&RightEdge[i+1]!=COL-1)
        {
            if((RightEdge[i]-RightEdge[i+1]<=0)
              &&(RightEdge[i-1]-RightEdge[i]>=1)
              &&(RightEdge[i-2]-RightEdge[i]>=2)
              &&(RightEdge[i-3]-RightEdge[i]>=4)
              &&(RightEdge[i]<=143)
              &&(RightEdge[i]>=121))//找到右边线有拐点
            {
                prt->InflectionPointRow=i;//记录拐点的行
                prt->InflectionPointCol=RightEdge[i];//记录拐点的列
                prt->InflectionPointFlag=1;//标记找到右拐点
                break;//退出for
            }
        }
    }
}

//对左边线进行操作，如果存在多个拐点寻找最近的拐点
//prt:传入一个结构体变量的地址
//Start：扫描的起始行
//End ：扫描的结束行
static void FindInflectionPointL(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //初始化结构体变量
    prt->ScanInflectionPointStart=Start;//设置扫描的起始行
    prt->ScanInflectionPointEnd=End;//设置扫描的结束行
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    //左拐点
    for(i= prt->ScanInflectionPointStart; i>prt->ScanInflectionPointEnd; i--)
    {
        if(LeftEdge[i]!=0&&LeftEdge[i+1]!=0)
        {
            if((LeftEdge[i]-LeftEdge[i+1]>=0)
              &&(LeftEdge[i]-LeftEdge[i-1]>=1)
              &&(LeftEdge[i]-LeftEdge[i-2]>=2)
              &&(LeftEdge[i]-LeftEdge[i-3]>=4)
              &&(LeftEdge[i]>=22)
              &&(LeftEdge[i]<=44))//找到拐点
            {
                prt->InflectionPointRow=i;//记录该拐点的行
                prt->InflectionPointCol=LeftEdge[i];//记录该拐点的列
                prt->InflectionPointFlag=1;//标记找到左拐点
                break;//找到退出
            }
        }
    }
}


static void CrossRecognition(CrossType *prt)
{
    //初始化变量
    prt->CrossLR=0;
    prt->CrossL=0;
    prt->CrossR=0;

    //寻找左右拐点
    FindInflectionPointR(&InflectionPointR,StartRow-1,80);
    FindInflectionPointL(&InflectionPointL,StartRow-1,80);
    //如果左边界和右边界同时丢线大于5行，就认为遇到了十字
    if( InflectionPointR.InflectionPointFlag
        &&InflectionPointL.InflectionPointFlag
        &&(AllLose>=5 || (LeftLose>=5&&RightLose>=5)))
    {
        prt->CrossLR=1;//十字
        traffic_en = 1;
    }
    //判断右分岔路
    else if(InflectionPointR.InflectionPointFlag //存在右边拐点
            &&RightLose>=5&&LeftLose<3&&AllLose<3)
    {
        prt->CrossR=1;//标志右岔路
        traffic_en = 1;
    }
    //判断左分岔路
    else if(InflectionPointL.InflectionPointFlag//存在左边拐点
            &&LeftLose>=5&&RightLose<3&&AllLose<3)
    {
        prt->CrossL=1;//标志左岔路
        traffic_en = 1;
    }
}

uint8_t send_flag = 0;
uint8_t normal_flag = 0;
uint8_t cross_flag = 1;
//uint8_t signCnt = 0;
//static uint8_t signal_flag[11] = {
//	5,5,4,4,3,5,5,4,4
//};

//得到十字类型的相关标志作为控制标志位
static void Crossflag(void)
{
//	if(sign==0&&cross_flag)
//	{
//		sign = signal_flag[signCnt];
//		signCnt++;
//		cross_flag = 0;
//	}
    if(Cross.CrossLR)//十字
    {
        //printf("CrossLR\r\n");
        if(sign==sign_straight)
        {
            //printf("CrossLR straight\r\n");
            LRCross_flag = sign_straight;
            sign = 0;
            send_flag = 1;
        }
        else if(sign==sign_right)//右转信号
        {
            //printf("CrossLR right\r\n");
            LRCross_flag = sign_right;
            sign = 0;
            send_flag = 1;
        }
        else if(sign==sign_left)//左转信号
        {
            //printf("CrossLR left\r\n");
            LRCross_flag = sign_left;
            sign = 0;
            send_flag = 1;
        }
    }
    else if(Cross.CrossR)//右分叉
    {
        //printf("CrossR\r\n");
        if(sign==sign_straight)//直行信号
        {
            //printf("CrossR straight\r\n");
            RCross_flag = sign_straight;
            sign = 0;
            send_flag = 1;
        }
        else if(sign==sign_right)//右转信号
        {
            //printf("CrossR right\r\n");
            RCross_flag = sign_right;
            sign = 0;
            send_flag = 1;
        }
    }
    else if(Cross.CrossL)//左分叉
    {
        //printf("CrossL\r\n");
        if(sign==sign_straight)//直行信号
        {
            //printf("CrossL straight\r\n");
            LCross_flag = sign_straight;
            sign = 0;
            send_flag = 1;
        }
        else if(sign==sign_left)//左转信号
        {
            //printf("CrossL left\r\n");
            LCross_flag = sign_left;
            sign = 0;
            send_flag = 1;
        }
    }
}
extern uint8_t send_en;
//对分叉进行处理，得出舵机的控制中线
void CrossProcess(void)
{
    turn_flag = 0;
    CrossRecognition(&Cross);//识别岔路
    if(LRCross_flag==0&&RCross_flag==0&&LCross_flag==0)//未识别到岔路
    {
    	Crossflag();
    }
    if(LRCross_flag||RCross_flag||LCross_flag)
    {
		//十字
		if(LRCross_flag==sign_straight) {//直行

		}
		else if(LRCross_flag==sign_right) {//右转
			if(count>=2)
			{
				turn_flag = turn_right;
			}
			else
			{
				if((InflectionPointR.InflectionPointFlag==0&&InflectionPointL.InflectionPointFlag==0)
				  &&(RightLose>=5||AllLose>=5)) {
					count++;
				} else {
					count=0;
				}
			}
		}
		else if(LRCross_flag==sign_left) {//左转
			if(count>=2)
			{
				turn_flag = turn_left;
			}
			else
			{
				if((InflectionPointL.InflectionPointFlag==0&&InflectionPointR.InflectionPointFlag==0)
				  &&(LeftLose>=5||AllLose>=5))
				{
					count++;
				}
				else
				{
					count=0;
				}
			}
		}
		//右岔路
		if(RCross_flag==sign_straight) {//直行

		}
		else if(RCross_flag==sign_right) {//右转
			if(count>=2)
			{
				turn_flag = turn_right;
			}
			else
			{
				if((InflectionPointR.InflectionPointFlag==0&&InflectionPointL.InflectionPointFlag==0)
				  &&RightLose>=5&&LeftLose<3&&AllLose<3)
				{
					count++;
				}
				else
				{
					count=0;
				}
			}
		}
		//左岔路
		if(LCross_flag==sign_straight) {//直行

		}
		else if(LCross_flag==sign_left) {//左转
			if(count>=2)//3
			{
				turn_flag = turn_left;
			}
			else
			{
				if((InflectionPointL.InflectionPointFlag==0&&InflectionPointR.InflectionPointFlag==0)
				  &&LeftLose>=5&&RightLose<3&&AllLose<3)
				{
					count++;
				}
				else
				{
					count=0;
				}
			}
		}

		//在图像恢复正常后重置标志位
		if(LeftLose<1 && RightLose<1 && AllLose<1
	//	  && ((ABS(LeftEdge[60]  + Width[60] / 2 - MidLine) < 20)   || LeftEdge[60]  ==MidLine)
	//	  && ((ABS(RightEdge[60] - Width[60] / 2 - MidLine) < 20)   || RightEdge[60] ==MidLine)
		  && ((ABS(LeftEdge[70]  + Width[70] / 2 - MidLine) < 18)   || LeftEdge[70]  ==MidLine)
		  && ((ABS(RightEdge[70] - Width[70] / 2 - MidLine) < 18)   || RightEdge[70] ==MidLine)
		  && ((ABS(LeftEdge[80]  + Width[80] / 2 - MidLine) < 18)   || LeftEdge[80]  ==MidLine)
		  && ((ABS(RightEdge[80] - Width[80] / 2 - MidLine) < 18)   || RightEdge[80] ==MidLine)
		  && ((ABS(LeftEdge[90]  + Width[90] / 2 - MidLine) < 18)   || LeftEdge[90]  ==MidLine)
		  && ((ABS(RightEdge[90] - Width[90] / 2 - MidLine) < 18)   || RightEdge[90] ==MidLine)
		  && ((ABS(LeftEdge[100]  + Width[100] / 2 - MidLine) < 18)   || LeftEdge[100]  ==MidLine)
		  && ((ABS(RightEdge[100] - Width[100] / 2 - MidLine) < 18)   || RightEdge[100] ==MidLine)
		  && ((ABS(LeftEdge[110]  + Width[110] / 2 - MidLine) < 18)   || LeftEdge[110]  ==MidLine)
		  && ((ABS(RightEdge[110] - Width[110] / 2 - MidLine) < 18)   || RightEdge[110] ==MidLine)
		  && (RCross_flag||LCross_flag||LRCross_flag)
		  && ((RCross_flag==sign_straight||LCross_flag==sign_straight||LRCross_flag==sign_straight)||turn_flag))
		{
			RCross_flag = 0; LCross_flag = 0; LRCross_flag = 0;
			count = 0;
			normal_flag = 1;
			cross_flag = 1;
			traffic_en = 0;
//			send_en = 1;
			//printf("reset\r\n");
		}
    }
}

