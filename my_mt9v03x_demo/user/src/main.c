#include "zf_common_headfile.h"
#include "img.h"

#define WIFI_SSID_TEST          "506"
#define WIFI_PASSWORD_TEST      "504~freescale" // �����Ҫ���ӵ�WIFI û����������Ҫ�� ���� �滻Ϊ NULL

// ͼ�񱸷����飬�ڷ���ǰ��ͼ�񱸷��ٽ��з��ͣ��������Ա���ͼ�����˺�ѵ�����
uint8 image_copy[MT9V03X_H][MT9V03X_W];
uint8 gpio_status;

#define FREQ               (50)                                                // ����Ƶ��Ϊ50HZ
#define fly_R              (TIM4_PWM_MAP1_CH4_D15)
#define fly_L              (TIM4_PWM_MAP1_CH3_D14)
#define turn_L              (TIM4_PWM_MAP1_CH2_D13)
#define turn_R              (TIM4_PWM_MAP1_CH1_D12)
#define LED1_PIN        (B2)
#define LED2_PIN        (B1)
#define LED3_PIN        (B0)
#define PIT_CH                  (TIM2_PIT )                                      // ʹ�õ������жϱ�� ����޸� ��Ҫͬ����Ӧ�޸������жϱ���� isr.c �еĵ���
#define PIT_PRIORITY            (TIM2_IRQn)                                      // ��Ӧ�����жϵ��жϱ��
int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);      // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
    debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    pwm_init(fly_L, FREQ, 500);                                                   // PWM ͨ��1 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(fly_R, FREQ, 500);                                                   // PWM ͨ��2 ��ʼ��Ƶ�� 50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(turn_L, FREQ, 500);                                                   // PWM ͨ��3 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    pwm_init(turn_R, FREQ, 500);                                                   // PWM ͨ��4 ��ʼ��Ƶ��50Hz  ռ�ձȳ�ʼΪ 0
    gpio_init(LED1_PIN, GPO, 0, GPO_PUSH_PULL);
    gpio_init(LED2_PIN, GPO, 0, GPO_PUSH_PULL);
    gpio_init(LED3_PIN, GPO, 0, GPO_PUSH_PULL);
    // ��ˢ���ת�� 0%   Ϊ 500
    // ��ˢ���ת�� 20%  Ϊ 600
    // ��ˢ���ת�� 40%  Ϊ 700
    // ��ˢ���ת�� 60%  Ϊ 800
    // ��ˢ���ת�� 80%  Ϊ 900
    // ��ˢ���ת�� 100% Ϊ 1000
    pit_ms_init(PIT_CH, 10);                                                  // ��ʼ�� PIT_CH0 Ϊ�����ж� 1000ms ����
    interrupt_set_priority(PIT_PRIORITY, 0);                                    // ���� PIT1 �������жϵ��ж����ȼ�Ϊ 0

    ips114_init();
    ips114_show_string(0,0,"Hello");
    key_init(10);
    wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST);
    mt9v03x_init();
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy[0], MT9V03X_W, MT9V03X_H);
    // �˴���д�û����� ���������ʼ�������
    while(1)
    {
        if(key_get_state(KEY_1)==KEY_LONG_PRESS)
        {
            pwm_set_duty(fly_L, 700);
            pwm_set_duty(fly_R, 700);
            pwm_set_duty(turn_L, 600);
            pwm_set_duty(turn_R, 600);
        }
        if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
        {
            pwm_set_duty(fly_L, 500);
            pwm_set_duty(fly_R, 500);
            pwm_set_duty(turn_L, 500);
            pwm_set_duty(turn_R, 500);
        }
//        system_delay_ms(10);
        if(mt9v03x_finish_flag)
        {
            uint8 i=0,j=0;
            uint8 yuzhi=0;
            yuzhi=my_adapt_threshold( mt9v03x_image[0],188,120);
            for(i=0;i<120;i++)
            {
                for(j=0;j<188;j++)
                {
                    if(mt9v03x_image[i][j]>yuzhi) mt9v03x_image[i][j]=255;
                    else mt9v03x_image[i][j]=0;
                }
            }

            //ips114_displayimage03x(mt9v03x_image[0],94,60);
            //my_adapt_threshold( mt9v03x_image[0],188,120);
            mt9v03x_finish_flag = 0;

            // �ڷ���ǰ��ͼ�񱸷��ٽ��з��ͣ��������Ա���ͼ�����˺�ѵ�����
            memcpy(image_copy[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
            // ����ͼ��
            seekfree_assistant_camera_send();
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
void pit_handler (void)
{
    key_scanner();
    //gpio_toggle_level(LED3_PIN);//D5���ŵ�ƽ��ת
}
//void process_image() {
//    // ԭͼ�����ұ���
//    float begin_x = 0;
//    float begin_y = 0;
//    int x1 = img_raw.width / 2 - begin_x, y1 = begin_y;
//    ipts0_num = sizeof(ipts0) / sizeof(ipts0[0]);
//    for (; x1 > 0; x1--) if (AT_IMAGE(&img_raw, x1 - 1, y1) < thres) break;
//    if (AT_IMAGE(&img_raw, x1, y1) >= thres)
//        findline_lefthand_adaptive(&img_raw, block_size, clip_value, x1, y1, ipts0, &ipts0_num);
//    else ipts0_num = 0;
//    int x2 = img_raw.width / 2 + begin_x, y2 = begin_y;
//    ipts1_num = sizeof(ipts1) / sizeof(ipts1[0]);
//    for (; x2 < img_raw.width - 1; x2++) if (AT_IMAGE(&img_raw, x2 + 1, y2) < thres) break;
//    if (AT_IMAGE(&img_raw, x2, y2) >= thres)
//        findline_righthand_adaptive(&img_raw, block_size, clip_value, x2, y2, ipts1, &ipts1_num);
//    else ipts1_num = 0;
//
//    // ȥ����+͸�ӱ任
//    for (int i = 0; i < ipts0_num; i++) {
//        rpts0[i][0] = mapx[ipts0[i][1]][ipts0[i][0]];
//        rpts0[i][1] = mapy[ipts0[i][1]][ipts0[i][0]];
//    }
//    rpts0_num = ipts0_num;
//    for (int i = 0; i < ipts1_num; i++) {
//        rpts1[i][0] = mapx[ipts1[i][1]][ipts1[i][0]];
//        rpts1[i][1] = mapy[ipts1[i][1]][ipts1[i][0]];
//    }
//    rpts1_num = ipts1_num;
//
//    // �����˲�
//    blur_points(rpts0, rpts0_num, rpts0b, (int) round(line_blur_kernel));
//    rpts0b_num = rpts0_num;
//    blur_points(rpts1, rpts1_num, rpts1b, (int) round(line_blur_kernel));
//    rpts1b_num = rpts1_num;
//
//    // ���ߵȾ����
//    rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
//    resample_points(rpts0b, rpts0b_num, rpts0s, &rpts0s_num, sample_dist * pixel_per_meter);
//    rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
//    resample_points(rpts1b, rpts1b_num, rpts1s, &rpts1s_num, sample_dist * pixel_per_meter);
//
//    // ���߾ֲ��Ƕȱ仯��
//    local_angle_points(rpts0s, rpts0s_num, rpts0a, (int) round(angle_dist / sample_dist));
//    rpts0a_num = rpts0s_num;
//    local_angle_points(rpts1s, rpts1s_num, rpts1a, (int) round(angle_dist / sample_dist));
//    rpts1a_num = rpts1s_num;
//
//    // �Ƕȱ仯�ʷǼ�������
//    nms_angle(rpts0a, rpts0a_num, rpts0an, (int) round(angle_dist / sample_dist) * 2 + 1);
//    rpts0an_num = rpts0a_num;
//    nms_angle(rpts1a, rpts1a_num, rpts1an, (int) round(angle_dist / sample_dist) * 2 + 1);
//    rpts1an_num = rpts1a_num;
//
//    // �������߸���
//    track_leftline(rpts0s, rpts0s_num, rptsc0, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
//    rptsc0_num = rpts0s_num;
//    track_rightline(rpts1s, rpts1s_num, rptsc1, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
//    rptsc1_num = rpts1s_num;
//}
