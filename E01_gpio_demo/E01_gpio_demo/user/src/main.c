/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ���CH32V307VCT6 ��Դ���һ����
*
* CH32V307VCT6 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio V1.8.1
* ����ƽ̨          CH32V307VCT6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                             ��ע
* 2022-09-15        ��W            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"

// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ϵ�
//
// 2.���Կ������İ�����ɫ LED ��˸
//
// 3.����ͨ�����ߵ��Եķ�ʽ�鿴gpio_status���� ���鿴��ǰD0���ŵ�״̬

#define FREQ               (50)                                                // ����Ƶ��Ϊ50HZ
#define fly_R              (TIM4_PWM_MAP1_CH4_D15)
#define fly_L              (TIM4_PWM_MAP1_CH3_D14)
#define turn_L              (TIM4_PWM_MAP1_CH2_D13)
#define turn_R              (TIM4_PWM_MAP1_CH1_D12)


uint8 image[MT9V03X_H][MT9V03X_W]={0};
uint8 IMG[MT9V03X_H][MT9V03X_W]={0};
int rpts0[150][2]={0},rpts1[150][2]={0};//��¼��͸�Ӻ���߰��ߵ�x,y���� 0x,1y
uint8 rpts0_num=0,rpts1_num=0;//��¼��͸�Ӻ����ұ��ߵĸ���
float rpts0b[150][2]={0},rpts1b[150][2]={0};//��¼�˲�����߰��ߵ�x,y���� 0x,1y
uint8 rpts0b_num=0,rpts1b_num=0;//��¼�˲������ұ��ߵĸ���
float rpts0s[150][2]={0},rpts1s[150][2]={0};//��¼�Ⱦ��������߰��ߵ�x,y���� 0x,1y
uint8 rpts0s_num=0,rpts1s_num=0;//��¼�Ⱦ���������ұ��ߵĸ���
float rpts0a[150]={0},rpts1a[150]={0};//��¼���߾ֲ��Ƕȱ仯�ʺ���߰��ߵ�x,y���� 0x,1y
uint8 rpts0a_num=0,rpts1a_num=0;//��¼���߾ֲ��Ƕȱ仯�ʺ����ұ��ߵĸ���
float rpts0an[150]={0},rpts1an[150]={0};//��¼NMS����߰��ߵ�x,y���� 0x,1y
uint8 rpts0an_num=0,rpts1an_num=0;//��¼NMS�����ұ��ߵĸ���
float rptsc0[150][2]={0},rptsc1[150][2]={0};//��¼����׷�����ߺ����ߵ�x,y���� 0x,1y
uint8 rptsc0_num=0,rptsc1_num=0;//��¼����׷�����ߺ����ߵĸ���

int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L�ǵ�λ��
bool Lpt0_found, Lpt1_found;// L�ǵ�
uint8 is_straight0, is_straight1;// ��ֱ��
uint8 is_turn0, is_turn1;// ���
extern int far_Lpt0_rpts0s_id, far_Lpt1_rpts1s_id;
extern uint8 far_rpts0s_num, far_rpts1s_num;

float rpts[150][2];
uint8 rpts_num;

extern enum cross_type_e cross_type;
extern enum circle_type_e circle_type;
extern enum track_type_e track_type;

int aim_distance=10;//�������Ϊ0���յ���Ϊ1��aim_distance��Ϊ�����е�λ�ã����Ը��ݵ��ٺ͸���ʱȥ�ı����ֵ

float zonghe=0,error=0;
int motor_speed=0;
int speed=0;
int m1speed=0,m2speed=0;

#define LED1_PIN        (B2)

int main(void)
{
    clock_init(SYSTEM_CLOCK_120M);      // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
    debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    // �˴���д�û����� ���������ʼ�������
    ips114_set_dir(IPS114_PORTAIT);
    // ��ʼ��GPIO E2 E9Ϊ������� Ĭ������͵�ƽ
    gpio_init(LED1_PIN, GPO, 0, GPO_PUSH_PULL);
    mt9v03x_init();
    ips114_init();

    pwm_init(fly_L, FREQ, 500);                                                   // PWM ͨ��1 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(fly_R, FREQ, 500);                                                   // PWM ͨ��2 ��ʼ��Ƶ�� 50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(turn_L, FREQ, 500);                                                   // PWM ͨ��3 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(turn_R, FREQ, 500);                                                   // PWM ͨ��4 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    // �˴���д�û����� ���������ʼ�������

    while(1)//
    {
//        gpio_toggle_level(LED1_PIN);

        if(mt9v03x_finish_flag)
        {
//////////////////////////������ȡ������/////////////////////////////////////////////////////
            clear_IMG();//���IMG��image
//            get_image();//��ͼ��洢��image�����н��д���
////          gpio_toggle_level(LED1_PIN);
            image_Perspective_Correction();//��͸��
            find_leftline();//������
            find_rightline();//������
//            blur_points();//�˲�
            resample_points();//�Ⱦ����
            local_angle_points();//����������֮��ĽǶ�
            nms_angle();//NMS�Ǽ�������
            track_leftline();//����߸�������
            rptsc0_num = rpts0s_num;
            track_rightline();//�ұ��߸�������
            rptsc1_num = rpts1s_num;
///////////////////////////////////////////////////////////////////////////////////////////
            find_corners();//�ǵ���ȡ��ɸѡ
/////////////////////////////Ԫ���б�/////////////////////////////////////////////////////
////////////////////////// �������٣��л�Ѳ�߷���  ������Բ    /////////////////////////////////////////////
            if (rpts0s_num < rpts1s_num / 2 && rpts0s_num < 25)    track_type = TRACK_RIGHT;    //����߱��ұ����ٺܶ������£������ұ��߸�������
            else if (rpts1s_num < rpts0s_num / 2 && rpts1s_num < 25)    track_type = TRACK_LEFT;    //�ұ��߱�������ٺܶ������£���������߸�������
            else if (rpts0s_num < 10 && rpts1s_num > rpts0s_num)    track_type = TRACK_RIGHT;   //���ұ��߶����٣����ұ��߱�����߶�һ�㣬�����ұ��߸�������
            else if (rpts1s_num < 10 && rpts0s_num > rpts1s_num)   track_type = TRACK_LEFT;    //���ұ��߶����٣�������߱��ұ��߶�һ�㣬��������߸�������

            check_cross();//���ʮ��
            if(cross_type==CROSS_NONE)  check_circle();//����ʮ�֣����Բ��
////////////////////////////////////////////////////////////////////////////////////////////////////
            //���ݼ����ִ��ģʽ
            if(cross_type!=CROSS_NONE)   run_cross();
            if(circle_type!=CIRCLE_NONE)    run_circle();

//             ���߸���
            if (cross_type != CROSS_IN) //�Ǹ���Զ��ģʽ
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
                //ʮ�ָ���Զ�߿���
                if (track_type == TRACK_LEFT)    //����
                {
//                    track_leftline(far_rpts0s + far_Lpt0_rpts0s_id, far_rpts0s_num - far_Lpt0_rpts0s_id, rpts,10, 102 * 0.45 / 2);//��Զ�����ضϴ���
                    rpts_num = far_rpts0s_num - far_Lpt0_rpts0s_id;//�൱��������id֮ǰ�ĵ�
                }
                else                  //����
                {
//                    track_rightline(far_rpts1s + far_Lpt1_rpts1s_id, far_rpts1s_num - far_Lpt1_rpts1s_id, rpts,10, 102 * 0.45 / 2);//��Զ�����ضϴ���
                    rpts_num = far_rpts1s_num - far_Lpt1_rpts1s_id;//�൱��������id֮ǰ�ĵ�
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
            //pwm ���m1speed �Ҹ�m2speed


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






