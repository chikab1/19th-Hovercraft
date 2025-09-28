/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是CH32V307VCT6 开源库的一部分
*
* CH32V307VCT6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio V1.8.1
* 适用平台          CH32V307VCT6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期                                      作者                             备注
* 2022-09-15        大W            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，完成上电
//
// 2.可以看到核心板上蓝色 LED 闪烁
//
// 3.可以通过在线调试的方式查看gpio_status变量 来查看当前D0引脚的状态

#define FREQ               (50)                                                // 控制频率为50HZ
#define fly_R              (TIM4_PWM_MAP1_CH4_D15)
#define fly_L              (TIM4_PWM_MAP1_CH3_D14)
#define turn_L              (TIM4_PWM_MAP1_CH2_D13)
#define turn_R              (TIM4_PWM_MAP1_CH1_D12)


uint8 image[MT9V03X_H][MT9V03X_W]={0};
uint8 IMG[MT9V03X_H][MT9V03X_W]={0};
int rpts0[150][2]={0},rpts1[150][2]={0};//记录逆透视后边线白线的x,y坐标 0x,1y
uint8 rpts0_num=0,rpts1_num=0;//记录逆透视后左右边线的个数
float rpts0b[150][2]={0},rpts1b[150][2]={0};//记录滤波后边线白线的x,y坐标 0x,1y
uint8 rpts0b_num=0,rpts1b_num=0;//记录滤波后左右边线的个数
float rpts0s[150][2]={0},rpts1s[150][2]={0};//记录等距采样后边线白线的x,y坐标 0x,1y
uint8 rpts0s_num=0,rpts1s_num=0;//记录等距采样后左右边线的个数
float rpts0a[150]={0},rpts1a[150]={0};//记录边线局部角度变化率后边线白线的x,y坐标 0x,1y
uint8 rpts0a_num=0,rpts1a_num=0;//记录边线局部角度变化率后左右边线的个数
float rpts0an[150]={0},rpts1an[150]={0};//记录NMS后边线白线的x,y坐标 0x,1y
uint8 rpts0an_num=0,rpts1an_num=0;//记录NMS后左右边线的个数
float rptsc0[150][2]={0},rptsc1[150][2]={0};//记录边线追踪中线后中线的x,y坐标 0x,1y
uint8 rptsc0_num=0,rptsc1_num=0;//记录边线追踪中线后中线的个数

int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L角点位置
bool Lpt0_found, Lpt1_found;// L角点
uint8 is_straight0, is_straight1;// 长直道
uint8 is_turn0, is_turn1;// 弯道
extern int far_Lpt0_rpts0s_id, far_Lpt1_rpts1s_id;
extern uint8 far_rpts0s_num, far_rpts1s_num;

float rpts[150][2];
uint8 rpts_num;

extern enum cross_type_e cross_type;
extern enum circle_type_e circle_type;
extern enum track_type_e track_type;

int aim_distance=10;//将起点设为0，终点设为1，aim_distance则为在其中的位置，可以根据低速和高速时去改变这个值

float zonghe=0,error=0;
int motor_speed=0;
int speed=0;
int m1speed=0,m2speed=0;

#define LED1_PIN        (B2)

int main(void)
{
    clock_init(SYSTEM_CLOCK_120M);      // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口

    // 此处编写用户代码 例如外设初始化代码等
    ips114_set_dir(IPS114_PORTAIT);
    // 初始化GPIO E2 E9为推挽输出 默认输出低电平
    gpio_init(LED1_PIN, GPO, 0, GPO_PUSH_PULL);
    mt9v03x_init();
    ips114_init();

    pwm_init(fly_L, FREQ, 500);                                                   // PWM 通道1 初始化频率50Hz  占空比初始为 0
    pwm_init(fly_R, FREQ, 500);                                                   // PWM 通道2 初始化频率 50Hz  占空比初始为 0
    pwm_init(turn_L, FREQ, 500);                                                   // PWM 通道3 初始化频率50Hz  占空比初始为 0
    pwm_init(turn_R, FREQ, 500);                                                   // PWM 通道4 初始化频率50Hz  占空比初始为 0
    // 此处编写用户代码 例如外设初始化代码等

    while(1)//
    {
//        gpio_toggle_level(LED1_PIN);

        if(mt9v03x_finish_flag)
        {
//////////////////////////边线提取，处理/////////////////////////////////////////////////////
            clear_IMG();//清空IMG和image
//            get_image();//将图像存储到image数组中进行处理
////          gpio_toggle_level(LED1_PIN);
            image_Perspective_Correction();//逆透视
            find_leftline();//找左线
            find_rightline();//找右线
//            blur_points();//滤波
            resample_points();//等距采样
            local_angle_points();//计算三个点之间的角度
            nms_angle();//NMS非极大抑制
            track_leftline();//左边线跟踪中线
            rptsc0_num = rpts0s_num;
            track_rightline();//右边线跟踪中线
            rptsc1_num = rpts1s_num;
///////////////////////////////////////////////////////////////////////////////////////////
            find_corners();//角点提取，筛选
/////////////////////////////元素判别/////////////////////////////////////////////////////
////////////////////////// 单侧线少，切换巡线方向  切外向圆    /////////////////////////////////////////////
            if (rpts0s_num < rpts1s_num / 2 && rpts0s_num < 25)    track_type = TRACK_RIGHT;    //左边线比右边线少很多的情况下，采用右边线跟踪中线
            else if (rpts1s_num < rpts0s_num / 2 && rpts1s_num < 25)    track_type = TRACK_LEFT;    //右边线比左边线少很多的情况下，采用左边线跟踪中线
            else if (rpts0s_num < 10 && rpts1s_num > rpts0s_num)    track_type = TRACK_RIGHT;   //左右边线都很少，但右边线比左边线多一点，采用右边线跟踪中线
            else if (rpts1s_num < 10 && rpts0s_num > rpts1s_num)   track_type = TRACK_LEFT;    //左右边线都很少，但左边线比右边线多一点，采用左边线跟踪中线

            check_cross();//检查十字
            if(cross_type==CROSS_NONE)  check_circle();//不是十字，检查圆环
////////////////////////////////////////////////////////////////////////////////////////////////////
            //根据检查结果执行模式
            if(cross_type!=CROSS_NONE)   run_cross();
            if(circle_type!=CIRCLE_NONE)    run_circle();

//             中线跟踪
            if (cross_type != CROSS_IN) //非跟踪远线模式
            {
                if (track_type == TRACK_LEFT)
                {
                    for(int i=0;i<rptsc0_num;i++)
                    {
                        rpts[i][0]=rptsc0[i][0];
                        rpts[i][1]=rptsc0[i][1];
                    }
                    rpts_num = rptsc0_num;
                }
                else
                {
                    for(int i=0;i<rptsc1_num;i++)
                    {
                        rpts[i][0]=rptsc1[i][0];
                        rpts[i][1]=rptsc1[i][1];
                    }
                    rpts_num = rptsc1_num;
                }
            }
            else
            {
                //十字根据远线控制
                if (track_type == TRACK_LEFT)    //左线
                {
//                    track_leftline(far_rpts0s + far_Lpt0_rpts0s_id, far_rpts0s_num - far_Lpt0_rpts0s_id, rpts,10, 102 * 0.45 / 2);//对远线做截断处理
                    rpts_num = far_rpts0s_num - far_Lpt0_rpts0s_id;//相当于舍弃乐id之前的点
                }
                else                  //右线
                {
//                    track_rightline(far_rpts1s + far_Lpt1_rpts1s_id, far_rpts1s_num - far_Lpt1_rpts1s_id, rpts,10, 102 * 0.45 / 2);//对远线做截断处理
                    rpts_num = far_rpts1s_num - far_Lpt1_rpts1s_id;//相当于舍弃乐id之前的点
                }
            }

            zonghe=0;
            for(int i=0;i<30;i++)
            {
                zonghe+=rptsc0[i][0];
            }
            error=zonghe/30-rptsc0[aim_distance][0];
            motor_speed=Position_PID(error);
            m1speed=speed+motor_speed;
            m2speed=speed-motor_speed;
            //pwm 左给m1speed 右给m2speed


//            for(int i=0;i<rpts0_num;i++)
//            {
//                ips114_show_float(0,20,rpts0b[i][0],3,3);
//                ips114_show_float(0,40,rpts0b[i][1],3,3);
//                system_delay_ms(200);
//            }



//
////          ips114_show_uint(100,60,300,3);
//
//
//        for(int i=0;i<rptsc0_num;i++)
//        {
//          ips114_draw_point(rptsc0[i][0],rptsc0[i][1],RGB565_BLUE);
//        }
//        for(int j=0;j<rptsc0_num;j++)
//        {
//          ips114_draw_point(rptsc1[j][0],rptsc1[j][1],RGB565_RED);
//        }




//            if(Lpt0_found)  ips114_show_uint(200,0,1,3);
//            else    ips114_show_uint(200,0,0,3);
//            if(Lpt1_found)  ips114_show_uint(200,20,1,3);
//            else    ips114_show_uint(200,20,0,3);
//            ips114_show_uint(200,40,Lpt0_rpts0s_id,3);
//            ips114_show_uint(200,60,Lpt1_rpts1s_id,3);
//            system_delay_ms(200);

//            ips114_displayimage03x(image[0], 188, 120);
            mt9v03x_finish_flag=0;
        }
    }
}






