#ifndef __Mini3D_Render__
#define __Mini3D_Render__
#include "Mini3D.h"
#include "Mini3DGeometryCalc.h"
#include "Mini3DDraw.h"
#include <string>
#include <vector>
using namespace std;

void device_draw_triangle(device_t *device,
	const vertex_t *v1,
	const vertex_t *v2,
	const vertex_t *v3) ;
void device_render_trap(device_t *device, trapezoid_t *trap);
vertex_t interp_spos(vertex_t v1, vertex_t v2, float t);

// 定义一个结构体来表示点
struct vec2 {
	float x, y;
	string name;
	vec2(float x = 0, float y = 0, string name = "def") : x(x), y(y), name(name) {}
};
vector<point_t> Hodgmanmain(point_t min, point_t max, vector<point_t> subjectPolygon);
vector<vertex_t> Hodgmanmain(device_t *device, VertDraw vt1, VertDraw vt2, VertDraw vt3, vertex_t v1, vertex_t v2, vertex_t v3);
void drawCharAt(device_t *device, int xpos, int ypos, std::string c);
void drawCharAt(device_t *device, point_t p, std::string c);
#endif