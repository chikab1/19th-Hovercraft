/*
 * circle.c
 *
 *  Created on: 2024年6月5日
 *      Author: admin
 */
#include "zf_common_headfile.h"
#include "math.h"



enum circle_type_e circle_type = CIRCLE_NONE;

extern enum track_type_e track_type;

extern uint8 rpts0s_num,rpts1s_num;
extern uint8 rpts0s_num,rpts1s_num;
extern uint8 rptsc0_num,rptsc1_num;

extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L角点位置
extern uint8 Lpt0_found, Lpt1_found;// L角点
extern uint8 is_straight0, is_straight1;// 长直道


int none_left_line = 0, none_right_line = 0;
int have_left_line = 0, have_right_line = 0;

void check_circle(void)
{
    // 非圆环模式下，单边L角点, 单边长直道
    if (circle_type == CIRCLE_NONE && Lpt0_found && !Lpt1_found && is_straight1)//左边L角点，右边长直道
    {
        circle_type = CIRCLE_LEFT_BEGIN;//进左圆环
    }
    if (circle_type == CIRCLE_NONE && !Lpt0_found && Lpt1_found && is_straight0)//右边L角点，左边长直道
    {
        circle_type = CIRCLE_RIGHT_BEGIN;//进右圆环
    }
}

void run_circle(void)
{
    // 左环开始，寻外直道右线
    if (circle_type == CIRCLE_LEFT_BEGIN)
    {
        track_type = TRACK_RIGHT;

        //先丢左线后有线
        if (rpts0s_num < 10) none_left_line++;  //左边线数量小于一定值，视为左边线丢线
        if (rpts0s_num > 50 && none_left_line > 2) //左边线丢线后再有线
        {
            have_left_line++;
            if (have_left_line > 1)
            {
                circle_type = CIRCLE_LEFT_IN;
                none_left_line = 0;
                have_left_line = 0;
            }
        }
    }
    //入环，寻内圆左线
    else if (circle_type == CIRCLE_LEFT_IN)
    {
        track_type = TRACK_LEFT;

        //编码器打表过1/4圆   应修正为右线为转弯无拐点
//        if (rpts0s_num < 0.1 / 0.02 ||
//            current_encoder - circle_encoder >= ENCODER_PER_METER * (3.14 * 1 / 2)) { circle_type = CIRCLE_LEFT_RUNNING; }
    }
    //正常巡线，寻外圆右线
    else if (circle_type == CIRCLE_LEFT_RUNNING)
    {
        track_type = TRACK_RIGHT;

        if (Lpt1_found) rpts1s_num = rptsc1_num = Lpt1_rpts1s_id;   //发现右L角点
        //外环拐点(右L点)
        if (Lpt1_found && Lpt1_rpts1s_id < 20) circle_type = CIRCLE_LEFT_OUT;   //右有L角点并且离右拐点很近，准备出圆
    }
    //出环，寻内圆
    else if (circle_type == CIRCLE_LEFT_OUT)    //出环
    {
        track_type = TRACK_LEFT;

        //右线为长直道
        if (is_straight1)   circle_type = CIRCLE_LEFT_END;//已经出环了
    }
    //走过圆环，寻右线
    else if (circle_type == CIRCLE_LEFT_END)
    {
        track_type = TRACK_RIGHT;

        //左线先丢后有
        if (rpts0s_num < 10) none_left_line++;  //丢左线
        if (rpts0s_num > 50 && none_left_line > 3) //丢左线后又有线
        {
            circle_type = CIRCLE_NONE;
            none_left_line = 0;
        }
    }
    //右环控制，前期寻左直道
    else if (circle_type == CIRCLE_RIGHT_BEGIN)
    {
        track_type = TRACK_LEFT;

        //先丢右线后有线
        if (rpts1s_num < 10) none_right_line++; //丢右线
        if (rpts1s_num > 50 && none_right_line > 2) //丢右线后又有线
        {
            have_right_line++;
            if (have_right_line > 1)
            {
                circle_type = CIRCLE_RIGHT_IN;
                none_right_line = 0;
                have_right_line = 0;
            }
        }
    }
    //入右环，寻右内圆环
    else if (circle_type == CIRCLE_RIGHT_IN)
    {
        track_type = TRACK_RIGHT;

        //编码器打表过1/4圆   应修正为左线为转弯无拐点
//        if (rpts1s_num < 0.1 / sample_dist ||
//            current_encoder - circle_encoder >= ENCODER_PER_METER * (3.14 * 1 / 2)) { circle_type = CIRCLE_RIGHT_RUNNING; }
//
    }
    //正常巡线，寻外圆左线
    else if (circle_type == CIRCLE_RIGHT_RUNNING)
    {
        track_type = TRACK_LEFT;

        //外环存在拐点,可再加拐点距离判据(左L点)
        if (Lpt0_found) rpts0s_num = rptsc0_num = Lpt0_rpts0s_id;   //存在左L拐点
        if (Lpt0_found && Lpt0_rpts0s_id < 20)  //存在左L拐点并且离左拐点很近，准备出圆
        {
            circle_type = CIRCLE_RIGHT_OUT;
        }
    }
    //出环，寻内圆
    else if (circle_type == CIRCLE_RIGHT_OUT)
    {
        track_type = TRACK_RIGHT;

        //左长度加倾斜角度  应修正左右线找到且为直线
        if((rpts1s_num >100 && !Lpt1_found))    have_right_line++;
        if (is_straight0)   circle_type = CIRCLE_RIGHT_END; //左长直线
    }
        //走过圆环，寻左线
    else if (circle_type == CIRCLE_RIGHT_END) {
        track_type = TRACK_LEFT;

        //右线先丢后有
        if (rpts1s_num < 10)    none_right_line++;  //右丢线
        if (rpts1s_num > 50 && none_right_line > 2) //右丢线后又有线
        {
            circle_type = CIRCLE_NONE;
            none_right_line = 0;
        }
    }
}
