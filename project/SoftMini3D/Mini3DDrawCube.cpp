#include "Mini3DDrawCube.h"
#include "Mini3DRender.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

vertex_t mesh[] = {
	//  point_t pos; texcoord_t tc; color_t color; float rhw;
	{ { -1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ {  1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ {  1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { -1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	{ { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	{ {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
	{ {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
	{ { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	{ {  0,  0,  0, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
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

	quat->x = ca*cb*cy+sa*sb*sy;
	quat->y = sa*cb*cy-ca*sb*sy;
	quat->z = ca*sb*cy+sa*cb*sy;
	quat->w = ca*cb*sy-sa*sb*cy;
}
static vector_t garc;
static vector_t garc0;
static point_t point[3][3];
int testg;
void draw_cube(device_t *device) {

	transform_t *t = &device->transform;
	point_t trans = {0, 0, 0};
	vector_t quat;
	int i,j,k;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 diffTime = 0;
	float curDrgee = 0.f;


	static int key = 99;
	static Uint32 downTime = 0;
	// printf("garc0 %3.2f\n", garc0.x);
	// garc0.x = 2.f;
	// printf("tesg %d\n", testg);
	// testg = 21;

	ImGui::Begin("cube");
	if(ImGui::Button("旋转D0\n")) {
		// printf("time %d\n", currentTime);
		downTime = currentTime;
		// printf("key = %d\n", key);
		if(key == -1)
			key = 99;
		else
			key = -1;
		// printf("key = %d\n", key);
	}
	if(ImGui::Button("旋转D1\n")) {
		printf("time %d\n", currentTime);
		downTime = currentTime;
		if(key == 0)
			key = 99;
		else
			key = 0;
	}
	if(ImGui::Button("旋转D2\n")) {
		printf("time %d\n", currentTime);
		downTime = currentTime;
		if(key == 1)
			key = 99;
		else
			key = 1;
	}
	ImGui::SliderFloat4("cuberota", (float *)&garc0, 0.0f, 360.0f);



	// if(key) {
	// 	diffTime = currentTime - downTime;
	// 	float drgee = 0.05f*diffTime;
	// 	// printf("diffTime %d, drgee %3.2f\n", diffTime, drgee);
	// 	if(drgee < 90.f){
	// 		// garc.y = drgee;
	// 		curDrgee = garc.y+drgee;
	// 	}else{
	// 		garc.y += 90.f;
	// 		key = 0;
	// 		curDrgee = garc.y;
	// 		printf("key %3.2f\n", garc.y);
	// 	}
	// }else{
	// 	curDrgee = garc.y;
	// }

	// matrix_set_rotate(&t->rotate, 0, 1, 0, garc0.x);
	static point_t ro = {0.f, 0.f, 0.f,0.f};
	point_t ro0 = {0.f, 0.f, 0.f,0.f};

	// oltoquat(&quat, garc0);
	// matrix_set_Quaternion(&t->rotate, quat);


	matrix_set_translate(&t->trans, 1, 1, 1);
	transform_update(t);
	draw_box(device);	

	// printf("----\n");
	// for(i = -1;i<2;i++){
	// 	for(j=-1;j<2;j++){
	// 		for(k=-1;k<2;k++){
	// 			if(i == key){
	// 				ro = garc0;
	// 			}else{
	// 				ro = ro0;
	// 			}
	// 			oltoquat(&quat, ro);
	// 			matrix_set_Quaternion(&t->rotate, quat);
	// 			matrix_set_translate(&t->trans, trans.x+i, trans.y+j, trans.z+k);
	// 			transform_update(t);
	// 			// point[i+1][j+1] = draw_box(device);	
	// 			draw_box(device);
	// 			// ImGui::SliderFloat4("cube", (float *)&point[i+1][j+1], 0.0f, 360.0f);
	// 		}
	// 	}
	// }
	ImGui::End();
}