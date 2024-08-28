#ifndef __Mini3D_Render__
#define __Mini3D_Render__
#include "Mini3D.h"
#include "Mini3DGeometryCalc.h"

void device_draw_triangle(device_t *device,
	const vertex_t *v1,
	const vertex_t *v2,
	const vertex_t *v3) ;
void device_render_trap(device_t *device, trapezoid_t *trap);
vertex_t interp_spos(vertex_t v1, vertex_t v2, float t);
#endif