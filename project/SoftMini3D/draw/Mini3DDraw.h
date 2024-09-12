
#ifndef __Mini3D_Draw__
#define __Mini3D_Draw__

#include <Mini3DDevice.h>

point_t transform_home0(const device_t *device, const vector_t *from, const vector_t *to);
void drawLine(device_t *device, point_t p0, point_t p1, int c = 0);

#endif