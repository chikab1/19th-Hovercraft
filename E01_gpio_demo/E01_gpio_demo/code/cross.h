/*
 * cross.h
 *
 *  Created on: 2024��6��5��
 *      Author: admin
 */

#ifndef CROSS_H_
#define CROSS_H_

enum cross_type_e{
    CROSS_NONE = 0,     // ��ʮ��ģʽ
    CROSS_BEGIN,        // �ҵ���������L�ǵ�
    CROSS_IN,           // ����L�ǵ�ܽ���������ʮ���ڲ�(��ʱ�л�Զ�߿���)
    CROSS_NUM,
};
enum track_type_e {
    TRACK_LEFT,
    TRACK_RIGHT,
};


void check_cross(void);
void run_cross(void);
void cross_farline(void);


#endif /* CROSS_H_ */
