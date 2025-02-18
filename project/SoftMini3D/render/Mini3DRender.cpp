#include "Mini3DRender.h"
#include "Mini3DDevice.h"
#include "Mini3DDraw.h"
#include "Mini3DCalcrhw.h"
#include <stdio.h>
#include <math.h>

int print;
static void device_draw_traps(device_t *device, vertex_t v1, vertex_t v2, vertex_t v3);
//=====================================================================
// 渲染实现
//=====================================================================
// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
// static void trapezoid_edge_interp(trapezoid_t *trap, float y) {
// 	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
// 	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
// 	float t1 = (y - trap->left.v1.pos.y) / s1;
// 	float t2 = (y - trap->right.v1.pos.y) / s2;
// 	if(print == 0){
// 		// pr_debug("%s t1 = %f, t2 = %f, y = %f, rhw = %f/%f, u/v = %f/%f\n", __func__, t1, t2, y, trap->left.v1.rhw, trap->left.v2.rhw, trap->left.v.tc.u, trap->left.v.tc.v);
// 	}
// 	vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
// 	vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
// 	if(print == 0){
// 		// pr_debug("%s t1 = %f, t2 = %f, y = %f, rhw = %f, u/v = %f/%f\n", __func__, t1, t2, y, trap->left.v.rhw, trap->left.v.tc.u, trap->left.v.tc.v);
// 	}
// }

// 根据左右两边的端点，初始化计算出扫描线的起点和步长
// static void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
// 	float width = trap->right.v.pos.x - trap->left.v.pos.x;
// 	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
// 	scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
// 	scanline->y = y;
// 	scanline->v = trap->left.v;
// 	if(print == 0){
// 		// pr_debug("%s  y = %d, rhw = %3.2F\n", __func__, y, trap->left.v.rhw);
// 	}
// 	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
// 	vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
// 	// if(print == 0)
// 	// 	printf("scanline %3.2f\n", scanline->v.rhw);
// }

						/*
						|-------------->  x
						|    v1
						|   /t1
						|  / v
						| /
						|v2
						y
						*/
// static vector_t LightPoint = {0,1.f,0};
// static vector_t LightDir = {0,-1.f,0};
vertex_t interp_spos(vertex_t v1, vertex_t v2, float t) {
	float x_target = interp(v1.pos.x, v2.pos.x, t);
	float y_target = interp(v1.pos.y, v2.pos.y, t);
	vertex_t p;

	// 计算插值参数
	float p_x = (x_target - v1.pos.x) / (v2.pos.x - v1.pos.x);
	float p_y = (y_target - v1.pos.y) / (v2.pos.y - v1.pos.y);

	// 线性插值透视齐次坐标
	float rhw_target = interp(v1.rhw ,v2.rhw ,p_x);

	// 线性插值实际三维坐标
	float x_target_w = interp(v1.spos.x*v1.rhw, v2.spos.x*v2.rhw, p_x);
	float y_target_w = interp(v1.spos.y*v1.rhw, v2.spos.y*v2.rhw, p_x);
	float z_target_w = interp(v1.spos.z*v1.rhw, v2.spos.z*v2.rhw, p_x);

	// 
	// float w = 10.f/4;
	// 应用透视校正
	p.spos.x = x_target_w / rhw_target;
	p.spos.y = y_target_w / rhw_target;
	p.spos.z = z_target_w / rhw_target;
	p.rhw = rhw_target;

	p.pos.x = x_target;
	p.pos.y = y_target;
	return p;

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
/*
p3--t1.bottom
p2--t0.bottom t1.top
p1--t0.top		
*/	
// 主渲染函数
void device_render_trap(device_t *device, trapezoid_t *trap) {
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	float z_min = 10;
	float z_max = -10;
	for (j = top; j < bottom; j++) {
		// if(print == 0){
		// 	printf("draw %d line, %3.2f\n", j, trap->left.v.rhw);
		// }
		if(print == 0){
			// pr_debug("---\n");
		}
		if (j >= 0 && j < device->height) {
			// trapezoid_edge_interp(trap, (float)j + 0.5f);
			// {
				//=====================================================================
				// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
				// static void trapezoid_edge_interp(trapezoid_t *trap, float y) {
				float y = (float)j + 0.5f;
				float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
				float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
				float t1 = (y - trap->left.v1.pos.y) / s1;
				float t2 = (y - trap->right.v1.pos.y) / s2;


				vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
				vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
				// if(trap->t != 0) {
					vertex_t v1l = trap->left.v1; 
					vertex_t v2l = trap->left.v2; 
					vertex_t v1r = trap->right.v1; 
					vertex_t v2r = trap->right.v2; 
					vertex_t sposleft = interp_spos(v1l,v2l,t1);
					vertex_t sposright = interp_spos(v1r,v2r,t2);


					// float x_target_l = interp(v1l.pos.x, v2l.pos.x, t1);
					// float y_target_l = interp(v1l.pos.y, v2l.pos.y, t1);

					// float x_target_r = interp(v1r.pos.x, v2r.pos.x, t2);
					// float y_target_r = interp(v1r.pos.y, v2r.pos.y, t2);
					// float w = 10.f/4;
					// if (sposleft.spos.z >(w)){
					// 	device_point(device, x_target_l, y_target_l, 0xff00);
					// }
					// if (sposright.spos.z >(w)){
					// 	device_point(device, x_target_r, y_target_r, 0xff00);
					// }
				// }
// }
			// }
			// trapezoid_init_scan_line(trap, &scanline, j);
			// {
				// 根据左右两边的端点，初始化计算出扫描线的起点和步长
			// static void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
				float width = trap->right.v.pos.x - trap->left.v.pos.x;
				scanline.x = (int)(trap->left.v.pos.x + 0.5f);
				scanline.w = (int)(trap->right.v.pos.x + 0.5f) - scanline.x;
				scanline.y = j;
				scanline.v = trap->left.v;
				scanline.p0 = sposleft;
				scanline.p1 = sposright;
				scanline.lightdir = device->LightDir;
				scanline.lightpoint = device->LightPoint;
				if(print == 0){
					// pr_debug("y = %d, rhw = %3.2F\n", j, trap->left.v.rhw);
				}
				if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline.w = 0;
				vertex_division(&scanline.step, &trap->left.v, &trap->right.v, width);
				// if(print == 0)
				// 	printf("scanline %3.2f\n", scanline->v.rhw);
			// }
			// }
			// if(print == 0) pr_debug("%3.2f/%3.2f/%3.2f\n", scanline.v.spos.x, scanline.v.spos.y, scanline.v.spos.z);

			device_draw_scanline(device, &scanline);
		}
		// pr_debug("%3.2f/%3.2f\n", z_max, z_min);
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
	trap[0].t = 0;
	trap[1].t = 0;
	trap[0].isShort = false;
	trap[1].isShort = false;
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
/*
p3--t1.bottom
p2--t0.bottom t1.top
p1--t0.top
*/
	float h21 = p2->pos.y - p1->pos.y;
	float h32 = p3->pos.y - p2->pos.y;
	float t = h21/(h21+h32);
	// if (x <= p3->pos.x) {
	// 	pr_debug("t %3.2f L\n", t);
	// }else{
	// 	pr_debug("t %3.2f R\n", t);
	// }

	if (x <= p3->pos.x) {		// triangle left
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		trap[1].left.v1 = *p2;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p1;
		trap[1].right.v2 = *p3;
		
		trap[0].isShort = true;
		trap[1].isShort = false;
		trap[0].t = t;
		trap[1].t = t;
	}	else {					// triangle right
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p2;
		trap[1].left.v1 = *p1;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p2;
		trap[1].right.v2 = *p3;

		trap[0].isShort = false;
		trap[1].isShort = true;
		trap[0].t = t;
		trap[1].t = t;
	}


	return 2;
}

void device_trans_line(device_t *device, point_t p1, point_t p2, point_t p3, IUINT32 c) {
	device_draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, c);	
	device_draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, c);
	device_draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, c);
}

static void inter(vector_t *pin1, vector_t*pin0, float t){

	pin1->x = pin1->x+t*(pin0->x-pin1->x);
	pin1->y = pin1->y+t*(pin0->y-pin1->y);
	pin1->z = pin1->z+t*(pin0->z-pin1->z);
	pin1->w = pin1->w+t*(pin0->w-pin1->w);
}
static point_t inter_point(point_t p0, point_t p1, float t) {
	point_t ret;
	ret.x = p0.x + t * (p1.x-p0.x);
	ret.y = p0.y + t * (p1.y-p0.y);
	ret.z = p0.z + t * (p1.z-p0.z);
	ret.w = p0.w + t * (p1.w-p0.w);

	return ret;
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

	ImGui::Begin("device_draw_triangle");
	// 按照 Transform 变化
	// transform_apply(&device->transform, &c1, &v1->pos);
	matrix_apply( &c1, &v1->pos, &device->transform.mvp);
	// transform_apply(&device->transform, &c2, &v2->pos);
	matrix_apply( &c2, &v2->pos, &device->transform.mvp);
	// transform_apply(&device->transform, &c3, &v3->pos);
	matrix_apply( &c3, &v3->pos, &device->transform.mvp);

	ImGui::CheckboxFlags("circle", &device->test, ImGuiConfigFlags_NoMouse);
	// if(device->test){
	// 	if (c1.w < 0 && c2.w < 0 && c3.w < 0){
	// 		ImGui::End();
	// 		return;
	// 	}

	// }
	// 归一化
	transform_homogenize(&device->transform, &p1, &c1);
	transform_homogenize(&device->transform, &p2, &c2);
	transform_homogenize(&device->transform, &p3, &c3);

	// point_t co1, co2, co3;
	// point_t po1, po2, po3, po4;
	static int flag = 0;
	ImGui::CheckboxFlags("cube", &flag, ImGuiConfigFlags_NoMouse);

	VertDraw vert1 = drawLine(device, v1->pos, v2->pos, 0);
	ImGui::Text("---");
	VertDraw vert2 = drawLine(device, v2->pos, v3->pos, 0);
	ImGui::Text("---");
	VertDraw vert3 = drawLine(device, v3->pos, v1->pos, 0);



	// drawCharAt(device, p1, "v1");
	// drawCharAt(device, p2, "v2");
	// drawCharAt(device, p3, "v3");
	vector<vector_t> points;
	vertex_t t1, t2, t3;

	t1 = *v1;
	t2 = *v2;
	t3 = *v3;


	vector<vertex_t> out2 = Hodgmanmain(device, vert1, vert2, vert3, *v1, *v2, *v3);
	ImGui::Text("draw %d", out2.size());

	if(flag) {
		ImGui::SliderFloat2("vert", (float *)&out2[0].tc, 0, 1);
	}
	for(int i = 0;i<out2.size()-2;i++) {
		device_trans_line(device, out2[0].pos, out2[i+1].pos, out2[i+2].pos, 0xff);
	}
	// drawCharAt(device, out2[0].pos, "axxx");
	// device_draw_traps(device, out2[0], out2[1], out2[2]);

	if (render_state & RENDER_STATE_WIREFRAME) {		// 线框绘制
		// device_trans_line(device, p1, p2, p3, device->foreground);
		// device_draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->foreground);	
		// device_draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->foreground);
		// device_draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->foreground);
	}
	int use0 = 1;
	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		vertex_t t1, t2, t3, t4;

		vertex_t vt3, vt2;
		// v 投影之前
		t1 = *v1;
		t2 = *v2;
		t3 = *v3;

		t1.pos = p1;
		t1.pos.w = c1.w;
		t1.spos = v1->pos;	

		t2.pos = p2;
		t2.pos.w = c2.w;
		t2.spos = v2->pos;

		t3.pos = p3;
		t3.pos.w = c3.w;
		t3.spos = v3->pos;

		vt2.pos   = vert2.screenPosition[0];
		vt2.pos.w = vert2.localPosition[0].w;
		vt2.spos  = vert2.localPosition[0];

		vt3.pos   = vert3.screenPosition[0];
		vt3.pos.w = vert3.localPosition[0].w;
		vt3.spos  = vert3.localPosition[0];
/*
10 t3-----t2 11
    \    /
vert3\__/vert2
 vt3  \/  vt2
      t1
*/
		float p;
		p = vert2.t[0];
		vt2.tc.u = t1.tc.u + p * (t2.tc.u - t1.tc.u);
		vt2.tc.v = t1.tc.v + p * (t2.tc.v - t1.tc.v);
		// p = vert3.t[0];
		vt3.tc.u = t1.tc.u + p * (t3.tc.u - t1.tc.u);
		vt3.tc.v = t1.tc.v + p * (t3.tc.v - t1.tc.v);


		static texcoord_t tc1,tc2,tc3,tc4; 

		// ImGui::SliderFloat2("t1u", (float *)&tc1, 0, 1.f);
		// ImGui::SliderFloat2("t2u", (float *)&tc2, 0, 1.f);
		// ImGui::SliderFloat2("t4u", (float *)&tc4, 0, 1.f);
		// t1.tc = tc1;
		// t2.tc = tc2;
		// t4.tc = tc4;

		device_draw_traps(device, vt3, t2, t3);
		if(vert2.ret != -1)
			device_draw_traps(device, vt3, vt2, t2);
		device_draw_traps(device, t1, t2, t3);
		// drawCharAt(device, vt2.pos.x, vt2.pos.y-20, "t4");
		// drawCharAt(device, t1.pos.x, t1.pos.y-20, "t1");
		// drawCharAt(device, t2.pos.x, t2.pos.y-20, "t2");

	}


	ImGui::End();
	print = 1;
}




static void device_draw_traps(device_t *device, vertex_t v1, vertex_t v2, vertex_t v3) {
	trapezoid_t traps[2];

	vertex_rhw_init(&v1);	// 初始化 w
	vertex_rhw_init(&v2);	// 初始化 w
	vertex_rhw_init(&v3);	// 初始化 w

	// 拆分三角形为0-2个梯形，并且返回可用梯形数量
	int n = trapezoid_init_triangle(traps, &v1, &v2, &v3);
	if (n >= 1) {
		device_render_trap(device, &traps[0]);
		// calcrhw(device, traps);
		device_draw_line(device, traps[0].left.v1.pos, traps[0].left.v2.pos, 0);
		device_draw_line(device, traps[0].left.v1.pos, traps[0].left.v2.pos, 0);

		// drawCharAt(device, traps[0].left.v1.pos, "v1");
		// drawCharAt(device, traps[0].left.v2.pos, "v2");
		// drawCharAt(device, traps[0].left.v.pos,  "v");
		// drawCharAt(device, traps[0].right.v.pos, "v");
	}
	if (n >= 2) {
		device_render_trap(device, &traps[1]);
	}

		device_draw_line(device, traps[0].left.v.pos, traps[0].right.v.pos, 0);

		// point_t vp1, vp2;
		// vp1 = {0,0,0,1};
		// vp2 = {100,500,0,1};
		// vp1 = traps[0].left.v.pos;
		// vp2 = traps[0].right.v.pos;

		// vp1.x -= 50;
		// vp1.y -= 50;

		// vp2.x += 50;
		// vp2.y += 50;
		// device_draw_line(device, vp1, vp2, 0);
}