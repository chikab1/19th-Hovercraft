/*
 * circle.h
 *
 *  Created on: 2024��6��5��
 *      Author: admin
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

enum circle_type_e {
    CIRCLE_NONE = 0,                            // ��Բ��ģʽ
    CIRCLE_LEFT_BEGIN, CIRCLE_RIGHT_BEGIN,      // Բ����ʼ��ʶ�𵽵���L�ǵ���һ�೤ֱ����
    CIRCLE_LEFT_IN, CIRCLE_RIGHT_IN,            // Բ�����룬���ߵ�һ��ֱ����һ��Բ����λ�á�
    CIRCLE_LEFT_RUNNING, CIRCLE_RIGHT_RUNNING,  // Բ���ڲ���
    CIRCLE_LEFT_OUT, CIRCLE_RIGHT_OUT,          // ׼����Բ������ʶ�𵽳�������L�ǵ㡣
    CIRCLE_LEFT_END, CIRCLE_RIGHT_END,          // Բ�����������ٴ��ߵ�����ֱ����λ�á�
    CIRCLE_NUM,                                 //
};

void check_circle(void);
void run_circle(void);

#endif /* CIRCLE_H_ */
