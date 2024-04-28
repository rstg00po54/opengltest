#include "Mini3DCalcLines.h"
/*
persp->ortho

n 0 0 0
0 n 0 0
0 0 n+f -nf
0 0 1 0

ortho
2/(r-l) 0 0 0
0 2/(t-b) 0 0
0 0 2/(n-f) -(n+f)/(n-f)
0 0 0 1

o X p->r
2n/(r-l) 0 0 0
0 2n/(t-b) 0 0
0 0 (n+f)/(n-f) -2nf/(n-f)
0 0 1 0

*/
void drawMyLines(device_t *device) {

	ImGui::Begin("draw_box");


	static float x = 1.0f;
	static float y = 1.0f;
	// vertex_t p1 = mesh[i];
	point_t p[2];
	point_t po1[2];
	// point_t po2[2];
	matrix_t m_out[2];
	point_t psrc[][2] = {
		// {{-100,0,0,1},{100,0,0,1}},
		// {{0,-100,0,1},{0,100,0,1}},
		// {{0,0,-100,1},{0,0,100,1}},

		// {{1,1,100,1},{1,1,-100,1}},
		// {{1,-1,100,1},{1,-1,-100,1}},

		// {{1,1,1,1},{1,-1.5,1,1}},
		{{-1,100,1,1},{-1,-100,1,1}},

		// {{100,1,1,1},{-100,1,1,1}},
		// {{100,-1,1,1},{-100,-1,1,1}}

		// {{0,2/3.f,0,1},{-1.732/3.f,-1/3.f,0,1}},
		// {{0,2/3.f,0,1},{1.732/3.f,-1/3.f,0,1}},
		// {{-1.732/3.f,-1/3.f,0,1},{1.732/3.f,-1/3.f,0,1}},
		
		};
		// x = psrc[0][0].x;
		// y = psrc[0][0].y;
	int len = sizeof(psrc)/sizeof(psrc[0]);
	for(int i = 0;i<len;i++) {

		if(i == 0){
			ImGui::SliderFloat("x", (float *)&x, 0.0f, 50.0f);
			ImGui::SliderFloat("y", (float *)&y, 0.0f, 50.0f);
		}
		psrc[0][0].y = -y;
		psrc[0][1].y = y;

		// psrc[1][0].y = -y;
		// psrc[1][1].y = y;
		// PRINTF_POINT(psrc[0][0]);
		// PRINTF_POINT(psrc[0][1]);
		// PRINTF_POINT(psrc[1][0]);
		// PRINTF_POINT(psrc[1][1]);
	

		matrix_apply( &p[0], &psrc[i][0], &device->transform.mvp);
		matrix_apply( &p[1], &psrc[i][1], &device->transform.mvp);

		// 归一化
		// p[0]->po1[0]
		transform_home(&device->transform, &po1[0], &po1[1], &p[0], &p[1]);

		if(i == 0) {
			ImGui::SliderFloat4("p0", (float *)&p[0], -3.0f, 3.0f);
			ImGui::SliderFloat4("p1", (float *)&p[1], -3.0f, 3.0f);

		}
		// PRINT_POINT(p);
		// PRINT_POINT(po);
		// SDL_RenderDrawLine(renderer,0,0,po.x, po.y);
		device_draw_line(device, po1[1].x, po1[1].y, po1[0].x, po1[0].y, 0xffffffff);
		// device_draw_line(device, po2[1].x, po2[1].y, po2[0].x, po2[0].y, 0xffffffff);
	}
	ImGui::End();
}
