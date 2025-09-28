/*
 * pid.c
 *
 *  Created on: 2024年6月8日
 *      Author: admin
 */
#include "zf_common_headfile.h"
int KP_p=0,KI_p=0,KD_p=0;

float Position_PID (float error)
{
    float pwm=0;
    float Integral_error=0,last_error=0;
     Integral_error+=error;                                    //求出偏差的积分
    if(Integral_error>2000)Integral_error=2000;
    if(Integral_error<-2000)Integral_error=-2000;
     pwm=KP_p*error+KI_p*Integral_error+KD_p*(error-last_error);       //位置式PID控制器
     last_error=error;                                       //保存上一次偏差
     return pwm;                                           //增量输出
}
