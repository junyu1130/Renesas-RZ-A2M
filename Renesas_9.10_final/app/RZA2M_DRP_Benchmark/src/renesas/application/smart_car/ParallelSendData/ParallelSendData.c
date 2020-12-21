/*
 * ParallelSendData.c
 *
 *  Created on: 2020年8月23日
 *      Author: Y
 */

#include "ParallelSendData.h"

static int_t s_my_gpio_handle;

//static uint32_t s_main_led_flg;      /* LED lighting/turning off */

//define some para
//---------High-signal---------//

//Pin-6
static st_r_drv_gpio_pin_rw_t s_p64_hi ={GPIO_PORT_6_PIN_4,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p65_hi ={GPIO_PORT_6_PIN_5,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p66_hi ={GPIO_PORT_6_PIN_6,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p67_hi ={GPIO_PORT_6_PIN_7,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
//Pin-7
static st_r_drv_gpio_pin_rw_t s_p70_hi ={GPIO_PORT_7_PIN_0,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p71_hi ={GPIO_PORT_7_PIN_1,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p73_hi ={GPIO_PORT_7_PIN_3,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p74_hi ={GPIO_PORT_7_PIN_4,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p75_hi ={GPIO_PORT_7_PIN_5,GPIO_LEVEL_HIGH,GPIO_SUCCESS};

//Pin-9
static st_r_drv_gpio_pin_rw_t s_p94_hi ={GPIO_PORT_9_PIN_4,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p95_hi ={GPIO_PORT_9_PIN_5,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p96_hi ={GPIO_PORT_9_PIN_6,GPIO_LEVEL_HIGH,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p97_hi ={GPIO_PORT_9_PIN_7,GPIO_LEVEL_HIGH,GPIO_SUCCESS};

//---------Low-signal---------//

//Pin-6
static st_r_drv_gpio_pin_rw_t s_p64_lo ={GPIO_PORT_6_PIN_4,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p65_lo ={GPIO_PORT_6_PIN_5,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p66_lo ={GPIO_PORT_6_PIN_6,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p67_lo ={GPIO_PORT_6_PIN_7,GPIO_LEVEL_LOW,GPIO_SUCCESS};

//Pin-7
static st_r_drv_gpio_pin_rw_t s_p70_lo ={GPIO_PORT_7_PIN_0,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p71_lo ={GPIO_PORT_7_PIN_1,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p73_lo ={GPIO_PORT_7_PIN_3,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p74_lo ={GPIO_PORT_7_PIN_4,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p75_lo ={GPIO_PORT_7_PIN_5,GPIO_LEVEL_LOW,GPIO_SUCCESS};

//Pin-9
static st_r_drv_gpio_pin_rw_t s_p94_lo ={GPIO_PORT_9_PIN_4,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p95_lo ={GPIO_PORT_9_PIN_5,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p96_lo ={GPIO_PORT_9_PIN_6,GPIO_LEVEL_LOW,GPIO_SUCCESS};
static st_r_drv_gpio_pin_rw_t s_p97_lo ={GPIO_PORT_9_PIN_7,GPIO_LEVEL_LOW,GPIO_SUCCESS};

static st_r_drv_gpio_pin_rw_t s_p93    ={GPIO_PORT_9_PIN_3, GPIO_LEVEL_SC_DEFAULT, GPIO_SUCCESS };

static const r_gpio_port_pin_t s_led_pin_list[] =
{
//    GPIO_PORT_6_PIN_0,
	GPIO_PORT_6_PIN_4,
	GPIO_PORT_6_PIN_5,
	GPIO_PORT_6_PIN_6,
	GPIO_PORT_6_PIN_7,
	GPIO_PORT_7_PIN_0,
	GPIO_PORT_7_PIN_1,
	GPIO_PORT_7_PIN_3,
	GPIO_PORT_7_PIN_4,
	GPIO_PORT_7_PIN_5,
	GPIO_PORT_9_PIN_4,
	GPIO_PORT_9_PIN_5,
	GPIO_PORT_9_PIN_6,
	GPIO_PORT_9_PIN_7
//	GPIO_PORT_B_PIN_5
};

//并口传输数据IO初始化
int_t parallelSendData_Init(void)
{
    int_t err;
    st_r_drv_gpio_pin_list_t pin_led;
    char_t data;
    s_my_gpio_handle = open(DEVICE_INDENTIFIER "gpio", O_RDWR);
    pin_led.p_pin_list = s_led_pin_list;
    pin_led.count = (sizeof(s_led_pin_list)) / (sizeof(s_led_pin_list[0]));
    err = direct_control(s_my_gpio_handle, CTL_GPIO_INIT_BY_PIN_LIST, &pin_led);
    ParallelData0_0;
	ParallelData1_0;
	ParallelData2_0;
	ParallelData3_0;
	ParallelData4_0;
	ParallelData5_0;
	ParallelData6_0;
	ParallelData7_0;
	ParallelData8_0;
	ParallelData9_0;
	ParallelData10_0;
	ParallelData11_0;
	ParallelData12_0;
    return err;
}

//int Read_Bit(int_t handle,st_r_drv_gpio_pin_rw_t pin){
//
//    int_t gpio_err;
//    e_r_drv_gpio_level_t ret;
//
//    gpio_err = direct_control(handle, CTL_GPIO_PIN_READ, &pin);
//    if (gpio_err < 0)
//    {
//        ret = GPIO_LEVEL_SC_DEFAULT;
//    }
//    else
//    {
//        ret = pin.level;
//    }
//    return (int)(ret-1);
//}

//并口发送13位数据
void parallelSendData(u16 data)
{
	if(data&0x0001) ParallelData0_1;else ParallelData0_0;
	if(data&0x0002) ParallelData1_1;else ParallelData1_0;
	if(data&0x0004) ParallelData2_1;else ParallelData2_0;
	if(data&0x0008) ParallelData3_1;else ParallelData3_0;
	if(data&0x0010) ParallelData4_1;else ParallelData4_0;
	if(data&0x0020) ParallelData5_1;else ParallelData5_0;
	if(data&0x0040) ParallelData6_1;else ParallelData6_0;
	if(data&0x0080) ParallelData7_1;else ParallelData7_0;
	if(data&0x0100) ParallelData8_1;else ParallelData8_0;
	if(data&0x0200) ParallelData9_1;else ParallelData9_0;
	if(data&0x0400) ParallelData10_1;else ParallelData10_0;
	if(data&0x0800) ParallelData11_1;else ParallelData11_0;
	if(data&0x1000) ParallelData12_1;else ParallelData12_0;
}
