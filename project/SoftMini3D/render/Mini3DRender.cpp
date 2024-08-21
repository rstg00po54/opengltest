#include "Mini3DRender.h"
#include "Mini3DDevice.h"
#include <stdio.h>

int print;
//=====================================================================
// 渲染实现
//=====================================================================
// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
static void trapezoid_edge_interp(trapezoid_t *trap, float y) {
	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
	float t1 = (y - trap->left.v1.pos.y) / s1;
	float t2 = (y - trap->right.v1.pos.y) / s2;
	if(print == 0){
		printf("%s t1 = %f, t2 = %f, y = %f, rhw = %f/%f, u/v = %f/%f\n", __func__, t1, t2, y, trap->left.v1.rhw, trap->left.v2.rhw, trap->left.v.tc.u, trap->left.v.tc.v);
	}
	vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
	vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
	if(print == 0){
		printf("%s t1 = %f, t2 = %f, y = %f, rhw = %f, u/v = %f/%f\n", __func__, t1, t2, y, trap->left.v.rhw, trap->left.v.tc.u, trap->left.v.tc.v);
	}
}

// 根据左右两边的端点，初始化计算出扫描线的起点和步长
static void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
	float width = trap->right.v.pos.x - trap->left.v.pos.x;
	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
	scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
	scanline->y = y;
	scanline->v = trap->left.v;
	if(print == 0){
		printf("%s  y = %d, rhw = %3.2F\n", __func__, y, trap->left.v.rhw);
	}
	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
	vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
	// if(print == 0)
	// 	printf("scanline %3.2f\n", scanline->v.rhw);
}
/*
lv2   rv2
-------
\     |
 \    |
lv\---| rv
   \  |
    \ |
	 \
lv1   rv1
*/

// 主渲染函数
void device_render_trap(device_t *device, trapezoid_t *trap) {
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
// PRINTF_POINT(trap->right.v1.pos);
// PRINTF_POINT(trap->right.v2.pos);
// PRINTF_POINT(trap->left.v1.pos);
// PRINTF_POINT(trap->left.v2.pos);

	for (j = top; j < bottom; j++) {
		// if(print == 0){
		// 	printf("draw %d line, %3.2f\n", j, trap->left.v.rhw);
		// }
		if(print == 0){
			printf("---\n");
		}
		if (j >= 0 && j < device->height) {
			trapezoid_edge_interp(trap, (float)j + 0.5f);
			trapezoid_init_scan_line(trap, &scanline, j);
			device_draw_scanline(device, &scanline);
		}
		if (j >= device->height) break;
	}

}

// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
//trapezoid 不规则四边形
int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, 
	const vertex_t *p2, const vertex_t *p3) {
	const vertex_t *p;
	float k, x;
// p1.y<p2.y<p3.y
	if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
	if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
	if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	if (p1->pos.y == p2->pos.y) {	// triangle down
		if (p1->pos.x > p2->pos.x) p = p1, p1 = p2, p2 = p;//p1.x < p2.x
		trap[0].top = p1->pos.y;
		trap[0].bottom = p3->pos.y;
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p2;
		trap[0].right.v2 = *p3;
		ImGui::Text("1 trap down");
		return (trap[0].top < trap[0].bottom)? 1 : 0;
	}

	if (p2->pos.y == p3->pos.y) {	// triangle up
		if (p2->pos.x > p3->pos.x) p = p2, p2 = p3, p3 = p;
		trap[0].top = p1->pos.y;
		trap[0].bottom = p3->pos.y;
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		ImGui::Text("1 trap up");
		return (trap[0].top < trap[0].bottom)? 1 : 0;
	}

	trap[0].top = p1->pos.y;
	trap[0].bottom = p2->pos.y;
	trap[1].top = p2->pos.y;
	trap[1].bottom = p3->pos.y;

	k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

	if (x <= p3->pos.x) {		// triangle left
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		trap[1].left.v1 = *p2;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p1;
		trap[1].right.v2 = *p3;
	}	else {					// triangle right
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p2;
		trap[1].left.v1 = *p1;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p2;
		trap[1].right.v2 = *p3;
	}

	return 2;
}



// #define PRINT_POINT(c1) printf(#c1);printf(":%f %f %f\n", c1.x, c1.y, c1.z);
// #define PRINTF_NAME_VALUE(X) {sprintf(&buffer_text[strlen(buffer_text)-2], #X); sprintf(buffer_text, "   %f %f %f\n", X.x, X.y, X.z);}
// 根据 render_state 绘制原始三角形
void device_draw_triangle(device_t *device,
	const vertex_t *v1,
	const vertex_t *v2,
	const vertex_t *v3) {
	point_t p1, p2, p3, c1, c2, c3;

	int render_state = device->render_state;

	ImGui::Begin("Hello");

// sprintf(buffer_text, )
	// 按照 Transform 变化
	// transform_apply(&device->transform, &c1, &v1->pos);
	matrix_apply( &c1, &v1->pos, &device->transform.mvp);
	// transform_apply(&device->transform, &c2, &v2->pos);
	matrix_apply( &c2, &v2->pos, &device->transform.mvp);
	// transform_apply(&device->transform, &c3, &v3->pos);
	matrix_apply( &c3, &v3->pos, &device->transform.mvp);
	// printf("+++\n");
	// PRINT_POINT(v1->pos);
	// PRINT_POINT(c1);
	// PRINT_POINT(v2->pos);
	// PRINT_POINT(c2);
	// PRINT_POINT(v3->pos);
	// PRINT_POINT(c3);
	// printf("---\n");

	// printf("%d\n", strlen(buffer_text));

	// 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
	// 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
	// if (transform_check_cvv(&c1) != 0) {
	// 	return;
	// } 
	// if (transform_check_cvv(&c2) != 0) {
	// 	return;
	// }
	// if (transform_check_cvv(&c3) != 0) {
	// 	return;
	// }

	// 归一化
	transform_homogenize(&device->transform, &p1, &c1);
	transform_homogenize(&device->transform, &p2, &c2);
	transform_homogenize(&device->transform, &p3, &c3);
	// PRINTF_POINT(p1);
	// PRINTF_POINT(p2);
	// PRINTF_POINT(p3);
		char *items[] = {"mode1", "mode2","mode3"};
		static int curIndex = 0;

		ImGui::Combo("mode", &curIndex, items, 3);
		device->module = curIndex;
		ImGui::Text("index %d", curIndex);
		if(ImGui::Button("p")){
			printf("click\n");
			print = 0;
		}
	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid_t traps[2];
		int n;
// v 投影之前
// c 投影之后
// p 归一化
		t1 = *v1;
		t2 = *v2;
		t3 = *v3;
		t1.pos = p1; 
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;
		
		vertex_rhw_init(&t1);	// 初始化 w
		vertex_rhw_init(&t2);	// 初始化 w
		vertex_rhw_init(&t3);	// 初始化 w

		t1.pos.z = 0;
		t2.pos.z = 0;
		t3.pos.z = 0;
		
		// 拆分三角形为0-2个梯形，并且返回可用梯形数量
		n = trapezoid_init_triangle(traps, &t1, &t2, &t3);



		if(print == 0){
			printf("--> %3.2f/%3.2f/%3.2f\n",  t1.rhw, t2.rhw, t3.rhw);
			printf("v1 rhw %f, u/v %f/%f\n", traps[0].left.v1.rhw, traps[0].left.v1.tc.u, traps[0].left.v1.tc.v);
			printf("v2 rhw %f, u/v %f/%f\n", traps[0].left.v2.rhw, traps[0].left.v2.tc.u, traps[0].left.v2.tc.v);
		}
		// printf("n=%d\n",n);
		ImGui::SliderFloat4("t1", (float *)&t1.pos,0,1);
		ImGui::SliderFloat4("t2", (float *)&t2.pos,0,1);
		ImGui::SliderFloat4("t3", (float *)&t3.pos,0,1);
		ImGui::SliderInt("n", (int *)&n, -3, 3);
		static bool v, v1, v2;
		ImGui::Checkbox("v", &v);
		ImGui::Checkbox("v1", &v1);
		ImGui::Checkbox("v2", &v2);
		if (n >= 1) {
			device_render_trap(device, &traps[0]);
		}
		if (n >= 2) device_render_trap(device, &traps[1]);


		if(n>=1){
			// device_draw_line(device, device->width/2, (int)traps[0].top, device->width/2, (int)traps[0].bottom, 0x0);
			float x = ((traps[0].left.v1.pos.x+traps[0].left.v2.pos.x)/2);
			float y = ((traps[0].left.v1.pos.y+traps[0].left.v2.pos.y)/2);
			{
#if 1
				float rhw1 = traps[0].left.v1.rhw;
				float rhw2 = traps[0].left.v2.rhw;
				float h = traps[0].bottom-traps[0].top;
				int y;
				for(y=traps[0].bottom-0.5;y>traps[0].top;y--) {
					vertex_t v1 = traps[0].left.v1;
					vertex_t v2 = traps[0].left.v2;
					float p = (y-v2.pos.y+0.5f)/(v1.pos.y-v2.pos.y);
					float u = v2.tc.u + p*(v1.tc.u-v2.tc.u);
					float rhw = v2.rhw + p*(v1.rhw-v2.rhw);
					float u0 = u/rhw;
					if(print == 0) {
						printf("y = %d, u = %f/%f, p = %f\n", y, u, u0, 1-p);
					}
					if(u0 > 0.875f){
						float x0 = v2.pos.x + p*(v1.pos.x-v2.pos.x);
						float y0 = v2.pos.y + p*(v1.pos.y-v2.pos.y);
						device_point(device, x0, y0, 0xff00);
						ImGui::SliderFloat("p", (float *)&p,0,1);
						break;
					}
				}
#endif


			}




			ImGui::SliderFloat("x", &x, 0, 1.f);
			ImGui::SliderFloat("y", &y, 0, 1.f);

			float xt = 0;
			float yt = 0;

			// device_point(device, xt, yt, 0xff);
			if(v){
				// device_point(device, (int)x, (int)y, 0);
				device_point(device, (int)traps[0].left.v.pos.x, (int)traps[0].left.v.pos.y, 0);

				// device_draw_line(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 
				// 					(int)traps[0].left.v2.pos.x, (int)traps[0].left.v2.pos.y, 0xff);
				// device_draw_line(device, (int)traps[0].right.v1.pos.x, (int)traps[0].right.v1.pos.y, 
				// 					(int)traps[0].right.v2.pos.x, (int)traps[0].right.v2.pos.y, 0xff);
			}
			if(v1){
				device_point(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 0);

				// device_draw_line(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 
				// 						(int)traps[0].left.v.pos.x, (int)traps[0].left.v.pos.y, 0xff00);

				// device_draw_line(device, (int)traps[0].right.v1.pos.x, (int)traps[0].right.v1.pos.y, 
				// 						(int)traps[0].right.v.pos.x, (int)traps[0].right.v.pos.y, 0xff00);
			}
			if(v2){
				device_point(device, (int)traps[0].left.v2.pos.x, (int)traps[0].left.v2.pos.y, 0);

				// device_draw_line(device, (int)traps[0].left.v2.pos.x, (int)traps[0].left.v2.pos.y, 
				// 						(int)traps[0].left.v.pos.x, (int)traps[0].left.v.pos.y, 0x0);
				// device_draw_line(device, (int)traps[0].right.v2.pos.x, (int)traps[0].right.v2.pos.y, 
				// 						(int)traps[0].right.v.pos.x, (int)traps[0].right.v.pos.y, 0x0);
			}
			ImGui::SliderFloat2("v1", (float *)&traps[0].left.v1.pos, -3.0f, 3.0f);
			ImGui::SliderFloat2("v2", (float *)&traps[0].left.v2.pos, -3.0f, 3.0f);
			ImGui::SliderFloat2("v", (float *)&traps[0].left.v.pos, -3.0f, 3.0f);
		}
		// if(n>=2){
		// 	device_draw_line(device, device->width/2, (int)traps[1].top, device->width/2, (int)traps[1].bottom, 0x0);
		// 	device_draw_line(device, (int)traps[1].left.v1.pos.x, (int)traps[1].left.v1.pos.y, 
		// 							(int)traps[1].left.v2.pos.x, (int)traps[1].left.v2.pos.y, 0xffff00);
		// }
	}

	if (render_state & RENDER_STATE_WIREFRAME) {		// 线框绘制
		device_draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->foreground);	
		device_draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->foreground);
		device_draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->foreground);
	}
	ImGui::End();
	print = 1;
}