/*
 * cross.c
 *
 *  Created on: 2024年6月5日
 *      Author: admin
 */
#include "zf_common_headfile.h"
#include "math.h"


enum cross_type_e cross_type = CROSS_NONE;

enum track_type_e track_type = TRACK_RIGHT;

extern uint8 image[MT9V03X_H][MT9V03X_W];

float far_rpts0[150][2];
float far_rpts1[150][2];
uint8 far_rpts0_num, far_rpts1_num;

float far_rpts0b[150][2];
float far_rpts1b[150][2];
uint8 far_rpts0b_num, far_rpts1b_num;

float far_rpts0s[150][2];
float far_rpts1s[150][2];
uint8 far_rpts0s_num, far_rpts1s_num;

float far_rpts0a[150];
float far_rpts1a[150];
uint8 far_rpts0a_num, far_rpts1a_num;

float far_rpts0an[150];
float far_rpts1an[150];
uint8 far_rpts0an_num, far_rpts1an_num;

extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L角点位置
extern bool Lpt0_found, Lpt1_found;// L角点

extern uint8 rptsc0_num,rptsc1_num;//记录边线追踪中线后中线的个数
extern uint8 rpts0s_num,rpts1s_num;//记录等距采样后左右边线的个数

uint8 far_x1, far_x2, far_y1, far_y2;
int not_have_line = 0;

bool far_Lpt0_found, far_Lpt1_found;
int far_Lpt0_rpts0s_id, far_Lpt1_rpts1s_id;
//uint8 cross_type=0; //0为非十字模式，1为找到左右两个L角点，2为两个L角点很近，即进入十字内部(此时切换远线控制)


void check_cross(void)//双L角点，切换十字模式
{
    bool Xfound = Lpt0_found && Lpt1_found;   //找到左右两个拐点，即为十字
    if (cross_type == CROSS_NONE && Xfound) cross_type = CROSS_BEGIN;
}

void run_cross(void)
{
    bool Xfound = Lpt0_found && Lpt1_found;

    //检测到十字，先按照近线走
    if (cross_type == CROSS_BEGIN) //做截断处理
    {
        if (Lpt0_found==1) rptsc0_num = rpts0s_num = Lpt0_rpts0s_id;
        if (Lpt1_found==1) rptsc1_num = rpts1s_num = Lpt1_rpts1s_id;

//        aim_distance = 0.4;
        //近角点过少，进入远线控制
        if (Xfound && (Lpt0_rpts0s_id < 5 || Lpt1_rpts1s_id < 5))    //左边线和右边线仅剩很少时，穿过十字
        {
            cross_type = CROSS_IN;
//            cross_encoder = current_encoder;
        }
    }
        //远线控制进十字,begin_y渐变靠近防丢线
    else if (cross_type == CROSS_IN) {
        //寻远线,算法与近线相同
        cross_farline();

        if (rpts1s_num < 5 && rpts0s_num < 5) not_have_line++;
        if (not_have_line > 2 && rpts1s_num > 20 && rpts0s_num > 20)//先经过没有线然后又有线说明从十字驶离了
        {
            cross_type = CROSS_NONE;
            not_have_line = 0;
        }
        if (far_Lpt1_found) track_type = TRACK_RIGHT;
        else if (far_Lpt0_found) track_type = 0;
        else if (not_have_line > 0 && rpts1s_num < 5) track_type = 1;
        else if (not_have_line > 0 && rpts0s_num < 5) track_type = 0;

    }
}

void cross_farline(void)
{
    uint16 sum=0;
    int yuzhi=0;
    int x1 = 94 - 50, y1 = 90;
    far_y1 = 0, far_y2 = 0;
    bool white_found = false;

    image_Perspective_Correction();//透视变换

    for(;y1>0;y1--)         //从左边界开始向上找
    {
        sum=0,yuzhi=0;
        for(int i=x1-3;i<=x1+3;i++)//对于以该点为中心的7*7范围内计算自适应阈值
        {
            for(int j=y1-3;j<=y1+3;j++)
            {
                sum+=mt9v03x_image[j][i];
            }
        }
        yuzhi=sum/49-3;//后面的减去的数值可以依据经验给定(2-5)即可
        if(image[y1][x1] >= yuzhi)  white_found = true;
        if(image[y1][x1] < yuzhi && white_found)
        {
            far_x1=x1;
            far_y1=y1;
            break;
        }
    }
    if(image[far_y1+1][far_x1] >= yuzhi)    find_farleftline();//左手巡线
    else far_rpts0_num = 0;


    int x2= 94 + 50,y2 = 90;
    white_found = false;
    for(;y2>0;y2--)         //从左边界开始向上找
    {
        sum=0,yuzhi=0;
        for(int i=x2-3;i<=x2+3;i++)//对于以该点为中心的7*7范围内计算自适应阈值
        {
            for(int j=y2-3;j<=y2+3;j++)
            {
                sum+=mt9v03x_image[j][i];
            }
        }
        yuzhi=sum/49-3;//后面的减去的数值可以依据经验给定(2-5)即可
        if(image[y2][x2] >= yuzhi)  white_found = true;
        if(image[y2][x2] < yuzhi && white_found)
        {
            far_x2=x2;
            far_y2=y2;
            break;
        }
    }
    if(image[far_y2+1][far_x2] >= yuzhi)    find_farrightline();//右手巡线
    else far_rpts1_num = 0;


    far_blur_points();// 边线滤波
    far_resample_points();// 边线等距采样
    far_local_angle_points();// 边线局部角度变化率
    far_nms_angle();// 角度变化率非极大抑制
// 找远线上的L角点
    far_Lpt0_found = far_Lpt1_found = false;
    for (int i = 0; i < MIN(far_rpts0s_num, 80); i++)
    {
        if (far_rpts0an[i] == 0) continue;
        int im1 = limit(i - 10, 0, far_rpts0s_num - 1);
        int ip1 = limit(i + 10, 0, far_rpts0s_num - 1);
        float conf = fabs(far_rpts0a[i]) - (fabs(far_rpts0a[im1]) + fabs(far_rpts0a[ip1])) / 2;
        if (70. / 180. * PI < conf && conf < 110. / 180. * PI && i < 100)
        {
            far_Lpt0_rpts0s_id = i;
            far_Lpt0_found = true;
            break;
        }
    }
    for (int i = 0; i < MIN(far_rpts1s_num, 80); i++)
    {
        if (far_rpts1an[i] == 0) continue;
        int im1 = limit(i - 10, 0, far_rpts1s_num - 1);
        int ip1 = limit(i + 10, 0, far_rpts1s_num - 1);
        float conf = fabs(far_rpts1a[i]) - (fabs(far_rpts1a[im1]) + fabs(far_rpts1a[ip1])) / 2;

        if (70. / 180. * PI < conf && conf < 110. / 180. * PI && i < 100)
        {
            far_Lpt1_rpts1s_id = i;
            far_Lpt1_found = true;
            break;
        }
    }
}

