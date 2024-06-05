#include "Mini3DRender.h"
#include "Mini3DDevice.h"

//=====================================================================
// 渲染实现
//=====================================================================

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
		if (j >= 0 && j < device->height) {
			trapezoid_edge_interp(trap, (float)j + 0.5f);
			trapezoid_init_scan_line(trap, &scanline, j);
			device_draw_scanline(device, &scanline);
		}
		if (j >= device->height) break;
	}
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
	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid_t traps[2];
		int n;

		t1.pos = p1; 
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;
		
		vertex_rhw_init(&t1);	// 初始化 w
		vertex_rhw_init(&t2);	// 初始化 w
		vertex_rhw_init(&t3);	// 初始化 w
		
		// 拆分三角形为0-2个梯形，并且返回可用梯形数量
		n = trapezoid_init_triangle(traps, &t1, &t2, &t3);
		// printf("n=%d\n",n);
		ImGui::SliderFloat2("trans", (float *)traps, -3.0f, 3.0f);
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
			device_draw_line(device, device->width/2, (int)traps[0].top, device->width/2, (int)traps[0].bottom, 0xffffff);
			if(v){
				device_point(device, (int)traps[0].left.v.pos.x, (int)traps[0].left.v.pos.y, 0xff);

				// device_draw_line(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 
				// 					(int)traps[0].left.v2.pos.x, (int)traps[0].left.v2.pos.y, 0xff);
				// device_draw_line(device, (int)traps[0].right.v1.pos.x, (int)traps[0].right.v1.pos.y, 
				// 					(int)traps[0].right.v2.pos.x, (int)traps[0].right.v2.pos.y, 0xff);
			}
			if(v1){
				device_point(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 0xff);

				// device_draw_line(device, (int)traps[0].left.v1.pos.x, (int)traps[0].left.v1.pos.y, 
				// 						(int)traps[0].left.v.pos.x, (int)traps[0].left.v.pos.y, 0xff00);

				// device_draw_line(device, (int)traps[0].right.v1.pos.x, (int)traps[0].right.v1.pos.y, 
				// 						(int)traps[0].right.v.pos.x, (int)traps[0].right.v.pos.y, 0xff00);
			}
			if(v2){
				device_point(device, (int)traps[0].left.v2.pos.x, (int)traps[0].left.v2.pos.y, 0xff);

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
}