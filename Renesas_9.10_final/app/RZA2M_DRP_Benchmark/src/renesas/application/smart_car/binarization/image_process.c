/*****************************************************
               图像处理部分
*****************************************************/

#include "image_process.h"

//uint8_t bin_image[ROW][COL];

//static uint8_t BinArray[256];

//大津法取阈值ֵ
uint8_t Ostu(uint8_t *p_image)
{
    float histogram[256];
    int size = ROW*COL;
    int i;
    float t;
    float variance;
    float maxVariance = 0, w0 = 0, avgValue = 0;
    float u = 0;// 整幅图的平均灰度
    uint8_t Ostu_Threshold = 0;
    //清空灰度数组
    for(i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }
    //生成灰度直方图
    for (i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            ++histogram[*p_image];
            p_image++;
        }
    }

    for (i = 0; i < 256; ++i)//归一化
    {
        histogram[i] = histogram[i] / size;
        u += i * histogram[i];
    }

    for (i = 0; i < 256; ++i)
    {
        w0 += histogram[i];  //前景灰度
        avgValue += i * histogram[i];
        t = avgValue / w0 - u;
        variance = t * t * w0 / (1 - w0); // g = w0/(1-w0)*(u0-u1)^2
        if (variance > maxVariance)
        {   //找到使灰度差最大的值,为阈值ֵ
            maxVariance = variance;
            Ostu_Threshold = i;
        }
    }

    return Ostu_Threshold;
}
