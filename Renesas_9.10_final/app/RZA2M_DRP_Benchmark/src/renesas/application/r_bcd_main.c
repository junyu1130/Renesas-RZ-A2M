/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "r_typedefs.h"
#include "r_bsp.h"

#include "r_bcd_ae.h"
#include "r_bcd_camera.h"
#include "r_bcd_lcd.h"
#include "draw.h"
#include "perform.h"
#include "main.h"
//#include "r_ostm_drv_api.h"
#include "r_os_abstraction_api.h"
//#include "r_os_abstraction.h"
//#include "r_opencv.h"

#include "r_drp.h"

#include <Typedef.h>
extern TPrecision* dnn_compute(TPrecision* input_img);


#include <image_process.h>
#include <ParallelSendData.h>
#include <syn6288.h>
#include <smart_car.h>
#include <TrackProcess.h>
#include <NRF24L01.h>
/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SCREEN_MESSAGE_LINE_BUF_SIZE       (64)
#define SCREEN_STRIDE (VIDEO_BUFFER_STRIDE)


typedef struct
{
	uint8_t* p_input_bufadr;
	uint8_t* p_output_bufadr;
	uint8_t* p_work_bufadr;
	uint8_t* p_work_bufadr2;
	uint32_t input_bufadr;
	uint32_t output_bufadr;
	uint32_t work_bufadr;
	uint32_t work_bufadr2;
	//R_DRP_BENCHMARK_ITEMS benchmark_item;
}R_PROCESS_PARAM;

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern uint32_t g_camera_vsync_us;

void r_img_inference( TPrecision* pImg );
Road_Situation road=Normal;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static uint8_t screen_str_buf[SCREEN_MESSAGE_LINE_BUF_SIZE];
static uint8_t work_buffer[16][(VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT) / 16] __attribute__ ((section("Video_RAM")));
//static uint8_t work_buffer2[(VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT) / 16] __attribute__ ((section("Video_RAM")));
static TPrecision img_t[80*80 ] ;
static uint8_t crop_image[80 *80] ;
uint8_t bin_image[COL *ROW];

/* use ae */
static uint8_t  look_up_table[256] __attribute__ ((section("Uncache_IRAM")));
static uint32_t ave_result[3] __attribute__ ((section("Uncache_IRAM")));

static r_bcd_ae_setting_t ae_setting;
static R_PROCESS_PARAM process_param;
R_PROCESS_PARAM* p_param = &process_param;
/* Switching ISP function */
static uint32_t key_status;
#define KEY_STAUS_INIT  (0xFFFFFFFFU)
#define KEY_CHECK_BIT   (0x00000003U)
#define KEY_JUST_ON     (0x00000002U)
static uint32_t isp_ae_mode;
#define ISP_AE_OFF  (0)
#define ISP_AE_ON   (1)
#define print_sign_out (0)
static void show_screen_info(R_PROCESS_PARAM* p_param)
{
	uint32_t t1, t2;
	uint16_t yOffset = 0;
	uint32_t frame_rate;

//	memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
//	snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Version: 1.0.0");
//	R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
//	yOffset += 30;
//
//	memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
//	frame_rate = 1000000 / g_camera_vsync_us;
//	snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Camera frame rate: %d fps (%d ms)",
//			frame_rate,
//			(int)(g_camera_vsync_us/1000),  (int)(g_camera_vsync_us - (int)(g_camera_vsync_us - ((g_camera_vsync_us/1000)*1000))));
//	R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
//	yOffset += 30;

	memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
	t1 = PerformGetElapsedTime_us(IPP_FPS);
	snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Totol Time: %d.%03dms",
			(int)(t1/1000), (int)(t1 - ((t1/1000) * 1000)));
	R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
	yOffset += 30;

	memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
	t1 = PerformGetElapsedTime_us(IPP_BAYER_2_GRAY_TOTAL);
	snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "bayer to gray DRP: %d.%03dms",
			(int)(t1/1000), (int)(t1 - ((t1/1000) * 1000)));
	R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
	yOffset += 30;


		memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
		t2 = PerformGetElapsedTime_us(IPP_BINARIZATION_Ostu);
		snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Ostu para get time: %d.%03dms",
					(int)(t2/1000), (int)(t2 - (t2/1000)*1000));
		R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
		yOffset += 30;

		memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
		t2 = PerformGetElapsedTime_us(IPP_BINARIZATION_FIXED_TOTAL);
		snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Binarization DRP: %d.%03dms",
					(int)(t2/1000), (int)(t2 - (t2/1000)*1000));
		R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
		yOffset += 30;

		memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				t2 = PerformGetElapsedTime_us(IPP_BINARIZATION_FIXED_TOTAL);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "DL Copy time: %d.%03dms",
							(int)(t2/1000), (int)(t2 - (t2/1000)*1000));
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
				yOffset += 30;


		memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
		t2 = PerformGetElapsedTime_us(IPP_CPU_INFERENCE);
		snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "INFERENCE : %d.%03dms",
					(int)(t2/1000), (int)(t2 - (t2/1000)*1000));
		R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
		yOffset += 30;

	switch(road){
		case (Left):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Left Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Left\r\n");
#endif
				break;

		case (Right):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Right Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Right\r\n");
#endif
				break;

		case (Stright):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Stright Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Straight\r\n");
#endif

				break;

		case (Stop):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Stop Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Stop\r\n");
#endif

				break;

		case (Red):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Red Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Red\r\n");
#endif
				break;

		case (Green):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Green Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Green\r\n");
#endif
				break;

//		case (People):
//				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
//				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "People Sign");
//				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);
//
//#if print_sign_out
//				printf("People\r\n");
//#endif
//
//				break;
		case (Normal):
				memset(screen_str_buf,0,SCREEN_MESSAGE_LINE_BUF_SIZE);
				snprintf(screen_str_buf, SCREEN_MESSAGE_LINE_BUF_SIZE, "Normal Sign");
				R_BCD_LcdWriteString(&screen_str_buf[0], 0 + 2, yOffset + 2, GREEN);

#if print_sign_out
				printf("Normal\r\n");
#endif

				break;
		default:
			break;
	}

}



static int32_t img_process(R_PROCESS_PARAM* p_param)
{
	uint8_t threshold;
	R_CACHE_L1DataInvalidLine((uint8_t*)bin_image,160*120);
	PerformSetStartTime(IPP_BINARIZATION_Ostu);

	threshold=Ostu((uint8_t*)bin_image);
	PerformSetEndTime(IPP_BINARIZATION_Ostu);

//		PerformSetStartTime(IPP_BINARIZATION_CV);
//		R_CACHE_L1DataInvalidLine((uint8_t*)p_param->p_work_bufadr, R_BCD_CAMERA_WIDTH * R_BCD_CAMERA_HEIGHT );
//		cv_threshold(p_param->p_work_bufadr, p_param->p_output_bufadr, R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT , threshold,0);
//		//imageProcess(p_param->p_work_bufadr,p_param->p_output_bufadr);
//		//R_BCD_LcdSwapVideoBuffer();
//		r_drp_binarization_fixed(p_param->work_bufadr  , p_param->output_bufadr , \
//				R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT , threshold);
//		R_CACHE_L1DataCleanLine((uint8_t*)p_param->p_output_bufadr, R_BCD_CAMERA_WIDTH * R_BCD_CAMERA_HEIGHT );
//		PerformSetEndTime(IPP_BINARIZATION_CV);

		PerformSetStartTime(IPP_BINARIZATION_FIXED_TOTAL);

#if use_bcd
		R_CACHE_L1DataInvalidLine((uint8_t*)p_param->p_work_bufadr, R_BCD_CAMERA_WIDTH * R_BCD_CAMERA_HEIGHT );
		r_drp_binarization_fixed(p_param->work_bufadr  , p_param->output_bufadr , \
						R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT , threshold);
		R_CACHE_L1DataCleanLine((uint8_t*)p_param->p_output_bufadr, R_BCD_CAMERA_WIDTH * R_BCD_CAMERA_HEIGHT );
#endif
		r_drp_binarization_fixed(p_param->work_bufadr2  , p_param->work_bufadr2 , \
								160, 120 , threshold);
		PerformSetEndTime(IPP_BINARIZATION_FIXED_TOTAL);




	return 0;
}

int find_mid_num(uint8_t* bin_imge){
	int left_flag=0,left_sign=0,right_sign=0;
	int i=0;
	for(i=0;i<160;i++){
		if (bin_imge[118*COL+i]==255)
				if (left_flag)
					right_sign=i;
				else
					left_sign=i;
					left_flag=1;
	}
	return (left_sign+right_sign)/2+10;
}

void crop_img(uint8_t* bin_image,uint8_t* crop_image){
	int x,y,z=0;
//	int mid_num=find_mid_num(bin_image);
	int mid_num=MidLine;
	for (x=30;x<110;x++)
	{
		for(y=mid_num-40;y<mid_num+40;y++){

			crop_image[z]=(uint8_t)((bin_image[x*COL+y]));
			z=z+1;
		}
	}
}
void fill_line(uint8_t* crop_image){
	int x,y;

	//左侧扫描
	for (x=0;x<80;x++){
		if (crop_image[x*80+0]==255)
			break;
		for(y=0;y<80;y++){
			if(crop_image[x*80+y]==255)
				break;
			else
				crop_image[x*80+y]=255;
		}
	}

	//右侧扫描
	for (x=0;x<80;x++){
		if (crop_image[x*80+79]==255)
			break;
		for(y=0;y<80;y++){
			if(crop_image[x*80+79-y]==255)
				break;
			else
				crop_image[x*80+79-y]=255;
		}
	}

}
Road_Situation temp_list[3]={Normal};
Road_Situation cal_road_filter(Road_Situation road_temp){
	int i=0;
	int num=2;
	for(i=0;i<num-1;i++){
		temp_list[i]=temp_list[i+1];
	}
	temp_list[num-1]=road_temp;

	int same_flag=1;
	for(i=0;i<num-1;i++){
			if (temp_list[i]!=temp_list[i+1]){
				same_flag=0;
			}
		}

	if (same_flag){
		return road_temp;
	}
	else
		return Normal;
//	Road_Situation road_fliter;
//	return road_fliter;
}


void r_img_inference( TPrecision* pImg )
{
	TPrecision *prediction;
	TPrecision max;

	int i;
	int w,h;

	PerformSetStartTime(IPP_CPU_INFERENCE);
		prediction = (TPrecision*) (intptr_t) dnn_compute(pImg);
	PerformSetEndTime(IPP_CPU_INFERENCE);

	//get  max prediction and road situation
	max = prediction[0];
	int n=0;
	for(i = 1;i < 6;i++)
	{
		if (prediction[i] > max)
		{
			max = prediction[i];
			n=i;
		}
	}
//printf("%d,%d\r\n",n,max);

#if 1



	if (max>0.93){
		road = (Road_Situation)n;
//		printf("%d\r\n",n);
	}
	else {
		road = Normal;
	}

	if(n==Stright)
	{
		if (max>0.9){
				road = (Road_Situation)n;
		//		printf("%d\r\n",n);
		}
		else
		{
			road = Normal;
		}
	}

	if(n==Stop)
	{
		if (max>0.96){
				road = (Road_Situation)n;
		//		printf("%d\r\n",n);
		}
		else
		{
			road = Normal;
		}
	}

	if(n==Traffic_light)
	{
		if (max>0.95){
				road = (Road_Situation)n;
		//		printf("%d\r\n",n);
		}
		else
		{
			road = Normal;
		}
	}

#else

	road = (Road_Situation)n;
#endif

#if print_sign_out
	switch(road){
			case (Left):
					printf("Left\r\n");
					break;
			case (Right):
					printf("Right\r\n");
					break;
			case (Stright):
					printf("Straight\r\n");
					break;
			case (Stop):
					printf("Stop\r\n");
					break;
			case (Red):
					printf("Red\r\n");
					break;
			case (Green):
					printf("Green\r\n");
					break;
			case (Normal):
					printf("Normal\r\n");
					break;
			default:
				break;
		}
		//road=cal_road_filter(road);
#endif

	/*********** Inference End *************/
}


//R_PROCESS_PARAM* p_param = &process_param;

extern uint8_t normal_flag;
extern uint8_t send_flag;
extern uint8_t cross_flag;
extern uint8_t traffic_en;
uint8_t object_buf = 0;
uint8_t send_sign = 0;
uint8_t send_en = 1;

/*******************************************************************************
* Function Name: sample_main
* Description  : First function called after initialization is completed
* Arguments    : -
* Return Value : -
*******************************************************************************/
void sample_main(void)
{
    int32_t frame_buf_id;
    int32_t ret_val;
    uint32_t us;

    int32_t result;


    uint32_t total_time;
    uint8_t frame_rate;

    /* DRP L2 Cache ON */
    PRR.AXIBUSCTL4.BIT.DRPARCACHE = 0xF;
    PRR.AXIBUSCTL4.BIT.DRPAWCACHE = 0xF;

    /* Initialization of VIN and MIPI driver */
    R_BCD_CameraInit();

    /* Initialization of LCD driver */
#if use_bcd
    /* VDC6 L2 Cache ON */
    PRR.AXIBUSCTL6.BIT.VDC601ARCACHE = 0xF;
    PRR.AXIBUSCTL6.BIT.VDC602ARCACHE = 0xF;
    PRR.AXIBUSCTL7.BIT.VDC604ARCACHE = 0xF;

    R_BCD_LcdInit();
#endif


#if use_auto_exposure
    unsigned short ae_flag=0;
    R_BCD_AeInit(&ae_setting);
    R_BCD_AeMakeGammaTable(&ae_setting, (double)1.2, look_up_table);
//    R_BCD_AeStop(&ae_setting);
#endif

    /* Initialize AE mode */
    isp_ae_mode = ISP_AE_OFF;

    /* Capture Start */
    R_BCD_CameraClearCaptureStatus();
    R_BCD_CameraCaptureStart();

    /* Initialize of Performance counter */
    PerformInit();
    /* Initialization of DRP driver */
    R_DK2_Initialize();
    parallelSendData_Init();
    NRF_Init();
    while(NRF24L01_Check());
//    uint8_t NRF2401TXBuffer[TX_PLOAD_WIDTH];
    uint8_t NRF2401RXBuffer[RX_PLOAD_WIDTH];
//    NRF2401TXBuffer[0]=36;
    /* main loop */
    int frame=0;
    uint8_t sign_buf = 0;
    uint8_t traffic_buf = 0;
    while (1)
    {
    	PerformSetStartTime(IPP_FPS);
    	int_t err=0;

        /* Wait until camera capture is complete */
        while ((frame_buf_id = R_BCD_CameraGetCaptureStatus()) == R_BCD_CAMERA_NOT_CAPTURED)
        {
            /* DO NOTHING */
        }

#if use_bcd
        p_param->p_output_bufadr = R_BCD_LcdGetVramAddress();
        R_MMU_VAtoPA((uint32_t)p_param->p_output_bufadr, &p_param->output_bufadr);

#endif
        p_param->p_input_bufadr  = R_BCD_CameraGetFrameAddress(frame_buf_id);
        p_param->p_work_bufadr = (uint8_t *)&work_buffer[0];
        p_param->p_work_bufadr2 = (uint8_t *)&bin_image[0];

        /* convert to phisical address */
		R_MMU_VAtoPA((uint32_t)p_param->p_input_bufadr, &p_param->input_bufadr);
		R_MMU_VAtoPA((uint32_t)p_param->p_work_bufadr, &p_param->work_bufadr);
		R_MMU_VAtoPA((uint32_t)p_param->p_work_bufadr2, &p_param->work_bufadr2);

		PerformSetStartTime(IPP_BAYER_2_GRAY_TOTAL);
		r_drp_bayer2grayscale(p_param->input_bufadr, p_param->work_bufadr, R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT);
		PerformSetEndTime(IPP_BAYER_2_GRAY_TOTAL);

		//PerformSetEndTime(IPP_FPS);//4ms
//		r_drp_resize_bilinear(p_param->work_bufadr, p_param->work_bufadr2, R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT, 160, 120);
		r_drp_resize_nearest(p_param->work_bufadr, p_param->work_bufadr2, R_BCD_CAMERA_WIDTH, R_BCD_CAMERA_HEIGHT, 160, 120);
//		PerformSetEndTime(IPP_FPS);//15ms

		img_process(p_param);
		//PerformSetEndTime(IPP_FPS);//20ms

		int i;

#if use_deeplearning
if(frame%3==0&&turn_flag==0){

		PerformSetStartTime(IPP_CPU_COPY2RESULT_MEM);

		R_CACHE_L1DataInvalidLine((uint8_t*)p_param->work_bufadr2,160*120);
		crop_img((uint8_t*)p_param->work_bufadr2,(uint8_t*)crop_image);
		R_CACHE_L1DataCleanLine((uint8_t*)crop_image, 80 * 80 );
		fill_line((uint8_t*)crop_image);

		int x,y,z=0;
		for (x=0;x<80;x++)
		{
			for(y=0;y<80;y++){

				if(crop_image[x*80+y]==255){
					img_t[z] = (TPrecision)1;
				}
				else {
					img_t[z] = (TPrecision)0;
						}
				z++;
			}
		}

		PerformSetEndTime(IPP_CPU_COPY2RESULT_MEM);
		r_img_inference( img_t);

}
else if(turn_flag)
{
	road = Normal;
}

frame++;
#endif

#if use_auto_exposure
//		int i=0;
		ave_result[1]=0;
		for (i=0;i<R_BCD_CAMERA_WIDTH*R_BCD_CAMERA_HEIGHT;i++){
			ave_result[1]=p_param->p_work_bufadr[i]+ave_result[1];
		}

#endif


#if use_send_data
		R_CACHE_L1DataInvalidLine((uint8_t*)p_param->work_bufadr2,160*120);
		crop_img((uint8_t*)p_param->work_bufadr2,(uint8_t*)crop_image);
		R_CACHE_L1DataCleanLine((uint8_t*)crop_image, 80 * 80 );

//		R_CACHE_L1DataInvalidLine((uint8_t*)crop_image, 80 * 80 );
		fill_line((uint8_t*)crop_image);

		TRACE_INFO("start\r\n" );
//		R_CACHE_L1DataInvalidLine((uint8_t*)crop_image, 80 * 80 );
		int x,y,z=0;
		for (x=0;x<120;x++)
		{
			for(y=0;y<160;y++){

					TRACE_INFO("%02X",bin_image[x*160+y]);
//					y=y+2;
						for (int y =0;y<50000;y++){
										;}
			}
//			x=x+2;
		}

//		R_CACHE_L1DataCleanLine((uint8_t*)crop_image, 80 * 80 );
		TRACE_INFO("\r\n");


//		r_show_rect(120*6,(mid_num-40)*6,80*6,80*6,(uint8_t *)p_param->output_bufadr);

#endif

#if use_auto_exposure
		unsigned int ave_lum=ave_result[1] / (R_BCD_CAMERA_WIDTH * R_BCD_CAMERA_HEIGHT);
		if(   (ave_lum<100) & (ae_flag==0)){
		R_BCD_AeRunAutoExpousure(&ae_setting, (uint16_t)(ave_lum/2));
		}
		else {
			ae_flag=1;
		}

#endif

		uint8_t road_temp = 0;
		uint8_t traffic_light = 0;//检测到十字一直接收，那必然是1/2，没检测到十字是0
		switch(road){
			case (Left):
					road_temp = sign_left;
					break;
			case (Right):
					road_temp = sign_right;
					break;
			case (Stright):
					road_temp = sign_straight;
					break;
			case (Stop):
					road_temp = sign_stop;
					break;
			case (Traffic_light):
					road_temp = sign_traffic;
#if use_NRF
//			PerformSetStartTime(IPP_Test_1);
			NRF24L01_RX_Mode(90);
			while(nRF24L01_RxPacket(NRF2401RXBuffer)){};
			traffic_light=NRF2401RXBuffer[1];
//			PerformSetEndTime(IPP_Test_1);
//			int t1 = PerformGetElapsedTime_us(IPP_Test_1);
//			printf("%02X",t1);
#endif

					break;
			case (Normal):
					road_temp = sign_normal;
					break;
			default:
					road_temp = sign_normal;
					break;
		}

		if(road_temp == sign_traffic)//识别到十字不发送数据，重置后允许发送
		{
			send_en = 0;
		}
		if(LeftLose<1 && RightLose<1 && AllLose<1)
		{
			send_en = 1;
		}

		R_CACHE_L1DataInvalidLine((uint8_t*)p_param->work_bufadr2,160*120);

		trackProcess();//赛道轨迹处理
		send_sign = 0;
		if(traffic_light==0)
		{
			if(traffic_buf==traffic_red)//这种情况说明没有十字标志识别到了，红灯后无绿灯，需人为修正
			{
#if use_NRF
				NRF24L01_RX_Mode(90);
				while(nRF24L01_RxPacket(NRF2401RXBuffer)){};
				traffic_light=NRF2401RXBuffer[1];
#endif
			}
		}

		//障碍物信号
		uint8_t objectFlag = 0;
		if(RCross_flag==sign_right&&turn_flag==0)
		{
#if use_NRF
				NRF24L01_RX_Mode(100);
				while(nRF24L01_RxPacket(NRF2401RXBuffer)){};
				objectFlag=NRF2401RXBuffer[1];
				if(objectFlag==1)
				{
					object_buf = 1;
				}
#endif
		}
		else if(object_buf==1)//无法满足接收条件但此刻仍然停止，继续接收
		{
#if use_NRF
				NRF24L01_RX_Mode(100);
				while(nRF24L01_RxPacket(NRF2401RXBuffer)){};
				objectFlag=NRF2401RXBuffer[1];
				if(objectFlag==2)
				{
					object_buf = 0;
				}
#endif
		}

		//识别到信号后，送给自身做赛道信息的处理,内部触发后会清零
		if(sign_buf!=road_temp
		&& (road_temp==sign_straight||road_temp==sign_right||road_temp==sign_left)
		&& send_en)//十字岔路识别到的信号不替换buf
		{
			sign_buf = road_temp;
			sign = road_temp;
		}
		else if(sign_buf!=road_temp && road_temp==sign_stop)
		{
			sign_buf = road_temp;
			send_sign = sign_stop;
		}
		else if(road_temp==sign_traffic && traffic_light == traffic_red && traffic_en == 1)
		{
			send_sign = sign_traffic;
			traffic_buf = traffic_red;
		}
		else if(traffic_light == traffic_green)
		{
			send_sign = sign_go;
			traffic_buf = 0;
		}
		else if(objectFlag==1)
		{
			send_sign = sign_traffic;
		}
		else if(objectFlag==2)
		{
			send_sign = sign_go;
		}


//		else if(traffic_light == traffic_red)//&&traffic_buf!=traffic_red)
//				//&&traffic_en == 1)
//		{
//			traffic_buf = traffic_red;
//			send_sign = sign_traffic;
//		}
//		else if(traffic_light == traffic_green)//&&traffic_buf!=traffic_green)
//				//&&traffic_en == 1)
//		{
//			traffic_buf = traffic_green;
//			send_sign = sign_go;
//		}

		if(send_flag&&cross_flag&&(send_sign==0))//岔路口发送识别信号标志 一次重置只允许触发一次
		{
			send_sign = sign_buf;
			sign_buf = 0;
			send_flag = 0;
			cross_flag = 0;
		}
		//处理得到13位数据并口传输,前面需要初始化IO
		uint16_t send_data =  ((send_sign&0x07) <<10) 	//识别到的信号 3b
							| ((turn_flag&0x03) <<8 )	//转向控制信号 2b
							| (CenterMeanValue&0xff );	//中线值            8b
		parallelSendData(send_data);//并口发送数据

//		if(send_sign==sign_go)//绿灯行
//		{
//			R_OS_TaskSleep(15);
//			if(traffic_light==traffic_green)
//			{
//				Play_voice(7);
//			}
//		}
//		else if(send_sign==sign_traffic)//红灯停
//		{
//			R_OS_TaskSleep(15);
//			if(traffic_light==traffic_red)
//			{
//				Play_voice(6);
//			}
//		}
//		else
//		{
//			R_OS_TaskSleep(15);
//			Play_voice(send_sign);
//		}

		PerformSetEndTime(IPP_FPS);//21ms

        /* Clear the current capture state and enable the detection of the next capture completion */
        R_BCD_CameraClearCaptureStatus();

#if use_bcd
		/* update display data on the LCD screen */
        R_BCD_LcdClearGraphicsBuffer();

        show_screen_info(p_param);

        /* Write the data(buf) on the cache to physical memory */
        R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), (GRAPHICS2_BUFFER_STRIDE * GRAPHICS2_BUFFER_HEIGHT));

        /* switch back buffer to display buffer */
        R_BCD_LcdSwapGraphicsBuffer();
#endif

    }

    return;
}
/*******************************************************************************
* End of function sample_main
*******************************************************************************/

/* End of File */
