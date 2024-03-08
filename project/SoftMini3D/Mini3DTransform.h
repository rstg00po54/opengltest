#ifndef __Mini3DTransform__
#define __Mini3DTransform__
#include "Mini3DMath.h"

typedef struct { 
	matrix_t world;         // 世界坐标变换
	matrix_t view;          // 摄影机坐标变换
	matrix_t projection;    // 投影变换
	matrix_t transform;     // transform = world * view * projection
	float w, h;             // 屏幕大小
}	transform_t;


void transform_update(transform_t *ts);
void transform_init(transform_t *ts, int width, int height);
void transform_apply(const transform_t *ts, vector_t *y, const vector_t *x);
int transform_check_cvv(const vector_t *v);
void transform_homogenize(const transform_t *ts, vector_t *y, const vector_t *x);
#endif
