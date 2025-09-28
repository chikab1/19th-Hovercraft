/*
 * cross.c
 *
 *  Created on: 2024��6��5��
 *      Author: admin
 */
#include "zf_common_headfile.h"
#include "math.h"

const unsigned char Array_forward_bird_col[22560];
const unsigned char Array_forward_bird_row[22560];

extern uint8 image[MT9V03X_H][MT9V03X_W];
extern uint8 IMG[MT9V03X_H][MT9V03X_W];
extern int rpts0[150][2],rpts1[150][2];//��¼��͸�Ӻ���߰��ߵ�x,y���� 0x,1y
extern uint8 rpts0_num,rpts1_num;//��¼��͸�Ӻ����ұ��ߵĸ���
extern float rpts0b[150][2],rpts1b[150][2];//��¼�˲�����߰��ߵ�x,y���� 0x,1y
extern uint8 rpts0b_num,rpts1b_num;//��¼�˲������ұ��ߵĸ���
extern float rpts0s[150][2],rpts1s[150][2];//��¼�Ⱦ��������߰��ߵ�x,y���� 0x,1y
extern uint8 rpts0s_num,rpts1s_num;//��¼�Ⱦ���������ұ��ߵĸ���
extern float rpts0a[150],rpts1a[150];//��¼���߾ֲ��Ƕȱ仯�ʺ���߰��ߵ�x,y���� 0x,1y
extern uint8 rpts0a_num,rpts1a_num;//��¼���߾ֲ��Ƕȱ仯�ʺ����ұ��ߵĸ���
extern float rpts0an[150],rpts1an[150];//��¼NMS����߰��ߵ�x,y���� 0x,1y
extern uint8 rpts0an_num,rpts1an_num;//��¼NMS�����ұ��ߵĸ���
extern float rptsc0[150][2],rptsc1[150][2];//��¼����׷�����ߺ����ߵ�x,y���� 0x,1y
extern uint8 rptsc0_num,rptsc1_num;//��¼����׷�����ߺ����ߵĸ���


extern float far_rpts0[150][2];
extern float far_rpts1[150][2];
extern uint8 far_rpts0_num, far_rpts1_num;

extern float far_rpts0b[150][2];
extern float far_rpts1b[150][2];
extern uint8 far_rpts0b_num, far_rpts1b_num;

extern float far_rpts0s[150][2];
extern float far_rpts1s[150][2];
extern uint8 far_rpts0s_num, far_rpts1s_num;

extern float far_rpts0a[150];
extern float far_rpts1a[150];
extern uint8 far_rpts0a_num, far_rpts1a_num;

extern float far_rpts0an[150];
extern float far_rpts1an[150];
extern uint8 far_rpts0an_num, far_rpts1an_num;



extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;// L�ǵ�λ��
extern bool Lpt0_found, Lpt1_found;// L�ǵ�
extern uint8 is_straight0, is_straight1;// ��ֱ��
//extern uint8 is_turn0, is_turn1;// ���

const int dir_front[4][2] = {{0,  -1},
                             {1,  0},
                             {0,  1},
                             {-1, 0}};
const int dir_frontleft[4][2] = {{-1, -1},
                                 {1,  -1},
                                 {1,  1},
                                 {-1, 1}};
const int dir_frontright[4][2] = {{1,  -1},
                                     {1,  1},
                                     {-1, 1},
                                     {-1, -1}};

extern uint8 far_x1, far_x2, far_y1, far_y2;



void clear_IMG(void)//���IMG���������
{
    for(int i=0;i<120;i++)
    {
        for(int j=0;j<188;j++)
        {
            IMG[i][j]=0;
            image[i][j]=0;
        }
    }
}

void get_image(void)//��mt9v03x_image����image���Ա㴦��
{
    for(int i=0;i<120;i++)
    {
        for(int j=0;j<188;j++)
        {
            image[i][j]=mt9v03x_image[i][j];
        }
    }
}

int8 limit(int8 x,int8 min,int8 max)//���ƺ���
{
    int8 x1;
    if(x>max)   x1=max;
    else if(x<min)  x1=min;
    else x1=x;
    return x1;
}

int MIN(int x,int min)
{
    int x1;
    if(x<min) x1=min;
    else x1=x;
    return x1;
}

int MAX(int x,int max)
{
    int x1;
    if(x>max) x1=max;
    else x1=x;
    return x1;
}

void find_leftline(void)//���ַ�ǽѲ�߷�
{
    uint8 x=93,y=115,flag=0,i=0,j=0,front_value=0,frontleft_value=0,step=0,turn=0;
    uint16 sum=0;
    int yuzhi=0,fx=0;
    while(x>3 && x<184 && y<116 && y>3 && turn<4 && step<150)
    {
        if(flag==0)//�ȴ��е�����Ѱ������㣬��Ϊ����
        {
            for(x=93;x>0;x--)
            {
                if((image[y][x]-image[y][x-1])>20)//�ҵ������
                {
                    sum=0,yuzhi=0;
                    for(i=x-3;i<=x+3;i++)//�����Ըõ�Ϊ���ĵ�7*7��Χ�ڼ�������Ӧ��ֵ
                    {
                        for(j=y-3;j<=y+3;j++)
                        {
                            sum+=image[j][i];
                        }
                    }
                    yuzhi=sum/49-3;//����ļ�ȥ����ֵ�������ݾ������(2-5)����
                    if(image[y][x-1]<yuzhi && image[y][x-2]<yuzhi && image[y][x-3]<yuzhi)//�ų����
                    {
                        fx=0;
                        flag=1;
                        break;
                    }
                }
            }
        }
        else{
            front_value=image[y+dir_front[fx][1]][x+dir_front[fx][0]];//�ŲȰ׵㣬ǰ����ͼ��ֵ
            frontleft_value=image[y+dir_frontleft[fx][1]][x+dir_frontleft[fx][0]];//�ŲȰ׵㣬��ǰ����ͼ��ֵ
            sum=0,yuzhi=0;
            for(i=x-3;i<=x+3;i++)
            {
                for(j=y-3;j<=y+3;j++)
                {
                    sum+=image[j][i];
                }
            }
            yuzhi=sum/49-3;
            if(front_value<yuzhi)//ǰ�� ��ת
            {
                fx=(fx+1)%4;
                turn++;
            }
            else if(frontleft_value<yuzhi)//ǰ�� ��ǰ�� ǰ��
            {
                x += dir_front[fx][0];  //��¼���߰׵��λ��
                y += dir_front[fx][1];
                rpts0[step][0] = x;
                rpts0[step][1] = y;
                step++;
                turn = 0;
            }
            else//ǰ�� ��ǰ�� ��ת
            {
                x += dir_frontleft[fx][0];
                y += dir_frontleft[fx][1];
                fx = (fx + 3) % 4;
                rpts0[step][0] = x;
                rpts0[step][1] = y;
                step++;
                turn = 0;
            }
        }
    }
    rpts0_num=step;
}

void find_farleftline(void)//���ַ�ǽѲԶ�߷�
{
    uint8 x=93,y=115,i=0,j=0,front_value=0,frontleft_value=0,step=0,turn=0;
    uint16 sum=0;
    int yuzhi=0,fx=0;

    x=far_x1,y=far_y1+1;
    while(x>3 && x<184 && y<116 && y>3 && turn<4 && step<150)
    {
        front_value=image[y+dir_front[fx][1]][x+dir_front[fx][0]];//�ŲȰ׵㣬ǰ����ͼ��ֵ
        frontleft_value=image[y+dir_frontleft[fx][1]][x+dir_frontleft[fx][0]];//�ŲȰ׵㣬��ǰ����ͼ��ֵ
        sum=0,yuzhi=0;
        for(i=x-3;i<=x+3;i++)
        {
            for(j=y-3;j<=y+3;j++)
            {
                sum+=image[j][i];
            }
        }
        yuzhi=sum/49-3;
        if(front_value<yuzhi)//ǰ�� ��ת
        {
            fx=(fx+1)%4;
            turn++;
        }
        else if(frontleft_value<yuzhi)//ǰ�� ��ǰ�� ǰ��
        {
            x += dir_front[fx][0];  //��¼���߰׵��λ��
            y += dir_front[fx][1];
            far_rpts0[step][0] = x;
            far_rpts0[step][1] = y;
            step++;
            turn = 0;
        }
        else//ǰ�� ��ǰ�� ��ת
        {
            x += dir_frontleft[fx][0];
            y += dir_frontleft[fx][1];
            fx = (fx + 3) % 4;
            far_rpts0[step][0] = x;
            far_rpts0[step][1] = y;
            step++;
            turn = 0;
        }
    }
    far_rpts0_num=step;
}

void find_rightline(void)//���ַ�ǽѲ�߷�
{
    uint8 x=95,y=115,flag=0,i=0,j=0,front_value=0,frontright_value=0,step=0,turn=0;
    uint16 sum=0;
    int yuzhi=0,fx=0;
    while(x>3 && x<184 && y<116 && y>3 && turn<4 && step<150)
    {
        if(flag==0)//�ȴ��е�����Ѱ������㣬��Ϊ����
        {
            for(x=95;x<188;x++)
            {
                if((image[y][x]-image[y][x+1])>20)//�ҵ������
                {
                    sum=0,yuzhi=0;
                    for(i=x-3;i<=x+3;i++)//�����Ըõ�Ϊ���ĵ�7*7��Χ�ڼ�������Ӧ��ֵ
                    {
                        for(j=y-3;j<=y+3;j++)
                        {
                            sum+=image[j][i];
                        }
                    }
                    yuzhi=sum/49-3;//����ļ�ȥ����ֵ�������ݾ������(2-5)����
                    if(image[y][x+1]<yuzhi && image[y][x+2]<yuzhi && image[y][x+3]<yuzhi)//�ų����
                    {
                        fx=0;
                        flag=1;
                        break;
                    }
                }
            }
        }
        else
        {
            front_value=image[y+dir_front[fx][1]][x+dir_front[fx][0]];//�ŲȰ׵㣬ǰ����ͼ��ֵ
            frontright_value=image[y+dir_frontright[fx][1]][x+dir_frontright[fx][0]];//�ŲȰ׵㣬��ǰ����ͼ��ֵ
            sum=0,yuzhi=0;
            for(i=x-3;i<=x+3;i++)
            {
                for(j=y-3;j<=y+3;j++)
                {
                    sum+=image[j][i];
                }
            }
            yuzhi=sum/49-3;
            if(front_value < yuzhi)//ǰ�� ��ת
            {
                fx=(fx+3)%4;
                turn++;
            }
            else if(frontright_value<yuzhi)//ǰ�� ��ǰ�� ǰ��
            {
                x += dir_front[fx][0];
                y += dir_front[fx][1];
                rpts1[step][0]=x;
                rpts1[step][1]=y;
                step++;
                turn=0;
            }
            else //ǰ�� ��ǰ�� ��ת
            {
                x += dir_frontright[fx][0];
                y += dir_frontright[fx][1];
                fx = (fx + 1) % 4;
                rpts1[step][0] = x;
                rpts1[step][1] = y;
                step++;
                turn = 0;
            }
        }
    }
    rpts1_num=step;
}

void find_farrightline(void)//���ַ�ǽѲԶ�߷�
{
    uint8 x=95,y=115,i=0,j=0,front_value=0,frontright_value=0,step=0,turn=0;
    uint16 sum=0;
    int yuzhi=0,fx=0;

    x=far_x2,y=far_y2+1;
    while(x>3 && x<184 && y<116 && y>3 && turn<4 && step<150)
    {
        front_value=image[y+dir_front[fx][1]][x+dir_front[fx][0]];//�ŲȰ׵㣬ǰ����ͼ��ֵ
        frontright_value=image[y+dir_frontright[fx][1]][x+dir_frontright[fx][0]];//�ŲȰ׵㣬��ǰ����ͼ��ֵ
        sum=0,yuzhi=0;
        for(i=x-3;i<=x+3;i++)
        {
            for(j=y-3;j<=y+3;j++)
            {
                sum+=image[j][i];
            }
        }
        yuzhi=sum/49-3;
        if(front_value < yuzhi)//ǰ�� ��ת
        {
            fx=(fx+3)%4;
            turn++;
        }
        else if(frontright_value<yuzhi)//ǰ�� ��ǰ�� ǰ��
        {
            x += dir_front[fx][0];
            y += dir_front[fx][1];
            far_rpts1[step][0]=x;
            far_rpts1[step][1]=y;
            step++;
            turn=0;
        }
        else //ǰ�� ��ǰ�� ��ת
        {
            x += dir_frontright[fx][0];
            y += dir_frontright[fx][1];
            fx = (fx + 1) % 4;
            far_rpts1[step][0] = x;
            far_rpts1[step][1] = y;
            step++;
            turn = 0;
        }
    }
    far_rpts1_num=step;
}


void image_Perspective_Correction(void)//��͸�Ӻ��� ֱ�ӽ��д��任  ȥ��bվ__�ո���û�е�___����͸����Ƶ
{
    for(int j=0;j<120;j++)
    {
        for(int i=0;i<188;i++)//j=0 i=1 row[1] col[1]
        {
            image[j][i]=mt9v03x_image[Array_forward_bird_row[j*188+i]][Array_forward_bird_col[j*188+i]];
        }
    }
}


void blur_points(void)//���������˲�
{
    int half = 3 ;//ȡhalf*2+1��������˲�
    uint16 sum0=0,sum1=0;
    for (uint8 i = 0; i < rpts0_num; i++)//����͸�Ӻ�ı��߽��б���
    {
        sum0=sum1=0;
        for (int8 j = -half; j <= half; j++) //�Ըõ���ǰ����half����
        {
            sum0 += rpts0[limit(i + j, 0, rpts0_num - 1)][0] * (half + 1 - abs(j));
            sum1 += rpts0[limit(i + j, 0, rpts0_num - 1)][1] * (half + 1 - abs(j));
        }
        rpts0b[i][0]  =  sum0 / ((2 * half + 2) * (half + 1) / 2);
        rpts0b[i][1]  =  sum1 / ((2 * half + 2) * (half + 1) / 2);
    }
    rpts0b_num=rpts0_num;

    sum0=sum1=0;
    for (uint8 i = 0; i < rpts1_num; i++)
    {
        sum0=sum1=0;
        for (int j = -half; j <= half; j++)
        {
            sum0 += rpts1[limit(i + j, 0, rpts1_num - 1)][0] * (half + 1 - abs(j));
            sum1 += rpts1[limit(i + j, 0, rpts1_num - 1)][1] * (half + 1 - abs(j));
        }
        rpts1b[i][0]  = sum0 / ((2 * half + 2) * (half + 1) / 2);
        rpts1b[i][1]  = sum1 / ((2 * half + 2) * (half + 1) / 2);
    }
    rpts1b_num=rpts1_num;
}

void far_blur_points(void)//���������˲�
{
    int half = 3 ;//ȡhalf*2+1��������˲�
    for (int i = 0; i < far_rpts0_num; i++)//����͸�Ӻ�ı��߽��б���
    {
        far_rpts0b[i][0] = far_rpts0b[i][1] = 0;
        for (int j = -half; j <= half; j++) //�Ըõ���ǰ����half����
        {
            far_rpts0b[i][0] += far_rpts0[limit(i + j, 0, far_rpts0_num - 1)][0] * (half + 1 - abs(j));
            far_rpts0b[i][1] += far_rpts0[limit(i + j, 0, far_rpts0_num - 1)][1] * (half + 1 - abs(j));
        }
        far_rpts0b[i][0] /= (2 * half + 2) * (half + 1) / 2;
        far_rpts0b[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
    far_rpts0b_num=far_rpts0_num;

    for (int i = 0; i < far_rpts1_num; i++)
    {
        far_rpts1b[i][0] = far_rpts1b[i][1] = 0;
        for (int j = -half; j <= half; j++) {
            far_rpts1b[i][0] += far_rpts1[limit(i + j, 0, far_rpts1_num - 1)][0] * (half + 1 - abs(j));
            far_rpts1b[i][1] += far_rpts1[limit(i + j, 0, far_rpts1_num - 1)][1] * (half + 1 - abs(j));
        }
        far_rpts1b[i][0] /= (2 * half + 2) * (half + 1) / 2;
        far_rpts1b[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
    far_rpts1b_num=far_rpts1_num;
}

void resample_points(void)//�Ⱦ���� �������Բ�ֵ�ķ��� ������csdn������
{
//    float dist=1.5;//�Ⱦ������ļ��
////    rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
//    rpts0s[0][0] = rpts0b[0][0];
//    rpts0s[0][1] = rpts0b[0][1];
//    int len = 1;
//    for (int i = 0; i < rpts0b_num - 1; i++)
//    {
//        float x0 = rpts0b[i][0];
//        float y0 = rpts0b[i][1];
//        float x1 = rpts0b[i + 1][0];
//        float y1 = rpts0b[i + 1][1];
//
//        do {
//            float x = rpts0s[len - 1][0];
//            float y = rpts0s[len - 1][1];
//
//            float dx0 = x0 - x;
//            float dy0 = y0 - y;
//            float dx1 = x1 - x;
//            float dy1 = y1 - y;
//
//            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
//            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
//
//            float r0 = (dist1 - dist) / (dist1 - dist0);//Ӧ�����һ��С��
//            float r1 = 1 - r0;
//
//            if (r0 < 0 || r1 < 0) break;
//            x0 = x0 * r0 + x1 * r1;
//            y0 = y0 * r0 + y1 * r1;
//            rpts0s[len][0] = x0;
//            rpts0s[len][1] = y0;
//            len++;
//        } while (len < rpts0b_num);
//
//    }
//    rpts0s_num = len;
//
//    rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
//    rpts1s[0][0] = rpts1b[0][0];
//    rpts1s[0][1] = rpts1b[0][1];
//    len = 1;
//    for (int i = 0; i < rpts1b_num - 1 && len < rpts1s_num; i++) {
//        float x0 = rpts1b[i][0];
//        float y0 = rpts1b[i][1];
//        float x1 = rpts1b[i + 1][0];
//        float y1 = rpts1b[i + 1][1];
//
//        do {
//            float x = rpts1s[len - 1][0];
//            float y = rpts1s[len - 1][1];
//
//            float dx0 = x0 - x;
//            float dy0 = y0 - y;
//            float dx1 = x1 - x;
//            float dy1 = y1 - y;
//
//            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
//            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
//
//            float r0 = (dist1 - dist) / (dist1 - dist0);
//            float r1 = 1 - r0;
//
//            if (r0 < 0 || r1 < 0) break;
//            x0 = x0 * r0 + x1 * r1;
//            y0 = y0 * r0 + y1 * r1;
//            rpts1s[len][0] = x0;
//            rpts1s[len][1] = y0;
//            len++;
//        } while (len < rpts1s_num);
//
//    }
//    rpts1s_num = len;


    float dist=2.08;//�Ⱦ������ļ��
//    rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
    rpts0s[0][0] = rpts0[0][0];
    rpts0s[0][1] = rpts0[0][1];
    int len = 1;
    for (int i = 0; i < rpts0_num - 1; i++)
    {
        float x0 = rpts0[i][0];
        float y0 = rpts0[i][1];
        float x1 = rpts0[i + 1][0];
        float y1 = rpts0[i + 1][1];

        do {
            float x = rpts0s[len - 1][0];
            float y = rpts0s[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);//Ӧ�����һ��С��
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            rpts0s[len][0] = x0;
            rpts0s[len][1] = y0;
            len++;
        } while (len < rpts0_num);

    }
    rpts0s_num = len;

//    rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
    rpts1s[0][0] = rpts1[0][0];
    rpts1s[0][1] = rpts1[0][1];
    len = 1;
    for (int i = 0; i < rpts1_num - 1; i++) {
        float x0 = rpts1[i][0];
        float y0 = rpts1[i][1];
        float x1 = rpts1[i + 1][0];
        float y1 = rpts1[i + 1][1];

        do {
            float x = rpts1s[len - 1][0];
            float y = rpts1s[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            rpts1s[len][0] = x0;
            rpts1s[len][1] = y0;
            len++;
        } while (len < rpts1_num);

    }
    rpts1s_num = len;
}

void far_resample_points(void)//�Ⱦ���� �������Բ�ֵ�ķ��� ������csdn������
{
    float dist=2.08;//�Ⱦ������ļ��
    far_rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
    far_rpts0s[0][0] = far_rpts0b[0][0];
    far_rpts0s[0][1] = far_rpts0b[0][1];
    int len = 1;
    for (int i = 0; i < far_rpts0b_num - 1 && len < far_rpts0s_num; i++) {
        float x0 = far_rpts0b[i][0];
        float y0 = far_rpts0b[i][1];
        float x1 = far_rpts0b[i + 1][0];
        float y1 = far_rpts0b[i + 1][1];

        do {
            float x = far_rpts0s[len - 1][0];
            float y = far_rpts0s[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);//Ӧ�����һ��С��
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            far_rpts0s[len][0] = x0;
            far_rpts0s[len][1] = y0;
            len++;
        } while (len < far_rpts0s_num);

    }
    far_rpts0s_num = len;

    far_rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
    far_rpts1s[0][0] = far_rpts1b[0][0];
    far_rpts1s[0][1] = far_rpts1b[0][1];
    len = 1;
    for (int i = 0; i < far_rpts1b_num - 1 && len < far_rpts1s_num; i++) {
        float x0 = far_rpts1b[i][0];
        float y0 = far_rpts1b[i][1];
        float x1 = far_rpts1b[i + 1][0];
        float y1 = far_rpts1b[i + 1][1];

        do {
            float x = far_rpts1s[len - 1][0];
            float y = far_rpts1s[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            far_rpts1s[len][0] = x0;
            far_rpts1s[len][1] = y0;
            len++;
        } while (len < far_rpts1s_num);

    }
    far_rpts1s_num = len;
}

void local_angle_points(void)//���߾ֲ��Ƕȱ仯��
{
    const int dist=round(5);//������ļ��Ϊdist
    for (int i = 0; i < rpts0s_num; i++)
    {
        if (i <= 0 || i >= rpts0s_num - 1)
        {
            rpts0a[i] = 0;
            continue;
        }
        float dx1 = rpts0s[i][0] - rpts0s[limit(i - dist, 0, rpts0s_num - 1)][0];
        float dy1 = rpts0s[i][1] - rpts0s[limit(i - dist, 0, rpts0s_num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = rpts0s[limit(i + dist, 0, rpts0s_num - 1)][0] - rpts0s[i][0];
        float dy2 = rpts0s[limit(i + dist, 0, rpts0s_num - 1)][1] - rpts0s[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        rpts0a[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
    rpts0a_num = rpts0s_num;


    for (int i = 0; i < rpts1s_num; i++)
    {
        if (i <= 0 || i >= rpts1s_num - 1)
        {
            rpts1a[i] = 0;
            continue;
        }
        float dx1 = rpts1s[i][0] - rpts1s[limit(i - dist, 0, rpts1s_num - 1)][0];
        float dy1 = rpts1s[i][1] - rpts1s[limit(i - dist, 0, rpts1s_num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = rpts1s[limit(i + dist, 0, rpts1s_num - 1)][0] - rpts1s[i][0];
        float dy2 = rpts1s[limit(i + dist, 0, rpts1s_num - 1)][1] - rpts1s[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        rpts1a[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
    rpts1a_num = rpts1s_num;

}

void far_local_angle_points(void)//���߾ֲ��Ƕȱ仯��
{
    const int dist=round(5);//������ļ��Ϊdist
    for (int i = 0; i < far_rpts0s_num; i++)
    {
        if (i <= 0 || i >= far_rpts0s_num - 1)
        {
            far_rpts0a[i] = 0;
            continue;
        }
        float dx1 = far_rpts0s[i][0] - far_rpts0s[limit(i - dist, 0, far_rpts0s_num - 1)][0];
        float dy1 = far_rpts0s[i][1] - far_rpts0s[limit(i - dist, 0, far_rpts0s_num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = far_rpts0s[limit(i + dist, 0, far_rpts0s_num - 1)][0] - far_rpts0s[i][0];
        float dy2 = far_rpts0s[limit(i + dist, 0, far_rpts0s_num - 1)][1] - far_rpts0s[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        far_rpts0a[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
    far_rpts0a_num = far_rpts0s_num;


    for (int i = 0; i < far_rpts1s_num; i++)
    {
        if (i <= 0 || i >= far_rpts1s_num - 1)
        {
            far_rpts1a[i] = 0;
            continue;
        }
        float dx1 = far_rpts1s[i][0] - far_rpts1s[limit(i - dist, 0, far_rpts1s_num - 1)][0];
        float dy1 = far_rpts1s[i][1] - far_rpts1s[limit(i - dist, 0, far_rpts1s_num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = far_rpts1s[limit(i + dist, 0, far_rpts1s_num - 1)][0] - far_rpts1s[i][0];
        float dy2 = far_rpts1s[limit(i + dist, 0, far_rpts1s_num - 1)][1] - far_rpts1s[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        far_rpts1a[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
    far_rpts1a_num = far_rpts1s_num;
}

void nms_angle(void)//ȡ�ֲ����ֵ������ ������csdn����nms�Ǽ���ֵ����
{
    const int half = 7;//��half*2+1�ķ�Χ���ҵ�һ�����ֵ
    for (int i = 0; i < rpts0a_num; i++)
    {
        rpts0an[i] = rpts0a[i];
        for (int j = -half; j <= half; j++)     //��-half��half�ķ�Χ�������ֵ���������0
        {
            if (fabs(rpts0a[limit(i + j, 0, rpts0a_num - 1)]) > fabs(rpts0an[i]))
            {
                rpts0an[i] = 0;
                break;
            }
        }
    }
    rpts0an_num = rpts0a_num;


    for (int i = 0; i < rpts1a_num; i++)
    {
        rpts1an[i] = rpts1a[i];
        for (int j = -half; j <= half; j++)
        {
            if (fabs(rpts1a[limit(i + j, 0, rpts1a_num - 1)]) > fabs(rpts1an[i]))
            {
                rpts1an[i] = 0;
                break;
            }
        }
    }
    rpts1an_num = rpts1a_num;
}

void far_nms_angle(void)//ȡ�ֲ����ֵ������ ������csdn����nms�Ǽ���ֵ����
{
    const int half = 7;//��half*2+1�ķ�Χ���ҵ�һ�����ֵ
    for (int i = 0; i < far_rpts0a_num; i++)
    {
        far_rpts0an[i] = far_rpts0a[i];
        for (int j = -half; j <= half; j++)     //��-half��half�ķ�Χ�������ֵ���������0
        {
            if (fabs(far_rpts0a[limit(i + j, 0, far_rpts0a_num - 1)]) > fabs(far_rpts0an[i]))
            {
                far_rpts0an[i] = 0;
                break;
            }
        }
    }
    far_rpts0an_num = far_rpts0a_num;


    for (int i = 0; i < far_rpts1a_num; i++)
    {
        far_rpts1an[i] = far_rpts1a[i];
        for (int j = -half; j <= half; j++)
        {
            if (fabs(far_rpts1a[limit(i + j, 0, far_rpts1a_num - 1)]) > fabs(far_rpts1an[i]))
            {
                far_rpts1an[i] = 0;
                break;
            }
        }
    }
    far_rpts1an_num = far_rpts1a_num;
}

void track_leftline(void)//ѡ�������㣬���ߵķ��߷���ƽ�ƹ̶���ֵ�õ����� ƽ��dist��С
{
    int approx_num=round(5);
    float dist=90*0.45/2;
    for (int i = 0; i < rpts0s_num; i++)
    {
        float dx = rpts0s[limit(i + approx_num, 0, rpts0s_num - 1)][0] - rpts0s[limit(i - approx_num, 0, rpts0s_num - 1)][0];
        float dy = rpts0s[limit(i + approx_num, 0, rpts0s_num - 1)][1] - rpts0s[limit(i - approx_num, 0, rpts0s_num - 1)][1];
        if(dx==0 && dy==0)
        {

        }
        else
        {
            float dn = sqrt(dx * dx + dy * dy);
            dx /= dn;//cos
            dy /= dn;//sin
            rptsc0[i][0] = rpts0s[i][0] - dy * dist + 8;
            rptsc0[i][1] = rpts0s[i][1] + dx * dist - 8;
        }
    }
}

void track_rightline(void)//ѡ�������㣬���ߵķ��߷���ƽ�ƹ̶���ֵ�õ����� ƽ��dist��С
{
    int approx_num=round(5);
    float dist=90*0.45/2;
    for (int i = 0; i < rpts1s_num; i++)
    {
        float dx = rpts1s[limit(i + approx_num, 0, rpts1s_num - 1)][0] - rpts1s[limit(i - approx_num, 0, rpts1s_num - 1)][0];
        float dy = rpts1s[limit(i + approx_num, 0, rpts1s_num - 1)][1] - rpts1s[limit(i - approx_num, 0, rpts1s_num - 1)][1];
        if(dx==0 && dy==0)
        {

        }
        else
        {
            float dn = sqrt(dx * dx + dy * dy);
            dx /= dn;//cos
            dy /= dn;//sin
            rptsc1[i][0] = rpts1s[i][0] + dy * dist - 8;
            rptsc1[i][1] = rpts1s[i][1] - dx * dist + 8;
        }
    }
}


void find_corners(void)
{
    // ʶ��L�յ�
    Lpt0_found = Lpt1_found = false;
    if(rpts0s_num > 50) is_straight0=1;//����Ҫ��ǰ50������㳤ֱ��
    else is_straight0=0;
    if(rpts1s_num > 50) is_straight1=1;
    else is_straight1=0;

    for (int i = 0; i < rpts0s_num; i++)
    {
        if (rpts0an[i] == 0) continue;
        int im1 = limit(i - 10, 0, rpts0s_num - 1);
        int ip1 = limit(i + 10, 0, rpts0s_num - 1);
        float conf = fabs(rpts0a[i]) - (fabs(rpts0a[im1]) + fabs(rpts0a[ip1])) / 2;//��ǰ���Ѱ��������������conf

        //L�ǵ���ֵ
        if (Lpt0_found == false && 50. / 180. * PI < conf && conf < 160. / 180. * PI && i < 40)//60-150�� ������������ýǶ�  i<40Ϊ��Ҫ�������ǰ40����Ż�ȥʶ��
        {
            Lpt0_rpts0s_id = i;//��¼�յ�������ĵڼ�λ
            Lpt0_found = true;//�Ѿ��ҵ��յ�
        }
        //��ֱ����ֵ
        if (conf > 5. / 180. * PI && i < 50) is_straight0 = 0; //��ǰ50�㲢�ҽǶȴ���5���ǾͲ��ǳ�ֱ����
        if (Lpt0_found == true && is_straight0 == 0) break;
    }
    for (int i = 0; i < rpts1s_num; i++) {
        if (rpts1an[i] == 0) continue;
        int im1 = limit(i - (int) round(0.2 / 0.02), 0, rpts1s_num - 1);
        int ip1 = limit(i + (int) round(0.2 / 0.02), 0, rpts1s_num - 1);
        float conf = fabs(rpts1a[i]) - (fabs(rpts1a[im1]) + fabs(rpts1a[ip1])) / 2;

        //L�ǵ���ֵ
        if (Lpt1_found == false && 50. / 180. * PI < conf && conf < 160. / 180. * PI && i < 40)//60-150�� ������������ýǶ�  i<40Ϊ��Ҫ�������ǰ40����Ż�ȥʶ��
        {
            Lpt1_rpts1s_id = i;
            Lpt1_found = 1;
        }
        //��ֱ����ֵ
        if (conf > 5. / 180. * PI && i < 1.0 / 0.02) is_straight1 = 0;
        if (Lpt1_found == 1 && is_straight1 == 0) break;
    }
}






const unsigned char Array_forward_bird_col[22560]=
{
58,59,59,59,60,60,61,61,61,62,62,62,63,63,64,64,64,65,65,66,66,66,67,67,68,68,68,69,69,70,70,70,71,71,71,72,72,73,73,73,74,74,75,75,75,76,76,77,77,77,78,78,79,79,79,80,80,80,81,81,82,82,82,83,83,84,84,84,85,85,86,86,86,87,87,88,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,98,99,99,100,100,100,101,101,102,102,102,103,103,104,104,104,105,105,106,106,106,107,107,107,108,108,109,109,109,110,110,111,111,111,112,112,113,113,113,114,114,115,115,115,116,116,116,117,117,118,118,118,119,119,120,120,120,121,121,122,122,122,123,123,124,124,124,125,125,125,126,126,127,127,127,128,128,129,129,129,130,130,131,131,131,
58,58,59,59,60,60,60,61,61,61,62,62,63,63,63,64,64,65,65,65,66,66,67,67,67,68,68,69,69,69,70,70,71,71,71,72,72,73,73,73,74,74,74,75,75,76,76,76,77,77,78,78,78,79,79,80,80,80,81,81,82,82,82,83,83,84,84,84,85,85,86,86,86,87,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,99,100,100,100,101,101,102,102,102,103,103,104,104,104,105,105,106,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,112,113,113,113,114,114,115,115,115,116,116,117,117,117,118,118,119,119,119,120,120,121,121,121,122,122,123,123,123,124,124,125,125,125,126,126,126,127,127,128,128,128,129,129,130,130,130,131,131,132,
58,58,59,59,59,60,60,60,61,61,62,62,62,63,63,64,64,64,65,65,66,66,66,67,67,68,68,68,69,69,70,70,70,71,71,72,72,72,73,73,74,74,74,75,75,76,76,76,77,77,78,78,78,79,79,80,80,80,81,81,82,82,82,83,83,83,84,84,85,85,85,86,86,87,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,99,100,100,101,101,101,102,102,103,103,103,104,104,104,105,105,106,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,112,113,113,114,114,114,115,115,116,116,116,117,117,118,118,118,119,119,120,120,120,121,121,122,122,122,123,123,124,124,124,125,125,126,126,126,127,127,127,128,128,129,129,129,130,130,131,131,131,132,
57,58,58,59,59,59,60,60,61,61,61,62,62,63,63,63,64,64,65,65,65,66,66,67,67,67,68,68,69,69,69,70,70,71,71,71,72,72,73,73,73,74,74,75,75,75,76,76,77,77,77,78,78,79,79,79,80,80,81,81,81,82,82,83,83,83,84,84,85,85,85,86,86,87,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,99,100,100,101,101,101,102,102,103,103,103,104,104,105,105,105,106,106,107,107,107,108,108,109,109,109,110,110,111,111,111,112,112,113,113,113,114,114,115,115,115,116,116,117,117,117,118,118,119,119,119,120,120,121,121,121,122,122,123,123,123,124,124,125,125,125,126,126,127,127,127,128,128,129,129,129,130,130,131,131,131,132,132,
57,58,58,58,59,59,60,60,60,61,61,62,62,62,63,63,64,64,64,65,65,66,66,66,67,67,68,68,68,69,69,70,70,70,71,71,72,72,73,73,73,74,74,75,75,75,76,76,77,77,77,78,78,79,79,79,80,80,81,81,81,82,82,83,83,83,84,84,85,85,85,86,86,87,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,99,100,100,101,101,101,102,102,103,103,103,104,104,105,105,105,106,106,107,107,107,108,108,109,109,109,110,110,111,111,111,112,112,113,113,113,114,114,115,115,116,116,116,117,117,118,118,118,119,119,120,120,120,121,121,122,122,122,123,123,124,124,124,125,125,126,126,126,127,127,128,128,128,129,129,130,130,130,131,131,132,132,132,
57,57,58,58,59,59,59,60,60,61,61,61,62,62,63,63,63,64,64,65,65,65,66,66,67,67,68,68,68,69,69,70,70,70,71,71,72,72,72,73,73,74,74,74,75,75,76,76,76,77,77,78,78,78,79,79,80,80,80,81,81,82,82,82,83,83,84,84,85,85,85,86,86,87,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,99,100,100,101,101,101,102,102,103,103,104,104,104,105,105,106,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,112,113,113,114,114,114,115,115,116,116,116,117,117,118,118,118,119,119,120,120,121,121,121,122,122,123,123,123,124,124,125,125,125,126,126,127,127,127,128,128,129,129,129,130,130,131,131,131,132,132,133,
57,57,58,58,58,59,59,60,60,60,61,61,62,62,62,63,63,64,64,64,65,65,66,66,67,67,67,68,68,69,69,69,70,70,71,71,71,72,72,73,73,73,74,74,75,75,75,76,76,77,77,78,78,78,79,79,80,80,80,81,81,82,82,82,83,83,84,84,84,85,85,86,86,86,87,87,88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,97,98,98,99,99,100,100,100,101,101,102,102,102,103,103,104,104,104,105,105,106,106,106,107,107,108,108,108,109,109,110,110,111,111,111,112,112,113,113,113,114,114,115,115,115,116,116,117,117,117,118,118,119,119,119,120,120,121,121,122,122,122,123,123,124,124,124,125,125,126,126,126,127,127,128,128,128,129,129,130,130,130,131,131,132,132,133,133,
56,57,57,58,58,59,59,59,60,60,61,61,61,62,62,63,63,63,64,64,65,65,66,66,66,67,67,68,68,68,69,69,70,70,70,71,71,72,72,72,73,73,74,74,75,75,75,76,76,77,77,77,78,78,79,79,79,80,80,81,81,82,82,82,83,83,84,84,84,85,85,86,86,86,87,87,88,88,88,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,98,98,98,99,99,100,100,100,101,101,102,102,102,103,103,104,104,104,105,105,106,106,107,107,107,108,108,109,109,109,110,110,111,111,111,112,112,113,113,114,114,114,115,115,116,116,116,117,117,118,118,118,119,119,120,120,120,121,121,122,122,123,123,123,124,124,125,125,125,126,126,127,127,127,128,128,129,129,130,130,130,131,131,132,132,132,133,133,
56,57,57,57,58,58,59,59,60,60,60,61,61,62,62,62,63,63,64,64,64,65,65,66,66,67,67,67,68,68,69,69,69,70,70,71,71,72,72,72,73,73,74,74,74,75,75,76,76,76,77,77,78,78,79,79,79,80,80,81,81,81,82,82,83,83,83,84,84,85,85,86,86,86,87,87,88,88,88,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,98,98,98,99,99,100,100,100,101,101,102,102,103,103,103,104,104,105,105,105,106,106,107,107,107,108,108,109,109,110,110,110,111,111,112,112,112,113,113,114,114,114,115,115,116,116,117,117,117,118,118,119,119,119,120,120,121,121,122,122,122,123,123,124,124,124,125,125,126,126,126,127,127,128,128,129,129,129,130,130,131,131,131,132,132,133,133,133,
56,56,57,57,58,58,58,59,59,60,60,61,61,61,62,62,63,63,63,64,64,65,65,66,66,66,67,67,68,68,68,69,69,70,70,71,71,71,72,72,73,73,73,74,74,75,75,76,76,76,77,77,78,78,78,79,79,80,80,81,81,81,82,82,83,83,83,84,84,85,85,86,86,86,87,87,88,88,88,89,89,90,90,91,91,91,92,92,93,93,93,94,94,95,95,95,96,96,97,97,98,98,98,99,99,100,100,100,101,101,102,102,103,103,103,104,104,105,105,105,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,113,113,113,114,114,115,115,115,116,116,117,117,118,118,118,119,119,120,120,120,121,121,122,122,123,123,123,124,124,125,125,125,126,126,127,127,128,128,128,129,129,130,130,130,131,131,132,132,133,133,133,134,
56,56,57,57,57,58,58,59,59,59,60,60,61,61,62,62,62,63,63,64,64,65,65,65,66,66,67,67,67,68,68,69,69,70,70,70,71,71,72,72,72,73,73,74,74,75,75,75,76,76,77,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,83,84,84,85,85,85,86,86,87,87,88,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,98,99,99,100,100,101,101,101,102,102,103,103,103,104,104,105,105,106,106,106,107,107,108,108,109,109,109,110,110,111,111,111,112,112,113,113,114,114,114,115,115,116,116,116,117,117,118,118,119,119,119,120,120,121,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,127,128,128,129,129,129,130,130,131,131,132,132,132,133,133,134,134,
55,56,56,57,57,58,58,58,59,59,60,60,61,61,61,62,62,63,63,63,64,64,65,65,66,66,66,67,67,68,68,69,69,69,70,70,71,71,71,72,72,73,73,74,74,74,75,75,76,76,77,77,77,78,78,79,79,79,80,80,81,81,82,82,82,83,83,84,84,85,85,85,86,86,87,87,88,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,98,99,99,100,100,101,101,101,102,102,103,103,104,104,104,105,105,106,106,107,107,107,108,108,109,109,109,110,110,111,111,112,112,112,113,113,114,114,115,115,115,116,116,117,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,123,124,124,125,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,131,132,132,133,133,134,134,134,
55,56,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,62,63,63,64,64,65,65,65,66,66,67,67,67,68,68,69,69,70,70,70,71,71,72,72,73,73,73,74,74,75,75,76,76,76,77,77,78,78,79,79,79,80,80,81,81,82,82,82,83,83,84,84,84,85,85,86,86,87,87,87,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,99,99,99,100,100,101,101,102,102,102,103,103,104,104,104,105,105,106,106,107,107,107,108,108,109,109,110,110,110,111,111,112,112,113,113,113,114,114,115,115,116,116,116,117,117,118,118,119,119,119,120,120,121,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,127,128,128,129,129,130,130,130,131,131,132,132,133,133,133,134,134,135,
55,55,56,56,57,57,57,58,58,59,59,60,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,66,67,67,68,68,69,69,69,70,70,71,71,72,72,72,73,73,74,74,75,75,75,76,76,77,77,78,78,78,79,79,80,80,81,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,87,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,99,99,99,100,100,101,101,102,102,102,103,103,104,104,105,105,105,106,106,107,107,108,108,108,109,109,110,110,111,111,111,112,112,113,113,114,114,114,115,115,116,116,117,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,126,127,127,128,128,129,129,129,130,130,131,131,132,132,132,133,133,134,134,135,135,
55,55,55,56,56,57,57,58,58,58,59,59,60,60,61,61,62,62,62,63,63,64,64,65,65,65,66,66,67,67,68,68,68,69,69,70,70,71,71,71,72,72,73,73,74,74,74,75,75,76,76,77,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,87,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,99,99,99,100,100,101,101,102,102,102,103,103,104,104,105,105,106,106,106,107,107,108,108,109,109,109,110,110,111,111,112,112,112,113,113,114,114,115,115,115,116,116,117,117,118,118,118,119,119,120,120,121,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,131,132,132,133,133,134,134,134,135,135,
54,55,55,56,56,56,57,57,58,58,59,59,60,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,66,67,67,68,68,69,69,70,70,70,71,71,72,72,73,73,73,74,74,75,75,76,76,76,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,83,84,84,85,85,86,86,86,87,87,88,88,89,89,90,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,96,97,97,98,98,99,99,100,100,100,101,101,102,102,103,103,103,104,104,105,105,106,106,106,107,107,108,108,109,109,110,110,110,111,111,112,112,113,113,113,114,114,115,115,116,116,116,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,126,127,127,128,128,129,129,130,130,130,131,131,132,132,133,133,133,134,134,135,135,136,
54,54,55,55,56,56,57,57,58,58,58,59,59,60,60,61,61,61,62,62,63,63,64,64,65,65,65,66,66,67,67,68,68,68,69,69,70,70,71,71,72,72,72,73,73,74,74,75,75,75,76,76,77,77,78,78,79,79,79,80,80,81,81,82,82,82,83,83,84,84,85,85,86,86,86,87,87,88,88,89,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,97,98,98,99,99,100,100,100,101,101,102,102,103,103,104,104,104,105,105,106,106,107,107,107,108,108,109,109,110,110,111,111,111,112,112,113,113,114,114,114,115,115,116,116,117,117,118,118,118,119,119,120,120,121,121,121,122,122,123,123,124,124,125,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,132,132,132,133,133,134,134,135,135,135,136,
54,54,55,55,56,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,67,67,67,68,68,69,69,70,70,71,71,71,72,72,73,73,74,74,74,75,75,76,76,77,77,78,78,78,79,79,80,80,81,81,82,82,82,83,83,84,84,85,85,86,86,86,87,87,88,88,89,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,97,98,98,99,99,100,100,100,101,101,102,102,103,103,104,104,104,105,105,106,106,107,107,108,108,108,109,109,110,110,111,111,112,112,112,113,113,114,114,115,115,115,116,116,117,117,118,118,119,119,119,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,127,127,127,128,128,129,129,130,130,130,131,131,132,132,133,133,134,134,134,135,135,136,136,
53,54,54,55,55,56,56,57,57,57,58,58,59,59,60,60,61,61,61,62,62,63,63,64,64,65,65,65,66,66,67,67,68,68,69,69,69,70,70,71,71,72,72,73,73,73,74,74,75,75,76,76,77,77,77,78,78,79,79,80,80,81,81,81,82,82,83,83,84,84,85,85,85,86,86,87,87,88,88,89,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,97,98,98,99,99,100,100,101,101,101,102,102,103,103,104,104,105,105,105,106,106,107,107,108,108,109,109,109,110,110,111,111,112,112,113,113,113,114,114,115,115,116,116,117,117,117,118,118,119,119,120,120,121,121,121,122,122,123,123,124,124,125,125,125,126,126,127,127,128,128,129,129,129,130,130,131,131,132,132,133,133,133,134,134,135,135,136,136,137,
53,54,54,54,55,55,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,67,67,67,68,68,69,69,70,70,71,71,72,72,72,73,73,74,74,75,75,76,76,76,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,88,88,89,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,97,98,98,99,99,100,100,101,101,102,102,102,103,103,104,104,105,105,106,106,106,107,107,108,108,109,109,110,110,110,111,111,112,112,113,113,114,114,114,115,115,116,116,117,117,118,118,119,119,119,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,127,127,127,128,128,129,129,130,130,131,131,132,132,132,133,133,134,134,135,135,136,136,136,137,
53,53,54,54,55,55,56,56,56,57,57,58,58,59,59,60,60,61,61,61,62,62,63,63,64,64,65,65,65,66,66,67,67,68,68,69,69,70,70,70,71,71,72,72,73,73,74,74,74,75,75,76,76,77,77,78,78,79,79,79,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,88,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,98,99,99,100,100,101,101,102,102,102,103,103,104,104,105,105,106,106,107,107,107,108,108,109,109,110,110,111,111,112,112,112,113,113,114,114,115,115,116,116,116,117,117,118,118,119,119,120,120,121,121,121,122,122,123,123,124,124,125,125,125,126,126,127,127,128,128,129,129,130,130,130,131,131,132,132,133,133,134,134,135,135,135,136,136,137,137,
53,53,53,54,54,55,55,56,56,57,57,58,58,58,59,59,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,67,67,68,68,68,69,69,70,70,71,71,72,72,73,73,73,74,74,75,75,76,76,77,77,78,78,78,79,79,80,80,81,81,82,82,83,83,83,84,84,85,85,86,86,87,87,88,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,98,99,99,100,100,101,101,102,102,103,103,103,104,104,105,105,106,106,107,107,108,108,108,109,109,110,110,111,111,112,112,113,113,113,114,114,115,115,116,116,117,117,118,118,118,119,119,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,132,132,133,133,133,134,134,135,135,136,136,137,137,138,
52,53,53,54,54,55,55,55,56,56,57,57,58,58,59,59,60,60,60,61,61,62,62,63,63,64,64,65,65,66,66,66,67,67,68,68,69,69,70,70,71,71,71,72,72,73,73,74,74,75,75,76,76,77,77,77,78,78,79,79,80,80,81,81,82,82,82,83,83,84,84,85,85,86,86,87,87,88,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,98,99,99,100,100,101,101,102,102,103,103,104,104,104,105,105,106,106,107,107,108,108,109,109,109,110,110,111,111,112,112,113,113,114,114,115,115,115,116,116,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,124,124,125,125,126,126,126,127,127,128,128,129,129,130,130,131,131,131,132,132,133,133,134,134,135,135,136,136,137,137,137,138,
52,52,53,53,54,54,55,55,56,56,57,57,57,58,58,59,59,60,60,61,61,62,62,63,63,63,64,64,65,65,66,66,67,67,68,68,69,69,69,70,70,71,71,72,72,73,73,74,74,75,75,75,76,76,77,77,78,78,79,79,80,80,81,81,81,82,82,83,83,84,84,85,85,86,86,87,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,99,100,100,101,101,102,102,103,103,104,104,105,105,105,106,106,107,107,108,108,109,109,110,110,111,111,111,112,112,113,113,114,114,115,115,116,116,117,117,117,118,118,119,119,120,120,121,121,122,122,123,123,123,124,124,125,125,126,126,127,127,128,128,129,129,129,130,130,131,131,132,132,133,133,134,134,135,135,135,136,136,137,137,138,138,
52,52,53,53,53,54,54,55,55,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,80,81,81,82,82,83,83,84,84,85,85,86,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,100,101,101,102,102,103,103,104,104,105,105,106,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,127,128,128,129,129,130,130,131,131,132,132,133,133,133,134,134,135,135,136,136,137,137,138,138,139,
51,52,52,53,53,54,54,55,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,
51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,137,138,138,139,139,
51,51,52,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,134,135,135,136,136,137,137,138,138,139,139,140,
50,51,51,52,52,53,53,54,54,55,55,56,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,
50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,
50,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,136,137,137,138,138,139,139,140,140,141,
49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,
49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,142,
48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,142,142,
48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,140,140,141,141,142,142,143,
48,48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,142,142,143,
47,48,48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,142,142,143,143,
47,47,48,48,49,49,50,50,51,51,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,121,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,135,135,136,136,137,137,138,138,139,139,140,140,141,141,142,142,143,143,144,
46,47,47,48,48,49,50,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,58,59,59,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,72,72,73,73,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,112,113,113,114,114,115,116,116,117,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,125,126,127,127,128,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,136,137,138,138,139,139,140,140,141,141,142,142,143,143,144,144,
46,47,47,48,48,49,49,50,50,51,51,52,52,53,53,54,54,55,55,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,103,103,104,104,105,105,106,106,107,107,108,108,109,109,110,110,111,111,112,113,113,114,114,115,115,116,116,117,117,118,118,119,119,120,120,121,122,122,123,123,124,124,125,125,126,126,127,127,128,128,129,129,130,131,131,132,132,133,133,134,134,135,135,136,136,137,137,138,138,139,139,140,141,141,142,142,143,143,144,144,145,
46,46,47,47,48,48,49,49,50,50,51,51,52,52,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,70,70,71,71,72,72,73,73,74,74,75,75,76,76,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108,109,110,110,111,111,112,112,113,113,114,114,115,115,116,116,117,118,118,119,119,120,120,121,121,122,122,123,123,124,124,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132,133,134,134,135,135,136,136,137,137,138,138,139,139,140,140,141,142,142,143,143,144,144,145,145,
45,46,46,47,47,48,48,49,49,50,51,51,52,52,53,53,54,54,55,55,56,56,57,57,58,59,59,60,60,61,61,62,62,63,63,64,64,65,66,66,67,67,68,68,69,69,70,70,71,71,72,73,73,74,74,75,75,76,76,77,77,78,78,79,80,80,81,81,82,82,83,83,84,84,85,85,86,87,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,99,100,101,101,102,102,103,103,104,104,105,105,106,106,107,108,108,109,109,110,110,111,111,112,112,113,113,114,115,115,116,116,117,117,118,118,119,119,120,120,121,122,122,123,123,124,124,125,125,126,126,127,127,128,129,129,130,130,131,131,132,132,133,133,134,134,135,135,136,137,137,138,138,139,139,140,140,141,141,142,142,143,144,144,145,145,146,
45,45,46,46,47,47,48,48,49,50,50,51,51,52,52,53,53,54,54,55,56,56,57,57,58,58,59,59,60,60,61,62,62,63,63,64,64,65,65,66,66,67,67,68,69,69,70,70,71,71,72,72,73,73,74,75,75,76,76,77,77,78,78,79,79,80,81,81,82,82,83,83,84,84,85,85,86,86,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,100,100,101,101,102,102,103,103,104,104,105,105,106,107,107,108,108,109,109,110,110,111,111,112,113,113,114,114,115,115,116,116,117,117,118,119,119,120,120,121,121,122,122,123,123,124,124,125,126,126,127,127,128,128,129,129,130,130,131,132,132,133,133,134,134,135,135,136,136,137,138,138,139,139,140,140,141,141,142,142,143,143,144,145,145,146,146,
44,45,45,46,46,47,48,48,49,49,50,50,51,51,52,52,53,54,54,55,55,56,56,57,57,58,58,59,60,60,61,61,62,62,63,63,64,65,65,66,66,67,67,68,68,69,69,70,71,71,72,72,73,73,74,74,75,75,76,77,77,78,78,79,79,80,80,81,81,82,83,83,84,84,85,85,86,86,87,88,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,98,99,100,100,101,101,102,102,103,103,104,105,105,106,106,107,107,108,108,109,109,110,111,111,112,112,113,113,114,114,115,115,116,117,117,118,118,119,119,120,120,121,121,122,123,123,124,124,125,125,126,126,127,128,128,129,129,130,130,131,131,132,132,133,134,134,135,135,136,136,137,137,138,138,139,140,140,141,141,142,142,143,143,144,145,145,146,146,147,
44,44,45,45,46,47,47,48,48,49,49,50,50,51,52,52,53,53,54,54,55,55,56,56,57,58,58,59,59,60,60,61,61,62,63,63,64,64,65,65,66,66,67,68,68,69,69,70,70,71,71,72,73,73,74,74,75,75,76,76,77,78,78,79,79,80,80,81,81,82,82,83,84,84,85,85,86,86,87,87,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,99,99,100,100,101,101,102,102,103,104,104,105,105,106,106,107,107,108,108,109,110,110,111,111,112,112,113,113,114,115,115,116,116,117,117,118,118,119,120,120,121,121,122,122,123,123,124,125,125,126,126,127,127,128,128,129,130,130,131,131,132,132,133,133,134,134,135,136,136,137,137,138,138,139,139,140,141,141,142,142,143,143,144,144,145,146,146,147,147,
43,44,44,45,46,46,47,47,48,48,49,49,50,51,51,52,52,53,53,54,54,55,56,56,57,57,58,58,59,59,60,61,61,62,62,63,63,64,65,65,66,66,67,67,68,68,69,70,70,71,71,72,72,73,73,74,75,75,76,76,77,77,78,78,79,80,80,81,81,82,82,83,84,84,85,85,86,86,87,87,88,89,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,97,98,99,99,100,100,101,101,102,102,103,104,104,105,105,106,106,107,108,108,109,109,110,110,111,111,112,113,113,114,114,115,115,116,116,117,118,118,119,119,120,120,121,121,122,123,123,124,124,125,125,126,127,127,128,128,129,129,130,130,131,132,132,133,133,134,134,135,135,136,137,137,138,138,139,139,140,140,141,142,142,143,143,144,144,145,145,146,147,147,148,
43,43,44,45,45,46,46,47,47,48,48,49,50,50,51,51,52,52,53,54,54,55,55,56,56,57,57,58,59,59,60,60,61,61,62,63,63,64,64,65,65,66,67,67,68,68,69,69,70,70,71,72,72,73,73,74,74,75,76,76,77,77,78,78,79,79,80,81,81,82,82,83,83,84,85,85,86,86,87,87,88,88,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,98,98,99,99,100,100,101,101,102,103,103,104,104,105,105,106,107,107,108,108,109,109,110,110,111,112,112,113,113,114,114,115,116,116,117,117,118,118,119,119,120,121,121,122,122,123,123,124,125,125,126,126,127,127,128,129,129,130,130,131,131,132,132,133,134,134,135,135,136,136,137,138,138,139,139,140,140,141,141,142,143,143,144,144,145,145,146,147,147,148,148,
42,43,43,44,45,45,46,46,47,47,48,49,49,50,50,51,51,52,53,53,54,54,55,55,56,57,57,58,58,59,59,60,61,61,62,62,63,63,64,65,65,66,66,67,67,68,69,69,70,70,71,71,72,73,73,74,74,75,75,76,76,77,78,78,79,79,80,80,81,82,82,83,83,84,84,85,86,86,87,87,88,88,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,98,98,99,99,100,100,101,102,102,103,103,104,104,105,106,106,107,107,108,108,109,110,110,111,111,112,112,113,113,114,115,115,116,116,117,117,118,119,119,120,120,121,121,122,123,123,124,124,125,125,126,127,127,128,128,129,129,130,131,131,132,132,133,133,134,135,135,136,136,137,137,138,139,139,140,140,141,141,142,143,143,144,144,145,145,146,147,147,148,148,149,
42,42,43,44,44,45,45,46,46,47,48,48,49,49,50,50,51,52,52,53,53,54,54,55,56,56,57,57,58,59,59,60,60,61,61,62,63,63,64,64,65,65,66,67,67,68,68,69,69,70,71,71,72,72,73,73,74,75,75,76,76,77,77,78,79,79,80,80,81,82,82,83,83,84,84,85,86,86,87,87,88,88,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,98,98,99,99,100,100,101,102,102,103,103,104,104,105,106,106,107,107,108,109,109,110,110,111,111,112,113,113,114,114,115,115,116,117,117,118,118,119,119,120,121,121,122,122,123,123,124,125,125,126,126,127,127,128,129,129,130,130,131,132,132,133,133,134,134,135,136,136,137,137,138,138,139,140,140,141,141,142,142,143,144,144,145,145,146,146,147,148,148,149,149,
41,42,43,43,44,44,45,45,46,47,47,48,48,49,49,50,51,51,52,52,53,54,54,55,55,56,56,57,58,58,59,59,60,60,61,62,62,63,63,64,65,65,66,66,67,67,68,69,69,70,70,71,72,72,73,73,74,74,75,76,76,77,77,78,78,79,80,80,81,81,82,83,83,84,84,85,85,86,87,87,88,88,89,90,90,91,91,92,92,93,94,94,95,95,96,96,97,98,98,99,99,100,101,101,102,102,103,103,104,105,105,106,106,107,108,108,109,109,110,110,111,112,112,113,113,114,114,115,116,116,117,117,118,119,119,120,120,121,121,122,123,123,124,124,125,126,126,127,127,128,128,129,130,130,131,131,132,132,133,134,134,135,135,136,137,137,138,138,139,139,140,141,141,142,142,143,143,144,145,145,146,146,147,148,148,149,149,150,
41,41,42,43,43,44,44,45,46,46,47,47,48,48,49,50,50,51,51,52,53,53,54,54,55,55,56,57,57,58,58,59,60,60,61,61,62,63,63,64,64,65,65,66,67,67,68,68,69,70,70,71,71,72,72,73,74,74,75,75,76,77,77,78,78,79,80,80,81,81,82,82,83,84,84,85,85,86,87,87,88,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,98,99,99,100,101,101,102,102,103,104,104,105,105,106,106,107,108,108,109,109,110,111,111,112,112,113,114,114,115,115,116,116,117,118,118,119,119,120,121,121,122,122,123,123,124,125,125,126,126,127,128,128,129,129,130,131,131,132,132,133,133,134,135,135,136,136,137,138,138,139,139,140,140,141,142,142,143,143,144,145,145,146,146,147,148,148,149,149,150,150,
40,41,41,42,43,43,44,44,45,46,46,47,47,48,49,49,50,50,51,52,52,53,53,54,55,55,56,56,57,57,58,59,59,60,60,61,62,62,63,63,64,65,65,66,66,67,68,68,69,69,70,70,71,72,72,73,73,74,75,75,76,76,77,78,78,79,79,80,81,81,82,82,83,84,84,85,85,86,86,87,88,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,98,99,100,100,101,101,102,102,103,104,104,105,105,106,107,107,108,108,109,110,110,111,111,112,113,113,114,114,115,116,116,117,117,118,118,119,120,120,121,121,122,123,123,124,124,125,126,126,127,127,128,129,129,130,130,131,131,132,133,133,134,134,135,136,136,137,137,138,139,139,140,140,141,142,142,143,143,144,145,145,146,146,147,147,148,149,149,150,150,151,
40,40,41,42,42,43,43,44,45,45,46,46,47,48,48,49,49,50,51,51,52,52,53,54,54,55,55,56,57,57,58,58,59,60,60,61,61,62,62,63,64,64,65,65,66,67,67,68,68,69,70,70,71,71,72,73,73,74,74,75,76,76,77,77,78,79,79,80,80,81,82,82,83,83,84,85,85,86,86,87,88,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,98,99,100,100,101,101,102,103,103,104,104,105,106,106,107,107,108,109,109,110,110,111,112,112,113,113,114,115,115,116,116,117,118,118,119,119,120,121,121,122,122,123,124,124,125,125,126,126,127,128,128,129,129,130,131,131,132,132,133,134,134,135,135,136,137,137,138,138,139,140,140,141,141,142,143,143,144,144,145,146,146,147,147,148,149,149,150,150,151,152,
39,40,40,41,42,42,43,43,44,45,45,46,46,47,48,48,49,49,50,51,51,52,53,53,54,54,55,56,56,57,57,58,59,59,60,60,61,62,62,63,63,64,65,65,66,66,67,68,68,69,69,70,71,71,72,72,73,74,74,75,75,76,77,77,78,78,79,80,80,81,82,82,83,83,84,85,85,86,86,87,88,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,98,99,100,100,101,101,102,103,103,104,104,105,106,106,107,108,108,109,109,110,111,111,112,112,113,114,114,115,115,116,117,117,118,118,119,120,120,121,121,122,123,123,124,124,125,126,126,127,127,128,129,129,130,130,131,132,132,133,133,134,135,135,136,137,137,138,138,139,140,140,141,141,142,143,143,144,144,145,146,146,147,147,148,149,149,150,150,151,152,152,
39,39,40,40,41,42,42,43,44,44,45,45,46,47,47,48,48,49,50,50,51,51,52,53,53,54,55,55,56,56,57,58,58,59,59,60,61,61,62,62,63,64,64,65,66,66,67,67,68,69,69,70,70,71,72,72,73,73,74,75,75,76,77,77,78,78,79,80,80,81,81,82,83,83,84,84,85,86,86,87,88,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,98,99,100,100,101,102,102,103,103,104,105,105,106,106,107,108,108,109,109,110,111,111,112,113,113,114,114,115,116,116,117,117,118,119,119,120,120,121,122,122,123,124,124,125,125,126,127,127,128,128,129,130,130,131,131,132,133,133,134,135,135,136,136,137,138,138,139,139,140,141,141,142,142,143,144,144,145,146,146,147,147,148,149,149,150,150,151,152,152,153,
38,39,39,40,41,41,42,42,43,44,44,45,45,46,47,47,48,49,49,50,50,51,52,52,53,54,54,55,55,56,57,57,58,58,59,60,60,61,62,62,63,63,64,65,65,66,66,67,68,68,69,70,70,71,71,72,73,73,74,74,75,76,76,77,78,78,79,79,80,81,81,82,83,83,84,84,85,86,86,87,87,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,99,99,100,100,101,102,102,103,103,104,105,105,106,107,107,108,108,109,110,110,111,112,112,113,113,114,115,115,116,116,117,118,118,119,120,120,121,121,122,123,123,124,124,125,126,126,127,128,128,129,129,130,131,131,132,132,133,134,134,135,136,136,137,137,138,139,139,140,141,141,142,142,143,144,144,145,145,146,147,147,148,149,149,150,150,151,152,152,153,153,
38,38,39,39,40,41,41,42,42,43,44,44,45,46,46,47,47,48,49,49,50,51,51,52,52,53,54,54,55,56,56,57,57,58,59,59,60,61,61,62,62,63,64,64,65,66,66,67,67,68,69,69,70,71,71,72,72,73,74,74,75,76,76,77,77,78,79,79,80,81,81,82,82,83,84,84,85,86,86,87,87,88,89,89,90,91,91,92,92,93,94,94,95,95,96,97,97,98,99,99,100,100,101,102,102,103,104,104,105,105,106,107,107,108,109,109,110,110,111,112,112,113,114,114,115,115,116,117,117,118,119,119,120,120,121,122,122,123,124,124,125,125,126,127,127,128,129,129,130,130,131,132,132,133,134,134,135,135,136,137,137,138,139,139,140,140,141,142,142,143,144,144,145,145,146,147,147,148,148,149,150,150,151,152,152,153,153,154,
37,38,38,39,39,40,41,41,42,43,43,44,44,45,46,46,47,48,48,49,50,50,51,51,52,53,53,54,55,55,56,56,57,58,58,59,60,60,61,61,62,63,63,64,65,65,66,67,67,68,68,69,70,70,71,72,72,73,73,74,75,75,76,77,77,78,79,79,80,80,81,82,82,83,84,84,85,85,86,87,87,88,89,89,90,90,91,92,92,93,94,94,95,96,96,97,97,98,99,99,100,101,101,102,102,103,104,104,105,106,106,107,107,108,109,109,110,111,111,112,113,113,114,114,115,116,116,117,118,118,119,119,120,121,121,122,123,123,124,125,125,126,126,127,128,128,129,130,130,131,131,132,133,133,134,135,135,136,136,137,138,138,139,140,140,141,142,142,143,143,144,145,145,146,147,147,148,148,149,150,150,151,152,152,153,153,154,155,
36,37,38,38,39,39,40,41,41,42,43,43,44,45,45,46,46,47,48,48,49,50,50,51,52,52,53,54,54,55,55,56,57,57,58,59,59,60,61,61,62,62,63,64,64,65,66,66,67,68,68,69,69,70,71,71,72,73,73,74,75,75,76,76,77,78,78,79,80,80,81,82,82,83,83,84,85,85,86,87,87,88,89,89,90,90,91,92,92,93,94,94,95,96,96,97,97,98,99,99,100,101,101,102,103,103,104,104,105,106,106,107,108,108,109,110,110,111,111,112,113,113,114,115,115,116,117,117,118,118,119,120,120,121,122,122,123,124,124,125,125,126,127,127,128,129,129,130,131,131,132,132,133,134,134,135,136,136,137,138,138,139,140,140,141,141,142,143,143,144,145,145,146,147,147,148,148,149,150,150,151,152,152,153,154,154,155,155,
36,36,37,38,38,39,40,40,41,41,42,43,43,44,45,45,46,47,47,48,49,49,50,50,51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,61,62,63,63,64,65,65,66,67,67,68,69,69,70,70,71,72,72,73,74,74,75,76,76,77,78,78,79,79,80,81,81,82,83,83,84,85,85,86,87,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,99,100,101,101,102,103,103,104,105,105,106,107,107,108,108,109,110,110,111,112,112,113,114,114,115,116,116,117,117,118,119,119,120,121,121,122,123,123,124,125,125,126,126,127,128,128,129,130,130,131,132,132,133,134,134,135,136,136,137,137,138,139,139,140,141,141,142,143,143,144,145,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,155,156,
35,36,36,37,38,38,39,40,40,41,42,42,43,44,44,45,45,46,47,47,48,49,49,50,51,51,52,53,53,54,55,55,56,57,57,58,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,69,70,70,71,72,72,73,73,74,75,75,76,77,77,78,79,79,80,81,81,82,83,83,84,85,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,101,102,103,103,104,105,105,106,107,107,108,109,109,110,111,111,112,113,113,114,114,115,116,116,117,118,118,119,120,120,121,122,122,123,124,124,125,126,126,127,128,128,129,129,130,131,131,132,133,133,134,135,135,136,137,137,138,139,139,140,141,141,142,142,143,144,144,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,
34,35,36,36,37,38,38,39,40,40,41,42,42,43,44,44,45,46,46,47,48,48,49,50,50,51,52,52,53,53,54,55,55,56,57,57,58,59,59,60,61,61,62,63,63,64,65,65,66,67,67,68,69,69,70,71,71,72,73,73,74,75,75,76,77,77,78,79,79,80,80,81,82,82,83,84,84,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,102,102,103,104,104,105,106,106,107,107,108,109,109,110,111,111,112,113,113,114,115,115,116,117,117,118,119,119,120,121,121,122,123,123,124,125,125,126,127,127,128,129,129,130,131,131,132,133,133,134,134,135,136,136,137,138,138,139,140,140,141,142,142,143,144,144,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,
34,34,35,36,36,37,38,38,39,40,40,41,42,42,43,44,44,45,46,46,47,48,48,49,50,50,51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,69,70,70,71,72,72,73,74,74,75,76,76,77,78,78,79,80,80,81,82,82,83,84,84,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,102,102,103,104,104,105,106,106,107,108,108,109,110,110,111,112,112,113,114,114,115,116,116,117,118,118,119,120,120,121,122,122,123,124,124,125,126,126,127,128,128,129,130,130,131,132,132,133,134,134,135,136,136,137,138,138,139,140,140,141,142,142,143,144,144,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,158,
33,34,34,35,36,36,37,38,38,39,40,40,41,42,42,43,44,45,45,46,47,47,48,49,49,50,51,51,52,53,53,54,55,55,56,57,57,58,59,59,60,61,61,62,63,63,64,65,65,66,67,67,68,69,69,70,71,71,72,73,73,74,75,75,76,77,78,78,79,80,80,81,82,82,83,84,84,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,102,102,103,104,104,105,106,106,107,108,108,109,110,111,111,112,113,113,114,115,115,116,117,117,118,119,119,120,121,121,122,123,123,124,125,125,126,127,127,128,129,129,130,131,131,132,133,133,134,135,135,136,137,137,138,139,139,140,141,141,142,143,144,144,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,158,159,
32,33,34,34,35,36,36,37,38,38,39,40,41,41,42,43,43,44,45,45,46,47,47,48,49,49,50,51,51,52,53,53,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,69,70,71,71,72,73,73,74,75,75,76,77,77,78,79,79,80,81,81,82,83,83,84,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,102,103,103,104,105,105,106,107,107,108,109,109,110,111,111,112,113,113,114,115,115,116,117,118,118,119,120,120,121,122,122,123,124,124,125,126,126,127,128,128,129,130,130,131,132,133,133,134,135,135,136,137,137,138,139,139,140,141,141,142,143,143,144,145,145,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,158,159,160,
32,32,33,34,34,35,36,36,37,38,39,39,40,41,41,42,43,43,44,45,45,46,47,48,48,49,50,50,51,52,52,53,54,54,55,56,56,57,58,59,59,60,61,61,62,63,63,64,65,65,66,67,67,68,69,70,70,71,72,72,73,74,74,75,76,76,77,78,79,79,80,81,81,82,83,83,84,85,85,86,87,87,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,99,99,100,101,101,102,103,103,104,105,105,106,107,107,108,109,110,110,111,112,112,113,114,114,115,116,116,117,118,119,119,120,121,121,122,123,123,124,125,125,126,127,127,128,129,130,130,131,132,132,133,134,134,135,136,136,137,138,138,139,140,141,141,142,143,143,144,145,145,146,147,147,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,158,159,160,161,
31,32,32,33,34,34,35,36,37,37,38,39,39,40,41,41,42,43,43,44,45,46,46,47,48,48,49,50,50,51,52,53,53,54,55,55,56,57,57,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,69,69,70,71,71,72,73,73,74,75,76,76,77,78,78,79,80,80,81,82,83,83,84,85,85,86,87,87,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,99,99,100,101,101,102,103,103,104,105,106,106,107,108,108,109,110,110,111,112,113,113,114,115,115,116,117,117,118,119,120,120,121,122,122,123,124,124,125,126,126,127,128,129,129,130,131,131,132,133,133,134,135,136,136,137,138,138,139,140,140,141,142,143,143,144,145,145,146,147,147,148,149,149,150,151,152,152,153,154,154,155,156,156,157,158,159,159,160,161,161,
30,31,32,32,33,34,34,35,36,37,37,38,39,39,40,41,41,42,43,44,44,45,46,46,47,48,49,49,50,51,51,52,53,53,54,55,56,56,57,58,58,59,60,61,61,62,63,63,64,65,65,66,67,68,68,69,70,70,71,72,73,73,74,75,75,76,77,77,78,79,80,80,81,82,82,83,84,85,85,86,87,87,88,89,89,90,91,92,92,93,94,94,95,96,97,97,98,99,99,100,101,101,102,103,104,104,105,106,106,107,108,109,109,110,111,111,112,113,113,114,115,116,116,117,118,118,119,120,121,121,122,123,123,124,125,125,126,127,128,128,129,130,130,131,132,133,133,134,135,135,136,137,137,138,139,140,140,141,142,142,143,144,145,145,146,147,147,148,149,149,150,151,152,152,153,154,154,155,156,157,157,158,159,159,160,161,161,162,
29,30,31,32,32,33,34,34,35,36,37,37,38,39,39,40,41,42,42,43,44,44,45,46,47,47,48,49,49,50,51,52,52,53,54,54,55,56,57,57,58,59,59,60,61,62,62,63,64,64,65,66,67,67,68,69,69,70,71,72,72,73,74,74,75,76,77,77,78,79,79,80,81,82,82,83,84,84,85,86,87,87,88,89,89,90,91,92,92,93,94,94,95,96,97,97,98,99,99,100,101,102,102,103,104,104,105,106,107,107,108,109,109,110,111,112,112,113,114,114,115,116,117,117,118,119,119,120,121,122,122,123,124,124,125,126,127,127,128,129,129,130,131,132,132,133,134,134,135,136,137,137,138,139,139,140,141,142,142,143,144,144,145,146,147,147,148,149,149,150,151,152,152,153,154,154,155,156,157,157,158,159,159,160,161,162,162,163,
29,29,30,31,32,32,33,34,34,35,36,37,37,38,39,39,40,41,42,42,43,44,45,45,46,47,47,48,49,50,50,51,52,52,53,54,55,55,56,57,58,58,59,60,60,61,62,63,63,64,65,66,66,67,68,68,69,70,71,71,72,73,73,74,75,76,76,77,78,79,79,80,81,81,82,83,84,84,85,86,86,87,88,89,89,90,91,92,92,93,94,94,95,96,97,97,98,99,100,100,101,102,102,103,104,105,105,106,107,107,108,109,110,110,111,112,113,113,114,115,115,116,117,118,118,119,120,120,121,122,123,123,124,125,126,126,127,128,128,129,130,131,131,132,133,134,134,135,136,136,137,138,139,139,140,141,141,142,143,144,144,145,146,147,147,148,149,149,150,151,152,152,153,154,154,155,156,157,157,158,159,160,160,161,162,162,163,164,
28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,40,41,42,42,43,44,45,45,46,47,48,48,49,50,51,51,52,53,53,54,55,56,56,57,58,59,59,60,61,62,62,63,64,64,65,66,67,67,68,69,70,70,71,72,72,73,74,75,75,76,77,78,78,79,80,81,81,82,83,83,84,85,86,86,87,88,89,89,90,91,92,92,93,94,94,95,96,97,97,98,99,100,100,101,102,103,103,104,105,105,106,107,108,108,109,110,111,111,112,113,114,114,115,116,116,117,118,119,119,120,121,122,122,123,124,124,125,126,127,127,128,129,130,130,131,132,133,133,134,135,135,136,137,138,138,139,140,141,141,142,143,144,144,145,146,146,147,148,149,149,150,151,152,152,153,154,155,155,156,157,157,158,159,160,160,161,162,163,163,164,165,
27,28,28,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,40,41,42,43,43,44,45,46,46,47,48,49,49,50,51,51,52,53,54,54,55,56,57,57,58,59,60,60,61,62,63,63,64,65,66,66,67,68,69,69,70,71,71,72,73,74,74,75,76,77,77,78,79,80,80,81,82,83,83,84,85,86,86,87,88,89,89,90,91,92,92,93,94,94,95,96,97,97,98,99,100,100,101,102,103,103,104,105,106,106,107,108,109,109,110,111,112,112,113,114,115,115,116,117,117,118,119,120,120,121,122,123,123,124,125,126,126,127,128,129,129,130,131,132,132,133,134,135,135,136,137,137,138,139,140,140,141,142,143,143,144,145,146,146,147,148,149,149,150,151,152,152,153,154,155,155,156,157,158,158,159,160,160,161,162,163,163,164,165,166,
26,27,28,28,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,40,41,42,43,43,44,45,46,46,47,48,49,49,50,51,52,52,53,54,55,55,56,57,58,58,59,60,61,61,62,63,64,64,65,66,67,67,68,69,70,70,71,72,73,73,74,75,76,76,77,78,79,79,80,81,82,82,83,84,85,85,86,87,88,88,89,90,91,91,92,93,94,95,95,96,97,98,98,99,100,101,101,102,103,104,104,105,106,107,107,108,109,110,110,111,112,113,113,114,115,116,116,117,118,119,119,120,121,122,122,123,124,125,125,126,127,128,128,129,130,131,131,132,133,134,134,135,136,137,137,138,139,140,140,141,142,143,143,144,145,146,146,147,148,149,149,150,151,152,152,153,154,155,155,156,157,158,158,159,160,161,161,162,163,164,164,165,166,167,
25,26,27,28,28,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,41,42,43,44,44,45,46,47,47,48,49,50,50,51,52,53,53,54,55,56,56,57,58,59,60,60,61,62,63,63,64,65,66,66,67,68,69,69,70,71,72,72,73,74,75,76,76,77,78,79,79,80,81,82,82,83,84,85,85,86,87,88,88,89,90,91,91,92,93,94,95,95,96,97,98,98,99,100,101,101,102,103,104,104,105,106,107,107,108,109,110,110,111,112,113,114,114,115,116,117,117,118,119,120,120,121,122,123,123,124,125,126,126,127,128,129,130,130,131,132,133,133,134,135,136,136,137,138,139,139,140,141,142,142,143,144,145,145,146,147,148,149,149,150,151,152,152,153,154,155,155,156,157,158,158,159,160,161,161,162,163,164,165,165,166,167,168,
24,25,26,27,27,28,29,30,31,31,32,33,34,34,35,36,37,38,38,39,40,41,41,42,43,44,44,45,46,47,48,48,49,50,51,51,52,53,54,54,55,56,57,58,58,59,60,61,61,62,63,64,64,65,66,67,68,68,69,70,71,71,72,73,74,75,75,76,77,78,78,79,80,81,81,82,83,84,85,85,86,87,88,88,89,90,91,91,92,93,94,95,95,96,97,98,98,99,100,101,101,102,103,104,105,105,106,107,108,108,109,110,111,111,112,113,114,115,115,116,117,118,118,119,120,121,122,122,123,124,125,125,126,127,128,128,129,130,131,132,132,133,134,135,135,136,137,138,138,139,140,141,142,142,143,144,145,145,146,147,148,148,149,150,151,152,152,153,154,155,155,156,157,158,159,159,160,161,162,162,163,164,165,165,166,167,168,169,
24,24,25,26,27,27,28,29,30,31,31,32,33,34,34,35,36,37,38,38,39,40,41,41,42,43,44,45,45,46,47,48,48,49,50,51,52,52,53,54,55,56,56,57,58,59,59,60,61,62,63,63,64,65,66,66,67,68,69,70,70,71,72,73,73,74,75,76,77,77,78,79,80,81,81,82,83,84,84,85,86,87,88,88,89,90,91,91,92,93,94,95,95,96,97,98,98,99,100,101,102,102,103,104,105,105,106,107,108,109,109,110,111,112,113,113,114,115,116,116,117,118,119,120,120,121,122,123,123,124,125,126,127,127,128,129,130,130,131,132,133,134,134,135,136,137,138,138,139,140,141,141,142,143,144,145,145,146,147,148,148,149,150,151,152,152,153,154,155,155,156,157,158,159,159,160,161,162,162,163,164,165,166,166,167,168,169,170,
23,23,24,25,26,27,27,28,29,30,30,31,32,33,34,34,35,36,37,38,38,39,40,41,42,42,43,44,45,46,46,47,48,49,49,50,51,52,53,53,54,55,56,57,57,58,59,60,61,61,62,63,64,65,65,66,67,68,68,69,70,71,72,72,73,74,75,76,76,77,78,79,80,80,81,82,83,84,84,85,86,87,87,88,89,90,91,91,92,93,94,95,95,96,97,98,99,99,100,101,102,102,103,104,105,106,106,107,108,109,110,110,111,112,113,114,114,115,116,117,118,118,119,120,121,121,122,123,124,125,125,126,127,128,129,129,130,131,132,133,133,134,135,136,137,137,138,139,140,140,141,142,143,144,144,145,146,147,148,148,149,150,151,152,152,153,154,155,156,156,157,158,159,159,160,161,162,163,163,164,165,166,167,167,168,169,170,171,
22,22,23,24,25,26,26,27,28,29,30,30,31,32,33,34,34,35,36,37,38,38,39,40,41,42,42,43,44,45,46,46,47,48,49,50,50,51,52,53,54,54,55,56,57,58,59,59,60,61,62,63,63,64,65,66,67,67,68,69,70,71,71,72,73,74,75,75,76,77,78,79,79,80,81,82,83,83,84,85,86,87,87,88,89,90,91,91,92,93,94,95,95,96,97,98,99,99,100,101,102,103,103,104,105,106,107,107,108,109,110,111,111,112,113,114,115,115,116,117,118,119,119,120,121,122,123,123,124,125,126,127,127,128,129,130,131,132,132,133,134,135,136,136,137,138,139,140,140,141,142,143,144,144,145,146,147,148,148,149,150,151,152,152,153,154,155,156,156,157,158,159,160,160,161,162,163,164,164,165,166,167,168,168,169,170,171,172,
21,21,22,23,24,25,25,26,27,28,29,30,30,31,32,33,34,34,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,52,52,53,54,55,56,56,57,58,59,60,60,61,62,63,64,65,65,66,67,68,69,69,70,71,72,73,73,74,75,76,77,78,78,79,80,81,82,82,83,84,85,86,86,87,88,89,90,91,91,92,93,94,95,95,96,97,98,99,100,100,101,102,103,104,104,105,106,107,108,108,109,110,111,112,113,113,114,115,116,117,117,118,119,120,121,121,122,123,124,125,126,126,127,128,129,130,130,131,132,133,134,134,135,136,137,138,139,139,140,141,142,143,143,144,145,146,147,147,148,149,150,151,152,152,153,154,155,156,156,157,158,159,160,161,161,162,163,164,165,165,166,167,168,169,169,170,171,172,173,
20,20,21,22,23,24,25,25,26,27,28,29,29,30,31,32,33,34,34,35,36,37,38,39,39,40,41,42,43,44,44,45,46,47,48,48,49,50,51,52,53,53,54,55,56,57,58,58,59,60,61,62,62,63,64,65,66,67,67,68,69,70,71,72,72,73,74,75,76,77,77,78,79,80,81,81,82,83,84,85,86,86,87,88,89,90,91,91,92,93,94,95,95,96,97,98,99,100,100,101,102,103,104,105,105,106,107,108,109,109,110,111,112,113,114,114,115,116,117,118,119,119,120,121,122,123,124,124,125,126,127,128,128,129,130,131,132,133,133,134,135,136,137,138,138,139,140,141,142,142,143,144,145,146,147,147,148,149,150,151,152,152,153,154,155,156,157,157,158,159,160,161,161,162,163,164,165,166,166,167,168,169,170,171,171,172,173,174,
19,19,20,21,22,23,24,24,25,26,27,28,29,29,30,31,32,33,34,34,35,36,37,38,39,39,40,41,42,43,44,44,45,46,47,48,49,50,50,51,52,53,54,55,55,56,57,58,59,60,60,61,62,63,64,65,65,66,67,68,69,70,70,71,72,73,74,75,75,76,77,78,79,80,80,81,82,83,84,85,85,86,87,88,89,90,90,91,92,93,94,95,96,96,97,98,99,100,101,101,102,103,104,105,106,106,107,108,109,110,111,111,112,113,114,115,116,116,117,118,119,120,121,121,122,123,124,125,126,126,127,128,129,130,131,131,132,133,134,135,136,136,137,138,139,140,141,142,142,143,144,145,146,147,147,148,149,150,151,152,152,153,154,155,156,157,157,158,159,160,161,162,162,163,164,165,166,167,167,168,169,170,171,172,172,173,174,175,
17,18,19,20,21,22,23,23,24,25,26,27,28,29,29,30,31,32,33,34,34,35,36,37,38,39,40,40,41,42,43,44,45,45,46,47,48,49,50,51,51,52,53,54,55,56,57,57,58,59,60,61,62,62,63,64,65,66,67,68,68,69,70,71,72,73,73,74,75,76,77,78,79,79,80,81,82,83,84,85,85,86,87,88,89,90,90,91,92,93,94,95,96,96,97,98,99,100,101,101,102,103,104,105,106,107,107,108,109,110,111,112,113,113,114,115,116,117,118,118,119,120,121,122,123,124,124,125,126,127,128,129,129,130,131,132,133,134,135,135,136,137,138,139,140,141,141,142,143,144,145,146,146,147,148,149,150,151,152,152,153,154,155,156,157,157,158,159,160,161,162,163,163,164,165,166,167,168,169,169,170,171,172,173,174,174,175,176,
16,17,18,19,20,21,22,22,23,24,25,26,27,28,28,29,30,31,32,33,34,34,35,36,37,38,39,40,40,41,42,43,44,45,46,47,47,48,49,50,51,52,53,53,54,55,56,57,58,59,59,60,61,62,63,64,65,65,66,67,68,69,70,71,71,72,73,74,75,76,77,78,78,79,80,81,82,83,84,84,85,86,87,88,89,90,90,91,92,93,94,95,96,96,97,98,99,100,101,102,102,103,104,105,106,107,108,108,109,110,111,112,113,114,115,115,116,117,118,119,120,121,121,122,123,124,125,126,127,127,128,129,130,131,132,133,133,134,135,136,137,138,139,139,140,141,142,143,144,145,146,146,147,148,149,150,151,152,152,153,154,155,156,157,158,158,159,160,161,162,163,164,164,165,166,167,168,169,170,170,171,172,173,174,175,176,177,177,
15,16,17,18,19,20,20,21,22,23,24,25,26,27,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,41,42,43,44,45,46,47,48,48,49,50,51,52,53,54,55,55,56,57,58,59,60,61,62,62,63,64,65,66,67,68,69,69,70,71,72,73,74,75,76,76,77,78,79,80,81,82,83,83,84,85,86,87,88,89,90,90,91,92,93,94,95,96,96,97,98,99,100,101,102,103,103,104,105,106,107,108,109,110,110,111,112,113,114,115,116,117,117,118,119,120,121,122,123,124,124,125,126,127,128,129,130,131,131,132,133,134,135,136,137,138,138,139,140,141,142,143,144,145,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,159,160,161,162,163,164,165,166,166,167,168,169,170,171,172,173,173,174,175,176,177,178,179,
14,15,16,17,18,18,19,20,21,22,23,24,25,26,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,50,51,52,53,54,55,56,57,58,58,59,60,61,62,63,64,65,66,66,67,68,69,70,71,72,73,73,74,75,76,77,78,79,80,81,81,82,83,84,85,86,87,88,89,89,90,91,92,93,94,95,96,97,97,98,99,100,101,102,103,104,105,105,106,107,108,109,110,111,112,113,113,114,115,116,117,118,119,120,120,121,122,123,124,125,126,127,128,128,129,130,131,132,133,134,135,136,136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,160,161,162,163,164,165,166,167,168,168,169,170,171,172,173,174,175,175,176,177,178,179,180,
13,14,15,16,16,17,18,19,20,21,22,23,24,25,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,58,59,60,61,61,62,63,64,65,66,67,68,69,70,70,71,72,73,74,75,76,77,78,79,79,80,81,82,83,84,85,86,87,88,88,89,90,91,92,93,94,95,96,97,98,98,99,100,101,102,103,104,105,106,107,107,108,109,110,111,112,113,114,115,116,116,117,118,119,120,121,122,123,124,125,125,126,127,128,129,130,131,132,133,134,134,135,136,137,138,139,140,141,142,143,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,161,162,163,164,165,166,167,168,169,170,170,171,172,173,174,175,176,177,178,179,179,180,181,
12,13,13,14,15,16,17,18,19,20,21,22,23,23,24,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,45,45,46,47,48,49,50,51,52,53,54,55,56,56,57,58,59,60,61,62,63,64,65,66,66,67,68,69,70,71,72,73,74,75,76,77,77,78,79,80,81,82,83,84,85,86,87,88,88,89,90,91,92,93,94,95,96,97,98,98,99,100,101,102,103,104,105,106,107,108,109,109,110,111,112,113,114,115,116,117,118,119,120,120,121,122,123,124,125,126,127,128,129,130,130,131,132,133,134,135,136,137,138,139,140,141,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,163,164,165,166,167,168,169,170,171,172,173,173,174,175,176,177,178,179,180,181,182,183,
10,11,12,13,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,45,46,47,47,48,49,50,51,52,53,54,55,56,57,58,59,60,60,61,62,63,64,65,66,67,68,69,70,71,72,73,73,74,75,76,77,78,79,80,81,82,83,84,85,86,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,100,101,102,103,104,105,106,107,108,109,110,111,112,113,113,114,115,116,117,118,119,120,121,122,123,124,125,126,126,127,128,129,130,131,132,133,134,135,136,137,138,139,139,140,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,164,165,165,166,167,168,169,170,171,172,173,174,175,176,177,178,178,179,180,181,182,183,184,
9,10,11,12,13,14,15,16,17,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,
8,9,10,11,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,175,176,177,178,179,180,181,182,183,184,185,186,187,
6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,0,0,
5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,0,0,0,
3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,0,0,0,0,0,
2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,0,0,0,0,0,0,
0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,176,177,178,179,180,181,182,183,184,185,186,187,0,0,0,0,0,0,0,0,
0,0,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,186,187,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,1,2,3,4,5,6,7,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,45,46,48,49,50,51,52,53,54,55,56,57,58,59,61,62,63,64,65,66,67,68,69,70,71,72,74,75,76,77,78,79,80,81,82,83,84,85,87,88,89,90,91,92,93,94,95,96,97,98,99,101,102,103,104,105,106,107,108,109,110,111,112,114,115,116,117,118,119,120,121,122,123,124,125,127,128,129,130,131,132,133,134,135,136,137,138,140,141,142,143,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,161,162,163,164,166,167,168,169,170,171,172,173,174,175,176,177,179,180,181,182,183,184,185,186,187,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,3,4,5,6,7,8,9,10,11,12,14,15,16,17,18,19,20,21,22,23,25,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,43,44,46,47,48,49,50,51,52,53,54,55,57,58,59,60,61,62,63,64,65,67,68,69,70,71,72,73,74,75,76,78,79,80,81,82,83,84,85,86,87,89,90,91,92,93,94,95,96,97,99,100,101,102,103,104,105,106,107,108,110,111,112,113,114,115,116,117,118,119,121,122,123,124,125,126,127,128,129,131,132,133,134,135,136,137,138,139,140,142,143,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,161,163,164,165,166,167,168,169,170,171,172,174,175,176,177,178,179,180,181,182,183,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,17,18,19,20,21,22,23,24,26,27,28,29,30,31,32,33,35,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,53,54,55,56,57,58,59,60,62,63,64,65,66,67,68,69,71,72,73,74,75,76,77,78,80,81,82,83,84,85,86,87,89,90,91,92,93,94,95,96,97,99,100,101,102,103,104,105,106,108,109,110,111,112,113,114,115,117,118,119,120,121,122,123,124,126,127,128,129,130,131,132,133,135,136,137,138,139,140,141,142,144,145,146,147,148,149,150,151,153,154,155,156,157,158,159,160,162,163,164,165,166,167,168,169,171,172,173,174,175,176,177,178,180,181,182,183,184,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,2,4,5,6,7,8,9,10,12,13,14,15,16,17,18,20,21,22,23,24,25,27,28,29,30,31,32,33,35,36,37,38,39,40,41,43,44,45,46,47,48,49,51,52,53,54,55,56,57,59,60,61,62,63,64,65,67,68,69,70,71,72,74,75,76,77,78,79,80,82,83,84,85,86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,106,107,108,109,110,111,112,114,115,116,117,118,119,121,122,123,124,125,126,127,129,130,131,132,133,134,135,137,138,139,140,141,142,143,145,146,147,148,149,150,151,153,154,155,156,157,158,159,161,162,163,164,165,166,168,169,170,171,172,173,174,176,177,178,179,180,181,182,184,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,8,9,10,11,12,13,15,16,17,18,19,21,22,23,24,25,26,28,29,30,31,32,33,35,36,37,38,39,40,42,43,44,45,46,47,49,50,51,52,53,54,56,57,58,59,60,61,63,64,65,66,67,68,70,71,72,73,74,75,77,78,79,80,81,82,84,85,86,87,88,89,91,92,93,94,95,97,98,99,100,101,102,104,105,106,107,108,109,111,112,113,114,115,116,118,119,120,121,122,123,125,126,127,128,129,130,132,133,134,135,136,137,139,140,141,142,143,144,146,147,148,149,150,151,153,154,155,156,157,158,160,161,162,163,164,165,167,168,169,170,171,173,174,175,176,177,178,180,181,182,183,184,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,5,6,7,8,9,11,12,13,14,15,17,18,19,20,21,23,24,25,26,27,29,30,31,32,33,35,36,37,38,39,41,42,43,44,45,46,48,49,50,51,52,54,55,56,57,58,60,61,62,63,64,66,67,68,69,70,72,73,74,75,76,77,79,80,81,82,83,85,86,87,88,89,91,92,93,94,95,97,98,99,100,101,103,104,105,106,107,109,110,111,112,113,114,116,117,118,119,120,122,123,124,125,126,128,129,130,131,132,134,135,136,137,138,140,141,142,143,144,145,147,148,149,150,151,153,154,155,156,157,159,160,161,162,163,165,166,167,168,169,171,172,173,174,175,177,178,179,180,181,182,184,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,4,5,7,8,9,10,11,13,14,15,16,18,19,20,21,22,24,25,26,27,28,30,31,32,33,35,36,37,38,39,41,42,43,44,46,47,48,49,50,52,53,54,55,56,58,59,60,61,63,64,65,66,67,69,70,71,72,74,75,76,77,78,80,81,82,83,84,86,87,88,89,91,92,93,94,95,97,98,99,100,102,103,104,105,106,108,109,110,111,112,114,115,116,117,119,120,121,122,123,125,126,127,128,130,131,132,133,134,136,137,138,139,140,142,143,144,145,147,148,149,150,151,153,154,155,156,158,159,160,161,162,164,165,166,167,168,170,171,172,173,175,176,177,178,179,181,182,183,184,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,7,8,10,11,12,13,15,16,17,18,20,21,22,23,25,26,27,28,30,31,32,33,35,36,37,38,40,41,42,43,45,46,47,48,49,51,52,53,54,56,57,58,59,61,62,63,64,66,67,68,69,71,72,73,74,76,77,78,79,81,82,83,84,86,87,88,89,91,92,93,94,95,97,98,99,100,102,103,104,105,107,108,109,110,112,113,114,115,117,118,119,120,122,123,124,125,127,128,129,130,132,133,134,135,137,138,139,140,141,143,144,145,146,148,149,150,151,153,154,155,156,158,159,160,161,163,164,165,166,168,169,170,171,173,174,175,176,178,179,180,181,183,184,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,4,5,7,8,9,10,12,13,14,16,17,18,19,21,22,23,24,26,27,28,29,31,32,33,35,36,37,38,40,41,42,43,45,46,47,49,50,51,52,54,55,56,57,59,60,61,63,64,65,66,68,69,70,71,73,74,75,76,78,79,80,82,83,84,85,87,88,89,90,92,93,94,96,97,98,99,101,102,103,104,106,107,108,110,111,112,113,115,116,117,118,120,121,122,123,125,126,127,129,130,131,132,134,135,136,137,139,140,141,143,144,145,146,148,149,150,151,153,154,155,157,158,159,160,162,163,164,165,167,168,169,170,172,173,174,176,177,178,179,181,182,183,184,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,7,9,10,11,13,14,15,16,18,19,20,22,23,24,25,27,28,29,31,32,33,35,36,37,38,40,41,42,44,45,46,48,49,50,51,53,54,55,57,58,59,61,62,63,64,66,67,68,70,71,72,74,75,76,77,79,80,81,83,84,85,87,88,89,90,92,93,94,96,97,98,99,101,102,103,105,106,107,109,110,111,112,114,115,116,118,119,120,122,123,124,125,127,128,129,131,132,133,135,136,137,138,140,141,142,144,145,146,148,149,150,151,153,154,155,157,158,159,161,162,163,164,166,167,168,170,171,172,173,175,176,177,179,180,181,183,184,185,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,5,7,8,9,11,12,13,15,16,17,19,20,21,23,24,25,27,28,29,31,32,33,35,36,37,39,40,41,43,44,45,47,48,49,51,52,53,54,56,57,58,60,61,62,64,65,66,68,69,70,72,73,74,76,77,78,80,81,82,84,85,86,88,89,90,92,93,94,96,97,98,100,101,102,104,105,106,108,109,110,112,113,114,116,117,118,120,121,122,124,125,126,128,129,130,132,133,134,135,137,138,139,141,142,143,145,146,147,149,150,151,153,154,155,157,158,159,161,162,163,165,166,167,169,170,171,173,174,175,177,178,179,181,182,183,185,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,7,9,10,12,13,14,16,17,18,20,21,22,24,25,26,28,29,31,32,33,35,36,37,39,40,41,43,44,45,47,48,50,51,52,54,55,56,58,59,60,62,63,64,66,67,69,70,71,73,74,75,77,78,79,81,82,83,85,86,88,89,90,92,93,94,96,97,98,100,101,103,104,105,107,108,109,111,112,113,115,116,117,119,120,122,123,124,126,127,128,130,131,132,134,135,136,138,139,141,142,143,145,146,147,149,150,151,153,154,155,157,158,160,161,162,164,165,166,168,169,170,172,173,174,176,177,179,180,181,183,184,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,5,7,8,10,11,12,14,15,17,18,19,21,22,23,25,26,28,29,30,32,33,35,36,37,39,40,42,43,44,46,47,49,50,51,53,54,55,57,58,60,61,62,64,65,67,68,69,71,72,74,75,76,78,79,80,82,83,85,86,87,89,90,92,93,94,96,97,99,100,101,103,104,106,107,108,110,111,112,114,115,117,118,119,121,122,124,125,126,128,129,131,132,133,135,136,137,139,140,142,143,144,146,147,149,150,151,153,154,156,157,158,160,161,163,164,165,167,168,169,171,172,174,175,176,178,179,181,182,183,185,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,5,6,8,9,10,12,13,15,16,18,19,20,22,23,25,26,27,29,30,32,33,35,36,37,39,40,42,43,45,46,47,49,50,52,53,55,56,57,59,60,62,63,65,66,67,69,70,72,73,74,76,77,79,80,82,83,84,86,87,89,90,92,93,94,96,97,99,100,102,103,104,106,107,109,110,112,113,114,116,117,119,120,121,123,124,126,127,129,130,131,133,134,136,137,139,140,141,143,144,146,147,149,150,151,153,154,156,157,159,160,161,163,164,166,167,168,170,171,173,174,176,177,178,180,181,183,184,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,5,7,8,10,11,13,14,16,17,19,20,21,23,24,26,27,29,30,32,33,35,36,38,39,40,42,43,45,46,48,49,51,52,54,55,57,58,59,61,62,64,65,67,68,70,71,73,74,75,77,78,80,81,83,84,86,87,89,90,92,93,94,96,97,99,100,102,103,105,106,108,109,111,112,113,115,116,118,119,121,122,124,125,127,128,129,131,132,134,135,137,138,140,141,143,144,146,147,148,150,151,153,154,156,157,159,160,162,163,165,166,167,169,170,172,173,175,176,178,179,181,182,183,185,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,5,6,8,9,11,12,14,15,17,18,20,21,23,24,26,27,29,30,32,33,35,36,38,39,41,42,44,45,47,48,50,51,53,54,56,57,59,60,62,63,65,66,68,69,71,72,74,75,77,78,80,81,83,84,86,87,89,90,92,93,94,96,97,99,100,102,103,105,106,108,109,111,112,114,115,117,118,120,121,123,124,126,127,129,130,132,133,135,136,138,139,141,142,144,145,147,148,150,151,153,154,156,157,159,160,162,163,165,166,168,169,171,172,174,175,177,178,180,181,183,184,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,5,7,9,10,12,13,15,16,18,19,21,22,24,25,27,29,30,32,33,35,36,38,39,41,42,44,45,47,48,50,52,53,55,56,58,59,61,62,64,65,67,68,70,72,73,75,76,78,79,81,82,84,85,87,88,90,91,93,95,96,98,99,101,102,104,105,107,108,110,111,113,114,116,118,119,121,122,124,125,127,128,130,131,133,134,136,138,139,141,142,144,145,147,148,150,151,153,154,156,157,159,161,162,164,165,167,168,170,171,173,174,176,177,179,181,182,184,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,5,6,8,9,11,13,14,16,17,19,20,22,24,25,27,28,30,32,33,35,36,38,39,41,43,44,46,47,49,50,52,54,55,57,58,60,61,63,65,66,68,69,71,73,74,76,77,79,80,82,84,85,87,88,90,91,93,95,96,98,99,101,102,104,106,107,109,110,112,113,115,117,118,120,121,123,125,126,128,129,131,132,134,136,137,139,140,142,143,145,147,148,150,151,153,154,156,158,159,161,162,164,166,167,169,170,172,173,175,177,178,180,181,183,184,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,6,7,9,10,12,14,15,17,18,20,22,23,25,27,28,30,31,33,35,36,38,40,41,43,44,46,48,49,51,52,54,56,57,59,61,62,64,65,67,69,70,72,74,75,77,78,80,82,83,85,87,88,90,91,93,95,96,98,99,101,103,104,106,108,109,111,112,114,116,117,119,121,122,124,125,127,129,130,132,134,135,137,138,140,142,143,145,146,148,150,151,153,155,156,158,159,161,163,164,166,168,169,171,172,174,176,177,179,180,182,184,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,5,6,8,10,11,13,15,16,18,20,21,23,25,26,28,30,31,33,35,36,38,40,41,43,45,46,48,50,51,53,55,56,58,60,61,63,65,66,68,70,71,73,75,76,78,80,81,83,85,86,88,90,91,93,95,96,98,100,101,103,105,106,108,110,111,113,115,116,118,120,121,123,125,126,128,130,131,133,135,136,138,140,141,143,145,146,148,150,151,153,155,156,158,160,161,163,165,166,168,170,171,173,175,176,178,180,181,183,185,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,7,9,11,12,14,16,18,19,21,23,24,26,28,30,31,33,35,36,38,40,42,43,45,47,48,50,52,54,55,57,59,60,62,64,66,67,69,71,72,74,76,78,79,81,83,84,86,88,90,91,93,95,96,98,100,102,103,105,107,108,110,112,114,115,117,119,120,122,124,126,127,129,131,132,134,136,138,139,141,143,144,146,148,150,151,153,155,156,158,160,162,163,165,167,168,170,172,174,175,177,179,180,182,184,186,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,5,6,8,10,12,14,15,17,19,21,22,24,26,28,29,31,33,35,36,38,40,42,44,45,47,49,51,52,54,56,58,59,61,63,65,67,68,70,72,74,75,77,79,81,82,84,86,88,89,91,93,95,97,98,100,102,104,105,107,109,111,112,114,116,118,119,121,123,125,127,128,130,132,134,135,137,139,141,142,144,146,148,150,151,153,155,157,158,160,162,164,165,167,169,171,172,174,176,178,180,181,183,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,7,9,11,13,15,17,18,20,22,24,26,27,29,31,33,35,37,38,40,42,44,46,47,49,51,53,55,57,58,60,62,64,66,68,69,71,73,75,77,78,80,82,84,86,88,89,91,93,95,97,98,100,102,104,106,108,109,111,113,115,117,118,120,122,124,126,128,129,131,133,135,137,139,140,142,144,146,148,149,151,153,155,157,159,160,162,164,166,168,169,171,173,175,177,179,180,182,184,186,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,5,7,8,10,12,14,16,18,20,22,23,25,27,29,31,33,35,37,38,40,42,44,46,48,50,52,54,55,57,59,61,63,65,67,69,70,72,74,76,78,80,82,84,85,87,89,91,93,95,97,99,101,102,104,106,108,110,112,114,116,117,119,121,123,125,127,129,131,132,134,136,138,140,142,144,146,148,149,151,153,155,157,159,161,163,164,166,168,170,172,174,176,178,179,181,183,185,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};

const unsigned char Array_forward_bird_row[22560]=
{
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,
27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,
28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,
29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,
30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,
31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,
33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,
34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,
35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,
36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,
37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,
38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,
42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,
43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,
44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,
45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,
46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,
48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,
49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,0,0,
50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,0,0,0,
52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,0,0,0,0,0,
53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,0,0,0,0,0,0,
54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,0,0,0,0,0,0,0,0,
0,0,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,0,0,0,0,0,0,0,0,0,0,
0,0,0,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};





