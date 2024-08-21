#include "Mini3DDrawPanel.h"
static vertex_t mesh[] = {
	//  point_t pos; texcoord_t tc; color_t color; float rhw;
	{ { -5, -5,  5, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ {  5, -5,  5, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ {  5, -5, -5, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { -5, -5, -5, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	// { { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	// { {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
	// { {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
	// { { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	// { {  0,  0,  0, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};
static void draw_plane(device_t *device, int a, int b, int c, int d, int count) {
	vertex_t p1 = mesh[a];
	vertex_t p2 = mesh[b];
	vertex_t p3 = mesh[c];
	vertex_t p4 = mesh[d];
	float h = 1.f;
	/*
	v
	|
	p2-p3
	|  |
	p1-p4-u

	p01 p11
	p00 p10

	*/
	float u[2],v[2];

	u[0] = 0;
	u[1] = 1.f;
	v[0] = 0;
	v[1] = 1.f;

	p1.tc.u = u[0];
	p1.tc.v = v[0];

	p2.tc.u = u[0];
	p2.tc.v = v[1];

	p3.tc.u = u[1];
	p3.tc.v = v[1];

	p4.tc.u = u[1];
	p4.tc.v = v[0];
	device_draw_triangle(device, &p1, &p2, &p3);
	// device_draw_triangle(device, &p3, &p4, &p1);
}
void draw_panel(device_t *device) {
    device->module = 1;
    matrix_set_translate(&device->transform.trans, 0, 0, 0);
    transform_update(&device->transform);
    draw_plane(device, 0, 1, 2, 3, 0);

	// device->module = 2;
    // matrix_set_translate(&device->transform.trans, 0, -1, 0);
    // transform_update(&device->transform);
    // draw_plane(device, 0, 1, 2, 3, 0);
}