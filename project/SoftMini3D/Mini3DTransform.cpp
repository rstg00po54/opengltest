
#include "Mini3DTransform.h"
//=====================================================================
// 坐标变换
//=====================================================================
// Vclip=Projection⋅View⋅Model⋅Local

#include <stdio.h>
#include <math.h>
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


	matrix_mul(&m, &ts->rotate, &ts->scale);
	matrix_mul(&ts->model, &ts->trans, &m);

	matrix_mul(&m, &ts->view, &ts->model);
	matrix_mul(&ts->mvp, &ts->projection, &m);

	// PRINT_M(ts->world);
	// PRINT_M(ts->view);
	// PRINT_M(ts->projection);
	// PRINT_M(ts->transform);
}

// 初始化，设置屏幕长宽
void transform_init(transform_t *ts, int width, int height) {
	float aspect = (float)width / ((float)height);
	matrix_set_identity(&ts->model);
	matrix_set_identity(&ts->view);
	
	ts->w = (float)width;
	ts->h = (float)height;
	transform_update(ts);
}

// 将矢量 x 进行 project 
void transform_apply(const transform_t *ts, vector_t *y, const vector_t *x) {
	matrix_apply(y, x, &ts->mvp);
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


/*
0.75 1 3.03 4
 0 - 800	x'
-1 - 1		x
x' = (x*(1/w)+1)*800*0.5

 0 - 600	y'
-1 - 1 		y
y' = (y*(1/w)+1)*600*0.5
*/

/*
p0 	3.932 -0.733 0.871 -0.129
p1 -3.932 0.733 7.189 6.189
	-11829.1 -1409.31 -6.775 1.000
	145.858 264.479 1.162 1.000
	((-3.932/0.129) +1) * 800 * 0.5
*/
// 归一化，得到屏幕坐标
void transform_homogenize(const transform_t *ts, vector_t *y, const vector_t *x) {
	float rhw = 1.0f / x->w;
	y->x = (x->x * rhw + 1.0f) * ts->w * 0.5f;
	y->y = (1.0f - x->y * rhw) * ts->h * 0.5f;
	y->z = x->z * rhw;
	y->w = 1.0f;
}
#define PRINTF_POINT(c1) printf(#c1);printf(":% 3.2f % 3.2f % 3.2f % 3.2f\n", c1.x, c1.y, c1.z, c1.w);
// #define PRINTF_POINT(c1)
/*
p0 	3.932 -0.733 0.871 -0.129
	-30.48	5.72	6.75

p1 -3.932 0.733 7.189 6.189
y = kx+m

x' = (x*(1/w)+1)*800*0.5

y' = (y*(1/w)+1)*600*0.5
*/
void transform_home(const transform_t *ts, vector_t *pout0, vector_t *pout1, vector_t *pin0, vector_t *pin1) {
	float k = 0;
	float m = 0;
	vector_t out0,out1;

	float rhw0 = 1.0f /abs(pin0->w);
	float rhw1 = 1.0f /abs(pin1->w);
// PRINTF_POINT((*pin0));
// PRINTF_POINT((*pin1));

	pin0->x *= rhw0;
	pin0->y *= rhw0;
	pin0->z *= rhw0;
	pin0->w *= rhw0;

	pin1->x *= rhw1;
	pin1->y *= rhw1;
	pin1->z *= rhw1;
	pin1->w *= rhw1;
// PRINTF_POINT((*pin0));
// PRINTF_POINT((*pin1));
	// if(pin0->z == pin1->z) {
	// 	printf("%f/%f =\n", pin0->z, pin1->z);
	// }else if(abs(pin0->z) > 1.f) {
	// 	/*
	// 	pin0-pout0-pout1-pin1

		
	// 	*/
	// 	// x
	// 	float kx = (pin1->x - pin0->x)/(pin1->z - pin0->z);
	// 	float x0 = (-1-pin1->z)*k;
	// 	// y
	// 	float ky = (pin1->y - pin0->y)/(pin1->z - pin0->z);
	// 	float y0 = (-1-pin1->z)*k;
	// 	pout0->x = x0;
	// 	pout0->y = y0;
	// 	pout0->z = -1;
	// 	printf("pin0->z %f, pout %f/%f/%f\n", pin0->z, pout0->x, pout0->y, pout0->z);
	// 	// printf("pin1->z %f, pout %f/%f/%f\n", pin1->z, pout1->x, pout1->y, pout1->z);
	// }

	out0 = *pin0;
	out1 = *pin1;

	if(pin0->x == pin1->x) {
		if(pin0->y>1) {
			out0.y = 1;
		}
		if(pin0->y<-1) {
			out0.y = -1;
		}
		if(pin1->y>1) {
			out1.y = 1;
		}
		if(pin1->y<-1) {
			out1.y = -1;
		}
		// printf("%f/%f\n", pout0->x, pout0->y);
		// printf("%f/%f\n", pout1->x, pout1->y);
		// return;
	}else if( (abs(pin0->y) < 1.0f)&&
				(abs(pin0->x) < 1.0f)&&
				(abs(pin1->x) < 1.0f)&&
				(abs(pin1->y) < 1.0f)){

	}else{
		k = (pin0->y-pin1->y)/(pin0->x-pin1->x);
		m = (pin0->y - k * pin0->x);
		if((abs(pin0->x) > 1.0f) ) {
			if(pin0->x > 1.0f) {
				out0.x = 1.f;
				out0.y = k*1.f+m;
			}else{
				out0.x = -1.f;
				out0.y = k*-1.f+m;
			}
		}
		if((abs(pin0->y) > 1.0f) ) {
			if(pin0->y > 1.0f) {
				out0.x = (1.f - m)/k;
				out0.y = 1.f;
			}else{
				out0.x = (-1.f - m)/k;
				out0.y = -1.f;
			}
		}

		if((abs(pin1->x) > 1.0f) ){
			if(pin1->x > 1.0f) {
				out1.x = 1.f;
				out1.y = k*1.f+m;
			}else{
				out1.x = -1.f;
				out1.y = k*-1.f+m;
			}
		}
		if((abs(pin1->y) > 1.0f) ){
			if(pin1->y > 1.0f) {
				out1.x = (1.f - m)/k;
				out1.y = 1.f;
			}else{
				out1.x = (-1.f - m)/k;
				out1.y = -1.f;
			}
		}
		if( (abs(pin0->y) > 1.0f)||
				(abs(pin0->x) > 1.0f)||
				(abs(pin1->x) > 1.0f)||
				(abs(pin1->y) > 1.0f)
		){
			if(rhw0 < 0) {
				if(k>0){
					out0.x = 1.f;//1 / -1
					out0.y = 1.f*k+m;
				}else{
					out0.x = -1.f;//1 / -1
					out0.y = -1.f*k+m;
				}
			}
			if(rhw1 < 0) {
				out1.x = -1.f;
				out1.y = -1.f*k+m;
			}
		}
		else{
				out0.x = pin1->x+(pin1->x-pin0->x);
				out0.y = pin1->y+(pin1->y-pin0->y);
		}
	}
// if((abs(pin0->x) > 1.0f) 
// 	||(abs(pin0->y) > 1.0f) 
// 	||(abs(pin1->x) > 1.0f) 
// 	||(abs(pin1->y) > 1.0f) 
	
// 	){
// 	printf("out of tra\n");
// 	k = (pin0->y-pin1->y)/(pin0->x-pin1->x);
// 	m = (pin0->y - k * pin0->x);
// 	if((k>=1)||(k<=-1)) {
// 		// x = (y-m)/k ,y = 1.0f

// 		out0.x = (-1.f - m)/k;
// 		out0.y = -1.f;
// 		out1.x = (1.f - m)/k;
// 		out1.y = 1.f;
// 	} else {
// 		// y = kx+m   ,x = 1.0f
// 		out0.x = -1.f;
// 		out0.y = k*-1.f+m;
// 		out1.x = 1.f;
// 		out1.y = k*1.f+m;
// 	}
// 	PRINTF_POINT(out0);
// PRINTF_POINT(out1);
// }else{

// 	out0 = *pin0;
// 	out1 = *pin1;
// }

	// PRINTF_POINT(out0);
	// PRINTF_POINT(out1);
	// printf("k = %f, m = %f\n", k, m);
	// *y0 = out0;
	// *y1 = out1;
	pout0->x = (out0.x+1)*ts->w*0.5f;//400
	pout0->y = (out0.y+1)*ts->h*0.5f;//300
	pout1->x = (out1.x+1)*ts->w*0.5f;
	pout1->y = (out1.y+1)*ts->h*0.5f;

	pout0->y = 600 - pout0->y;
	pout1->y = 600 - pout1->y;

	// printf("% 3.2f/% 3.2f\n", pout0->x, pout0->y);
	// printf("% 3.2f/% 3.2f\n", pout1->x, pout1->y);


}

