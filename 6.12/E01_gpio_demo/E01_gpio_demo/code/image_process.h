/*
 * image_process.h
 *
 *  Created on: 2024Äê6ÔÂ4ÈÕ
 *      Author: admin
 */

#ifndef IMAGE_PROCESS_H_
#define IMAGE_PROCESS_H_

void clear_IMG(void);
void get_image(void);
int8 limit(int8 x,int8 max,int8 min);
int MIN(int x,int min);
void find_leftline(void);
void find_farleftline(void);
void find_rightline(void);
void find_farrightline(void);
void image_Perspective_Correction(void);
void blur_points(void);
void far_blur_points(void);
void resample_points(void);
void far_resample_points(void);
void local_angle_points(void);
void far_local_angle_points(void);
void nms_angle(void);
void far_nms_angle(void);
void track_leftline(void);
void track_rightline(void);
void find_corners(void);

#endif /* IMAGE_PROCESS_H_ */
