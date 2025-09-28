/*
 * pid.c
 *
 *  Created on: 2024��6��8��
 *      Author: admin
 */
#include "zf_common_headfile.h"
int KP_p=0,KI_p=0,KD_p=0;

float Position_PID (float error)
{
    float pwm=0;
    float Integral_error=0,last_error=0;
     Integral_error+=error;                                    //���ƫ��Ļ���
    if(Integral_error>2000)Integral_error=2000;
    if(Integral_error<-2000)Integral_error=-2000;
     pwm=KP_p*error+KI_p*Integral_error+KD_p*(error-last_error);       //λ��ʽPID������
     last_error=error;                                       //������һ��ƫ��
     return pwm;                                           //�������
}
