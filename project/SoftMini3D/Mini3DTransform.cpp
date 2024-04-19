
#include "Mini3DTransform.h"
//=====================================================================
// 坐标变换
//=====================================================================
// Vclip=Projection⋅View⋅Model⋅Local

#include <stdio.h>

#define PRINT_M(ts) \
	printf(#ts"\n");\
	for(int i = 0;i<4;i++){\
		printf("%f, %f, %f, %f\n",\
			ts.m[i][0],\
			ts.m[i][1],\
			ts.m[i][2],\
			ts.m[i][3]);\
	}\

// 矩阵更新，计算 transform = world * view * projection
// Vclip=projection*view*model*local
void transform_update(transform_t *ts) {
	matrix_t m;
	matrix_mul(&m, &ts->view, &ts->world);
	matrix_mul(&ts->transform, &ts->projection, &m);

	// PRINT_M(ts->world);
	// PRINT_M(ts->view);
	// PRINT_M(ts->projection);
	// PRINT_M(ts->transform);
}

// 初始化，设置屏幕长宽
void transform_init(transform_t *ts, int width, int height) {
	float aspect = (float)width / ((float)height);
	matrix_set_identity(&ts->world);
	matrix_set_identity(&ts->view);
	matrix_set_perspective(&ts->projection, 45, aspect, 1.0f, 100.0f);
	ts->w = (float)width;
	ts->h = (float)height;
	transform_update(ts);
}

// 将矢量 x 进行 project 
void transform_apply(const transform_t *ts, vector_t *y, const vector_t *x) {
	matrix_apply(y, x, &ts->transform);
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int transform_check_cvv(const vector_t *v) {
	float w = v->w;
	int check = 0;
	if (v->z < 0.0f) check |= 1;
	if (v->z >  w) check |= 2;
	if (v->x < -w) check |= 4;
	if (v->x >  w) check |= 8;
	if (v->y < -w) check |= 16;
	if (v->y >  w) check |= 32;
	return check;
}

// 归一化，得到屏幕坐标
void transform_homogenize(const transform_t *ts, vector_t *y, const vector_t *x) {
	float rhw = 1.0f / x->w;
	y->x = (x->x * rhw + 1.0f) * ts->w * 0.5f;
	y->y = (1.0f - x->y * rhw) * ts->h * 0.5f;
	y->z = x->z * rhw;
	y->w = 1.0f;
}

/*
0.75 1 3.03 4
 0 - 800	x'
-1 - 1		x
x' = (x*(1/w)+1)*800*0.5

 0 - 600	y'
-1 - 1 		y
y' = (y*(1/w)+1)*600*0.5
*/
