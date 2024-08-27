#include "Mini3DDrawCube.h"
#include "Mini3DRender.h"
#include "Mini3DMath.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <iostream>

vertex_t mesh[] = {
	//  point_t pos; texcoord_t tc; color_t color; float rhw;
	{ { -1, -1,  1, 1 }, { -1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ {  1, -1,  1, 1 }, {  1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ {  1,  1,  1, 1 }, {  1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { -1,  1,  1, 1 }, { -1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	{ { -1, -1, -1, 1 }, { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	{ {  1, -1, -1, 1 }, {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
	{ {  1,  1, -1, 1 }, {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
	{ { -1,  1, -1, 1 }, { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	{ {  0,  0,  0, 1 }, {  0,  0,  0, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};

void draw_plane(device_t *device, int a, int b, int c, int d, int count) {
	vertex_t p1 = mesh[a];
	vertex_t p2 = mesh[b];
	vertex_t p3 = mesh[c];
	vertex_t p4 = mesh[d];
	float h = 1.f/6;
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
	u[1] = 1.f/3;
	v[0] = count*h;
	v[1] = count*h+1.f/18;

	p1.tc.u = u[0];
	p1.tc.v = v[0];

	p2.tc.u = u[0];
	p2.tc.v = v[1];

	p3.tc.u = u[1];
	p3.tc.v = v[1];

	p4.tc.u = u[1];
	p4.tc.v = v[0];
	device_draw_triangle(device, &p1, &p2, &p3);
	device_draw_triangle(device, &p3, &p4, &p1);
}
// #define PRINT_M(ts) \
// 	printf(#ts"\n");\
// 	for(int i = 0;i<4;i++){\
// 		printf("%f, %f, %f, %f\n",\
// 			ts.m[i][0],\
// 			ts.m[i][1],\
// 			ts.m[i][2],\
// 			ts.m[i][3]);\
// 	}
point_t draw_box(device_t *device) {
	// pr_debug("in");
	matrix_t m;
	point_t p = {0,0,0,1}, p2;

	matrix_apply(&p2, &p, &device->transform.model);
	// ImGui::SliderFloat3("正方体中心", (float *)&p2, -3.0f, 3.0f);
	draw_plane(device, 0, 1, 2, 3, 4);
	draw_plane(device, 7, 6, 5, 4, 5);
	draw_plane(device, 0, 4, 5, 1, 2);
	draw_plane(device, 1, 5, 6, 2, 1);
	draw_plane(device, 2, 6, 7, 3, 3);
	draw_plane(device, 3, 7, 4, 0, 0);
	return p2;
}
// x->y->z
static void oltoquat(vector_t *quat, vector_t ol){
	float alpha = ol.x * (M_PI / 180);
	float beta =  ol.y * (M_PI / 180);
	float gama =  ol.z * (M_PI / 180);

	float ca = cosf(alpha/2);
	float cb = cosf(beta/2);
	float cy = cosf(gama/2);
	float sa = sinf(alpha/2);
	float sb = sinf(beta/2);
	float sy = sinf(gama/2);

	quat->w = ca*cb*cy+sa*sb*sy;
	quat->x = sa*cb*cy-ca*sb*sy;
	quat->y = ca*sb*cy+sa*cb*sy;
	quat->z = ca*cb*sy-sa*sb*cy;
}
/*

q = (cos a/2, u*sin a/2)
w = qvq*
w+xi+yj+zk或者(w,(x,y,z))
q* = (w, -v1)
q1q2 = (w1w2-v1v2, w1v2+w2v1+v1xv2)
1,(0,0,0) 不旋转

// 矢量点乘
float vector_dotproduct(const vector_t *x, const vector_t *y) 

// 矢量叉乘
void vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y)

void vector_add(vector_t *z, const vector_t *x, const vector_t *y);
void vector_sub(vector_t *z, const vector_t *x, const vector_t *y);
*/
static point_t quat_mux(point_t p1, point_t p2) {
	point_t out,v,v1;
	point_t w1v2,w2v1;
	point_t v1xv2;
	// q1q2 = (w1w2-v1v2, w1v2+w2v1+v1xv2)
	// (w,(x,y,z))
	w1v2 = vector_dotfloat(&p2,&p1.w);
	w2v1 = vector_dotfloat(&p1,&p2.w);
	vector_crossproduct(&v1xv2, &p1, &p2);
	vector_add(&v, &w1v2, &w2v1);
	vector_add(&out, &v, &v1xv2);

	out.w = p1.w*p2.w-vector_dotproduct(&p1, &p2);
	return out;

}
static point_t quat_rotate(float x, float y, float z, point_t src, float angle) {
	float alpha = angle * (M_PI / 180);
	float cosa = cosf(alpha/2);
	float sina = sinf(alpha/2);
	point_t quat0,quat1;

	// point_t src = {x, y, z, 1.f};
	quat0.w = cosa;
	quat0.x = x*sina;
	quat0.y = y*sina;
	quat0.z = z*sina;
	quat1.w = cosa;
	quat1.x = -x*sina;
	quat1.y = -y*sina;
	quat1.z = -z*sina;

	point_t v = quat_mux(quat0, src);
	point_t o = quat_mux(v, quat1);
	return o;
}


static void rotate_x(matrix_t *m, matrix_t *s,float angle){
	matrix_t r;
	float alpha = angle * (M_PI / 180);
	matrix_set_identity(&r);
	r.m[1][1] = cosf(alpha);
	r.m[1][2] = -sinf(alpha);
	r.m[2][1] = sinf(alpha);
	r.m[2][2] = cosf(alpha);
	matrix_mul(m, &r, s);
}
static void rotate_y(matrix_t *m, matrix_t *s,float angle){
	matrix_t r;
	float alpha = angle * (M_PI / 180);
	matrix_set_identity(&r);
	r.m[0][0] = cosf(alpha);
	r.m[0][2] = sinf(alpha);
	r.m[2][0] = -sinf(alpha);
	r.m[2][2] = cosf(alpha);
	matrix_mul(m, &r, s);
}
static void rotate_z(matrix_t *m, matrix_t *s,float angle){
	matrix_t r;
	float alpha = angle * (M_PI / 180);
	matrix_set_identity(&r);
	r.m[0][0] = cosf(alpha);
	r.m[0][1] = -sinf(alpha);
	r.m[1][0] = sinf(alpha);
	r.m[1][1] = cosf(alpha);
	matrix_mul(m, &r, s);
}

class Cube{	
public:
	vector_t position;
	vector_t src_pos;
	vector_t angle;
	matrix_t rotate;
};
Cube c[3][3][3];
static matrix_t matrix_rotate;
void init_cube() {
	int i,j,k;
	for(i = 0;i<3;i++){
		for(j=0;j<3;j++){
			for(k=0;k<3;k++){
				Cube *c0 = &c[i][j][k];
				c0->position.x = i-1;
				c0->position.y = j-1;
				c0->position.z = k-1;
				c0->src_pos.x = i-1;
				c0->src_pos.y = j-1;
				c0->src_pos.z = k-1;
				matrix_set_identity(&c0->rotate);
			}
		}
	}
}
void draw_cube(device_t *device) {

	transform_t *t = &device->transform;
	point_t trans = {0, 0, 0};
	vector_t quat = {0,0,0,1};
	int i,j,k;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 diffTime = 0;
	float curDrgee = 0.f;


	static unsigned char key = 255;
	static Uint32 downTime = 0;
	// printf("garc0 %3.2f\n", garc0.x);
	// garc0.x = 2.f;
	// printf("tesg %d\n", testg);
	// testg = 21;
	device->module = 0;

	ImGui::Begin("cube");
	if(ImGui::Button("U\n")) {
		// printf("time %d\n", currentTime);
		downTime = currentTime;
		// printf("key = %d\n", key);
		if(key == 255)
			key = 'U';
		else
			key = 255;
		// printf("key = %d\n", key);
	}
	ImGui::SameLine();
	if(ImGui::Button("F\n")) {
		printf("time %d\n", currentTime);
		downTime = currentTime;
		if(key == 255)
			key = 'F';
		else
			key = 255;
	}
	ImGui::SameLine();
	if(ImGui::Button("R\n")) {
		printf("time %d\n", currentTime);
		downTime = currentTime;
		if(key == 255)
			key = 'R';
		else
			key = 255;
	}
	ImGui::SameLine();
	if(ImGui::Button("T\n")) {
		printf("time %d\n", currentTime);
		downTime = currentTime;
		if(key == 255)
			key = 'T';
		else
			key = 255;
	}


	static int value[10] = {0};
	ImGui::SliderInt4("int", value, 0, 5);

	static point_t spoint  = {0.f, 0.f, 0.f,0.f};

	if(key == 'U'){
		printf("key U\n");
		spoint.x += 90.f;
	}
	if(key == 'F'){
		printf("key F\n");
		spoint.y += 90.f;
	}
	if(key == 'R'){
		printf("key R\n");
		spoint.z += 90.f;
	}
#if 0
	oltoquat(&quat, spoint);
	matrix_set_Quaternion(&t->rotate, quat);
	matrix_set_translate(&t->trans, 1, 1, 1);
	transform_update(t);
	draw_box(device);	

	// printf("----\n");
#else
	static vector_int ro = {0};
	int count = 0;
	point_t ro0 = {0.f, 0.f, 0.f,0.f};

	point_t p;
	for(i = 0;i<3;i++){
		for(j = 0;j<3;j++){
			for(k = 0;k<3;k++){
				// c[i+1][j+1][k+1].position.x = i+1;
				// c[i+1][j+1][k+1].position.y = j+1;
				// c[i+1][j+1][k+1].position.z = k+1;
				Cube *cube;
				// cube= &c[0][0][2];
				// printf("U %3.2f/%3.2f/%3.2f\n", cube->position.x, cube->position.y, cube->position.z);
				cube= &c[i][j][k];
				if(key != 255){
					printf("=========================================\n");
					printf("U src %3.2f/%3.2f/%3.2f\n", cube->src_pos.x, cube->src_pos.y, cube->src_pos.z);
					printf("U now %3.2f/%3.2f/%3.2f\n", cube->position.x, cube->position.y, cube->position.z);
					printf("rotate %3.2f/%3.2f/%3.2f\n", cube->angle.x, cube->angle.y, cube->angle.z);
				}


				static float rx = 0.f;
				matrix_t m;
				matrix_set_identity(&m);
				// matrix_rotate
				if(key == 'U'){
					if(cube->position.y > 0.5f){
						rotate_y(&m, &cube->rotate, 90.f);
						cube->rotate = m;
					}
				}
				if(key == 'F'){
					if(cube->position.z > 0.5f){
						rotate_z(&m, &cube->rotate, 90.f);
						cube->rotate = m;
					}
				}
				t->rotate = cube->rotate;
				// oltoquat(&quat, cube->angle);
				// matrix_set_Quaternion(&t->rotate, quat);

				matrix_set_translate(&t->trans, cube->src_pos.x, cube->src_pos.y, cube->src_pos.z);
				transform_update(t);
				p = draw_box(device);
				// SDL_Delay(500);
				if(key != 255){
					printf("++ %f/%f/%f\n", p.x, p.y, p.z);
					cube->position = p;
				}
			}
		}
	}
#endif
	key = 255;
	ImGui::End();
}