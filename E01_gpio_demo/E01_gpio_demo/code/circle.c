/*
 * circle.c
 *
 *  Created on: 2024��6��5��
 *      Author: admin
 */
#include "zf_common_headfile.h"
#include "math.h"



enum circle_type_e circle_type = CIRCLE_NONE;

extern enum track_type_e track_type;

extern uint8 rpts0s_num,rpts1s_num;
extern uint8 rpts0s_num,rpts1s_num;
extern uint8 rptsc0_num,rptsc1_num;

extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L�ǵ�λ��
extern uint8 Lpt0_found, Lpt1_found;// L�ǵ�
extern uint8 is_straight0, is_straight1;// ��ֱ��


int none_left_line = 0, none_right_line = 0;
int have_left_line = 0, have_right_line = 0;

void check_circle(void)
{
    // ��Բ��ģʽ�£�����L�ǵ�, ���߳�ֱ��
    if (circle_type == CIRCLE_NONE && Lpt0_found && !Lpt1_found && is_straight1)//���L�ǵ㣬�ұ߳�ֱ��
    {
        circle_type = CIRCLE_LEFT_BEGIN;//����Բ��
    }
    if (circle_type == CIRCLE_NONE && !Lpt0_found && Lpt1_found && is_straight0)//�ұ�L�ǵ㣬��߳�ֱ��
    {
        circle_type = CIRCLE_RIGHT_BEGIN;//����Բ��
    }
}

void run_circle(void)
{
    // �󻷿�ʼ��Ѱ��ֱ������
    if (circle_type == CIRCLE_LEFT_BEGIN)
    {
        track_type = TRACK_RIGHT;

        //�ȶ����ߺ�����
        if (rpts0s_num < 10) none_left_line++;  //���������С��һ��ֵ����Ϊ����߶���
        if (rpts0s_num > 50 && none_left_line > 2) //����߶��ߺ�������
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
    //�뻷��Ѱ��Բ����
    else if (circle_type == CIRCLE_LEFT_IN)
    {
        track_type = TRACK_LEFT;

        //����������1/4Բ   Ӧ����Ϊ����Ϊת���޹յ�
//        if (rpts0s_num < 0.1 / 0.02 ||
//            current_encoder - circle_encoder >= ENCODER_PER_METER * (3.14 * 1 / 2)) { circle_type = CIRCLE_LEFT_RUNNING; }
    }
    //����Ѳ�ߣ�Ѱ��Բ����
    else if (circle_type == CIRCLE_LEFT_RUNNING)
    {
        track_type = TRACK_RIGHT;

        if (Lpt1_found) rpts1s_num = rptsc1_num = Lpt1_rpts1s_id;   //������L�ǵ�
        //�⻷�յ�(��L��)
        if (Lpt1_found && Lpt1_rpts1s_id < 20) circle_type = CIRCLE_LEFT_OUT;   //����L�ǵ㲢�����ҹյ�ܽ���׼����Բ
    }
    //������Ѱ��Բ
    else if (circle_type == CIRCLE_LEFT_OUT)    //����
    {
        track_type = TRACK_LEFT;

        //����Ϊ��ֱ��
        if (is_straight1)   circle_type = CIRCLE_LEFT_END;//�Ѿ�������
    }
    //�߹�Բ����Ѱ����
    else if (circle_type == CIRCLE_LEFT_END)
    {
        track_type = TRACK_RIGHT;

        //�����ȶ�����
        if (rpts0s_num < 10) none_left_line++;  //������
        if (rpts0s_num > 50 && none_left_line > 3) //�����ߺ�������
        {
            circle_type = CIRCLE_NONE;
            none_left_line = 0;
        }
    }
    //�һ����ƣ�ǰ��Ѱ��ֱ��
    else if (circle_type == CIRCLE_RIGHT_BEGIN)
    {
        track_type = TRACK_LEFT;

        //�ȶ����ߺ�����
        if (rpts1s_num < 10) none_right_line++; //������
        if (rpts1s_num > 50 && none_right_line > 2) //�����ߺ�������
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
    //���һ���Ѱ����Բ��
    else if (circle_type == CIRCLE_RIGHT_IN)
    {
        track_type = TRACK_RIGHT;

        //����������1/4Բ   Ӧ����Ϊ����Ϊת���޹յ�
//        if (rpts1s_num < 0.1 / sample_dist ||
//            current_encoder - circle_encoder >= ENCODER_PER_METER * (3.14 * 1 / 2)) { circle_type = CIRCLE_RIGHT_RUNNING; }
//
    }
    //����Ѳ�ߣ�Ѱ��Բ����
    else if (circle_type == CIRCLE_RIGHT_RUNNING)
    {
        track_type = TRACK_LEFT;

        //�⻷���ڹյ�,���ټӹյ�����о�(��L��)
        if (Lpt0_found) rpts0s_num = rptsc0_num = Lpt0_rpts0s_id;   //������L�յ�
        if (Lpt0_found && Lpt0_rpts0s_id < 20)  //������L�յ㲢������յ�ܽ���׼����Բ
        {
            circle_type = CIRCLE_RIGHT_OUT;
        }
    }
    //������Ѱ��Բ
    else if (circle_type == CIRCLE_RIGHT_OUT)
    {
        track_type = TRACK_RIGHT;

        //�󳤶ȼ���б�Ƕ�  Ӧ�����������ҵ���Ϊֱ��
        if((rpts1s_num >100 && !Lpt1_found))    have_right_line++;
        if (is_straight0)   circle_type = CIRCLE_RIGHT_END; //��ֱ��
    }
        //�߹�Բ����Ѱ����
    else if (circle_type == CIRCLE_RIGHT_END) {
        track_type = TRACK_LEFT;

        //�����ȶ�����
        if (rpts1s_num < 10)    none_right_line++;  //�Ҷ���
        if (rpts1s_num > 50 && none_right_line > 2) //�Ҷ��ߺ�������
        {
            circle_type = CIRCLE_NONE;
            none_right_line = 0;
        }
    }
}
