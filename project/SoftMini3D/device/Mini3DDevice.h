#ifndef __Mini3D_Device__
#define __Mini3D_Device__
#include "Mini3D.h"
#include "Mini3DGeometryCalc.h"
void device_init(device_t *device, int width, int height, void *fb);
void device_clear(device_t *device, int mode);
void device_pixel(device_t *device, int x, int y, IUINT32 color);
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);
void device_draw_scanline(device_t *device, scanline_t *scanline);
IUINT32 device_texture_read(const device_t *device, float u, float v);
#endif