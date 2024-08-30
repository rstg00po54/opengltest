#include "Mini3DCalcLines.h"
#include "Mini3DMath.h"
#include <math.h>
/*
persp->ortho

n 0 0 0
0 n 0 0
0 0 n+f -nf
0 0 1 0
*/

// fovy 角度
void matrix_set_perspective_to_ortho(matrix_t *m, float zn, float zf) {
	// float fax = 1.0f / (float)tan(2.0f*3.14f*fovy/360.0f);
	matrix_set_zero(m);
	m->m[0][0] = zn;
	m->m[1][1] = zn;
	m->m[2][2] = (zn + zf);
	m->m[3][2] = 1;
	m->m[2][3] = -zn*zf;

}

/*
ortho
2/(r-l) 0 0 0
0 2/(t-b) 0 0
0 0 2/(n-f) -(n+f)/(n-f)
0 0 0 1

2/(r-l) = 1/(nA*tan 22.5)
2/(t-b) = 1/(n*tan 22.5)
*/
void matrix_set_ortho(matrix_t *m, device_t *device, float zn, float zf) {
	float fax = 1.0f / (float)tan(2.0f*3.14f*45.0f/360.0f);
    float a = device->aspect_ratio;
	matrix_set_zero(m);
	m->m[0][0] = fax/(zn*a);
	m->m[1][1] = fax/(zn);
	m->m[2][2] = 2.f/(zn-zf);
	m->m[2][3] = -(zn+zf)/(zn-zf);
	m->m[3][3] = 1.f;

}


/*

o X p->r
2n/(r-l) 0 0 0
0 2n/(t-b) 0 0
0 0 (n+f)/(n-f) -2nf/(n-f)
0 0 1 0

*/
point_t calcM(point_t t, device_t *device) {
    point_t out;
    matrix_t pto,o,mvp,m;

    point_t a1,a2;

    matrix_set_perspective_to_ortho(&pto, 1.f, 100.f);
    matrix_set_ortho(&o, device, 1.f, 100.f);

    matrix_mul(&m, &o, &pto);
    matrix_mul(&mvp, &m, &device->transform.view);
    // out = o x pto x view x point

    matrix_apply(&a1, &t, &device->transform.view);
    matrix_apply(&a2, &a1, &pto);
    matrix_apply(&out, &a2, &o);
    // if(t.z!=0){
	// ImGui::SliderFloat4("a2", (float *)&a2, -3.0f, 3.0f);
	// ImGui::SliderFloat4("out", (float *)&out, -3.0f, 3.0f);

	// }
	// matrix_apply(&out, &t, &device->transform.mvp);


	return out;

}
void inter(vector_t *pin1, vector_t*pin0, float t){

	pin1->x = pin1->x+t*(pin0->x-pin1->x);
	pin1->y = pin1->y+t*(pin0->y-pin1->y);
	pin1->z = pin1->z+t*(pin0->z-pin1->z);
	pin1->w = pin1->w+t*(pin0->w-pin1->w);
}
// -w < x < w，-w < y < w，-w < z < w
void transform_home0(const transform_t *ts, 
				vector_t *pout0, vector_t *pout1, 
				vector_t *pin0, vector_t *pin1) {
	vector_t t,o;
	float hw;
	ImGui::Text("-------");
	ImGui::SliderFloat4("输入0", (float *)pin0, 0.0f, 50.0f);
	ImGui::SliderFloat4("输入1", (float *)pin1, 0.0f, 50.0f);
	static bool v = true;
	ImGui::Checkbox("clip", &v);
	if(abs(pin1->x)>abs(pin1->w)){
		ImGui::Text("need clip x");
	}
	if(abs(pin1->y)>abs(pin1->w)){
		ImGui::Text("need clip y");
	}
	if(abs(pin1->z)>abs(pin1->w)){
		ImGui::Text("need clip z");
	}

	// if(v){
		if(pin1->z < -abs(pin1->w)){
			ImGui::Text("clip z -");
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)		
		}
		if(pin1->z > abs(pin1->w)){
			ImGui::Text("clip z +");
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)	
			// vector_t ret;
			float t = (pin1->w - pin1->z)/((pin1->w-pin1->z)-(pin0->w-pin0->z));
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪z+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->x > abs(pin1->w)){
			ImGui::Text("clip x +");
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->x)/((pin1->w-pin1->x)-(pin0->w-pin0->x));
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪x+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->x < -abs(pin1->w)){
			ImGui::Text("clip x -");
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->x)/((pin1->w+pin1->x)-(pin0->w+pin0->x));
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪x-", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y > abs(pin1->w)){
			ImGui::Text("clip y +, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->y)/((pin1->w-pin1->y)-(pin0->w-pin0->y));
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪y+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y < -abs(pin1->w)){
			ImGui::Text("clip y -, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->y)/((pin1->w+pin1->y)-(pin0->w+pin0->y));
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪y-", (float *)pin1, 0.0f, 50.0f);
		}
	// }
	hw = 1.f/pin1->w;
	pin1->x = hw*pin1->x;
	pin1->y = hw*pin1->y;
	pin1->z = hw*pin1->z;
	pin1->w = hw*pin1->w;
	
	ImGui::SliderFloat4("pin1归一化", (float *)pin1, 0.0f, 50.0f);
	// ImGui::SliderFloat4("o", (float *)&o, 0.0f, 50.0f);

	// pout0->x = (pin0->x+1)*ts->w*0.5f;//400
	// pout0->y = (pin0->y+1)*ts->h*0.5f;//300
	// pout0->z = pin0->z;
	// pout1->x = (pin1->x+1)*ts->w*0.5f;
	// pout1->y = (pin1->y+1)*ts->h*0.5f;

	// pout0->y = 600 - pout0->y;
	// pout1->y = 600 - pout1->y;
	
	// ImGui::SliderFloat4("pout0", (float *)pout0, 0.0f, 50.0f);
	// ImGui::SliderFloat4("pout1", (float *)pout1, 0.0f, 50.0f);

}

void transform_Normalization(const transform_t *ts, vector_t *po1, vector_t *po0, vector_t *pin1, vector_t*pin0) {
	// vector_t po0,po1;
	float rhw;
	transform_home0(ts, po0, po1, pin0, pin1);
	transform_home0(ts, po1, po0, pin1, pin0);
	// *pin0 = po0;
	// *pin1 = po1;
	// pout0->y = 600 - pout0->y;
	// pout1->y = 600 - pout1->y;

	*po0 = *pin0;
	*po1 = *pin1;
	rhw = 1.f/po0->w;
	po0->x = (po0->x * rhw + 1.0f) * ts->w * 0.5f;
	po0->y = (1.0f - po0->y * rhw) * ts->h * 0.5f;
	rhw = 1.f/po1->w;
	po1->x = (po1->x * rhw + 1.0f) * ts->w * 0.5f;
	po1->y = (1.0f - po1->y * rhw) * ts->h * 0.5f;
}
#if 0
void drawTestLines(device_t *device) {

	static float x = 15.0f;
	static float y = 1.0f;
	point_t p[2],phome[2];
	point_t psrc[][2] = {
		{{0,0,0,1},{10,0,0,1}},
		// {{0,0,0,1},{0,10,0,1}},
		// {{0,0,0,1},{0,0,10,1}},
	};
	ImGui::SliderFloat("x", (float *)&x, 0.0f, 50.0f);
	int len = sizeof(psrc)/sizeof(psrc[0]);
	for(int i = 0;i<len;i++) {
		psrc[0][1].x = x;
		psrc[0][0].x = -x;
#if 0
		// p[0] = calcM(psrc[i][0],device);
		// p[1] = calcM(psrc[i][1],device);
		// transform_Normalization(&device->transform,&phome[0], &phome[1], &p[0], &p[1]);
#else
		matrix_apply(&p[0], &psrc[i][0],  &device->transform.mvp);
		matrix_apply(&p[1], &psrc[i][1],  &device->transform.mvp);
		transform_homogenize(&device->transform, &phome[0], &p[0]);
		transform_homogenize(&device->transform, &phome[1], &p[1]);
#endif
		device_draw_line(device, phome[0].x, phome[0].y, phome[1].x, phome[1].y, 0);
	}
	
}
#endif
void drawMyLines(device_t *device) {

	// ImGui::Begin("draw_box");


	static float x = 1.0f;
	static float y = 1.0f;
	// vertex_t p1 = mesh[i];
	point_t p[2],phome[2];
	// point_t po2[2];
	matrix_t m_out[2];
	point_t psrc[][2] = {
		{{0,0,0,1},{1,0,0,1}},
		{{0,0,0,1},{0,5,0,1}},
		{{0,0,0,1},{0,0,10,1}},

		{{0,0,0,1},{1,1,1,1}},
		// {{0,0,0,1},{1,1,1,1}},

		// {{1,-1,-1,1},{1,1,1,1}},
		// {{-1,-1,1,1},{-1,1,-1,1}},
		// {{-1,1,1,1},{1,-1,1,1}},
		// {{1,-1,1,1},{-1,-1,-1,1}},
		// {{-1,-1,-1,1},{1,1,-1,1}},
		// {{-1,1,1,1},{1,1,-1,1}},

		// {{1,1,-1,1},{1,1,1,1}},
	};
		// x = psrc[0][0].x;
		// y = psrc[0][0].y;
	matrix_set_rotate(&device->transform.rotate, 0, 1, 0,  0.f);
	transform_update(&device->transform);
	int len = sizeof(psrc)/sizeof(psrc[0]);
	for(int i = 0;i<len;i++) {

#if 1
		matrix_apply( &p[0], &psrc[i][0], &device->transform.mvp);
		matrix_apply( &p[1], &psrc[i][1], &device->transform.mvp);
		// p[0].z = -p[0].z;
		// p[1].z = -p[1].z;
        transform_homogenize(&device->transform, &phome[0], &p[0]);
        transform_homogenize(&device->transform, &phome[1], &p[1]);

#else
        // p[0] = calcM(psrc[i][0],device);
        // p[1] = calcM(psrc[i][1],device);
		// transform_Normalization(&device->transform,&phome[0], &phome[1], &p[0], &p[1]);

#endif
        float rhw = 1.0f / p[1].w;
        p[1].x = p[1].x*rhw;
        p[1].y = p[1].y*rhw;
        p[1].z = p[1].z*rhw;
        p[1].w = p[1].w*rhw;
		// 归一化
		// p[0]->po1[0]

		// transform_home(&device->transform, &phome[0], &phome[1], &p[0], &p[1]);

		// SDL_RenderDrawLine(renderer,0,0,po.x, po.y);
		int c[] = {0xff,0xff00,0x0000,0,0};
		device_draw_line(device, phome[0].x, phome[0].y, phome[1].x, phome[1].y, c[i>3?3:i]);
	}
	// drawTestLines(device);
	device_draw_line(device, 0.f, 0.f, 400.f, 400.f, 0);
	// ImGui::End();
}
