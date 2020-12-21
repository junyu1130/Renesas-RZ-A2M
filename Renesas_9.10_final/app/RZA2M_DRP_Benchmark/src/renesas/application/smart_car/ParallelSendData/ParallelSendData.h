/*
 * ParallelSendData.h
 *
 *  Created on: 2020年8月23日
 *      Author: Y
 */

#ifndef RENESAS_APPLICATION_SMART_CAR_PARALLELSENDDATA_PARALLELSENDDATA_H_
#define RENESAS_APPLICATION_SMART_CAR_PARALLELSENDDATA_PARALLELSENDDATA_H_

#include <smart_car.h>

#define ParallelData0_1 	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p64_hi)
#define ParallelData0_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p64_lo)
#define ParallelData1_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p65_hi)
#define ParallelData1_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p65_lo)
#define ParallelData2_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p74_hi)
#define ParallelData2_0		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p74_lo)
#define ParallelData3_1 	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p66_hi)
#define ParallelData3_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p66_lo)
#define ParallelData4_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p75_hi)
#define ParallelData4_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p75_lo)
#define ParallelData5_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p97_hi)
#define ParallelData5_0		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p97_lo)
#define ParallelData6_1 	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p67_hi)
#define ParallelData6_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p67_lo)
#define ParallelData7_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p96_hi)
#define ParallelData7_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p96_lo)
#define ParallelData8_1		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p70_hi)
#define ParallelData8_0		direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p70_lo)
#define ParallelData9_1 	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p95_hi)
#define ParallelData9_0     direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p95_lo)
#define ParallelData10_1	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p71_hi)
#define ParallelData10_0    direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p71_lo)
#define ParallelData11_1	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p94_hi)
#define ParallelData11_0	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p94_lo)
#define ParallelData12_1 	direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p73_hi)
#define ParallelData12_0    direct_control(s_my_gpio_handle, CTL_GPIO_PIN_WRITE, &s_p73_lo)

int_t parallelSendData_Init(void);
//int Read_Bit(int_t handle ,st_r_drv_gpio_pin_rw_t pin);
void parallelSendData(u16 data);//并口发送13位数据

#endif /* RENESAS_APPLICATION_SMART_CAR_PARALLELSENDDATA_PARALLELSENDDATA_H_ */
