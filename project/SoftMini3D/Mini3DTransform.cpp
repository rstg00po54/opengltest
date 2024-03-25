
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
/*
Vworld=Mprojection⋅Mview⋅Mmodel⋅Vlocal
  = 投影矩阵*视图矩阵*模型矩阵*本地坐标
  模型矩阵将对象从本地坐标系转换到世界坐标系。它包括了平移、旋转和缩放等变换，以确定对象在世界空间中的位置、方向和大小。
  视图矩阵将世界坐标系中的对象转换到观察者（相机）的视图空间。它包含了相机的位置、方向以及视角等信息，使得对象能够在相机视角下正确显示。
  投影矩阵将视图空间中的对象投影到屏幕空间，完成透视变换或正交投影，使得对象最终能够正确显示在屏幕上。
世界矩阵是模型矩阵和视图矩阵的乘积
	matrix_t world;         // 世界坐标变换
	matrix_t view;          // 摄影机坐标变换
	matrix_t projection;    // 投影变换
	matrix_t transform;     // transform = world * view * projection
*/
void transform_update(transform_t *ts) {
	matrix_t m;
	matrix_mul(&m, &ts->view, &ts->model);
	// matrix_mul(&ts->transform, &ts->projection, &m);

	// matrix_mul(&m, &ts->model, &ts->view);
	matrix_mul(&ts->transform, &ts->projection, &m);
	// matrix_mul(&ts->transform, &m, &ts->projection);

	// matrix_mul(&ts->transform, &ts->view, &ts->model);
	// matrix_mul(&ts->transform, &ts->projection, &m);

	// matrix_mul(&m, &ts->view, &ts->model);
	// matrix_mul(&ts->transform, &ts->projection, &m);
	// PRINT_M(ts->world);
	// PRINT_M(ts->view);
	// PRINT_M(ts->projection);
	// PRINT_M(ts->transform);
}

// 初始化，设置屏幕长宽
void transform_init(transform_t *ts, int width, int height) {
	// float aspect = (float)width / ((float)height);
	matrix_set_identity(&ts->model);
	matrix_set_identity(&ts->view);
	// matrix_set_perspective(&ts->projection, 45, aspect, 1.0f, 100.0f);
	ts->w = (float)width;
	ts->h = (float)height;
	// transform_update(ts);
}

// 将矢量 x 进行 project 
void transform_apply(const transform_t *ts, vector_t *y, const vector_t *x) {
	matrix_apply(y, &ts->transform, x);
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

