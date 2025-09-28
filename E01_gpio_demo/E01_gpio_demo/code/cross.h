/*
 * cross.h
 *
 *  Created on: 2024年6月5日
 *      Author: admin
 */

#ifndef CROSS_H_
#define CROSS_H_

enum cross_type_e{
    CROSS_NONE = 0,     // 非十字模式
    CROSS_BEGIN,        // 找到左右两个L角点
    CROSS_IN,           // 两个L角点很近，即进入十字内部(此时切换远线控制)
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
