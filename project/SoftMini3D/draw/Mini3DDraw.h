
#ifndef __Mini3D_Draw__
#define __Mini3D_Draw__

#include <Mini3DDevice.h>

#define OUT_CAMERA -1
#define OUT_CAMERA2 -3
#define OUT_SCREEN -2
/*
MVP之前：localPosition / worldPosition
MVP之后（剪裁坐标系）：clipPosition / projectedPosition
归一化之后（规范化设备坐标系）：ndcPosition / normalizedPosition
*/
typedef struct {
    float t[2];
    int ret;
    point_t localPosition[2];
    point_t clipPosition[2];
    point_t normalPosition[2];
    point_t screenPosition[2];
}VertDraw;
point_t transform_home0(const device_t *device, const vector_t *from, const vector_t *to);
VertDraw drawLine(device_t *device, point_t p0, point_t p1, int c = 0);


#endif