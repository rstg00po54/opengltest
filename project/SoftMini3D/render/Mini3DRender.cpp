#include "Mini3DRender.h"
#include "Mini3DDevice.h"
#include "Mini3DDraw.h"
#include "Mini3DCalcrhw.h"
#include <stdio.h>
#include <math.h>
#include <cmath>

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
// Perspective-correct interpolation of object-space position along a screen-space edge.
vertex_t interp_spos(vertex_t v1, vertex_t v2, float t) {
    vertex_t result = {};
    const float rhw = interp(v1.rhw, v2.rhw, t);
    result.pos.x = interp(v1.pos.x, v2.pos.x, t);
    result.pos.y = interp(v1.pos.y, v2.pos.y, t);
    result.pos.w = 1.0f;
    result.rhw = rhw;
    if (fabsf(rhw) < 1e-8f) return result;
    result.spos.x = interp(v1.spos.x * v1.rhw, v2.spos.x * v2.rhw, t) / rhw;
    result.spos.y = interp(v1.spos.y * v1.rhw, v2.spos.y * v2.rhw, t) / rhw;
    result.spos.z = interp(v1.spos.z * v1.rhw, v2.spos.z * v2.rhw, t) / rhw;
    result.spos.w = 1.0f;
    return result;
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
                if (width <= 0.0f) continue;
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

	// Compare the middle vertex with the long edge AT the middle vertex's Y.
    k = (p2->pos.y - p1->pos.y) / (p3->pos.y - p1->pos.y);
    x = p1->pos.x + (p3->pos.x - p1->pos.x) * k;
/*
p3--t1.bottom
p2--t0.bottom t1.top
p1--t0.top
*/
	float h21 = p2->pos.y - p1->pos.y;
	float h32 = p3->pos.y - p2->pos.y;
	float t = h21/(h21+h32);
	// if (p2->pos.x <= x) {
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
// Clip a polygon in HOMOGENEOUS clip space. Never divide by w before clipping.
// This projection uses a symmetric z clip range: -w <= z <= w.
struct clip_vertex_t {
    vertex_t vertex;  // Original attributes, not perspective-divided
    point_t clip;     // MVP * vertex.pos
};

static float frustum_distance(const point_t &v, int plane) {
    switch (plane) {
        case 0: return v.w + v.x; // left
        case 1: return v.w - v.x; // right
        case 2: return v.w + v.y; // bottom
        case 3: return v.w - v.y; // top
        case 4: return v.w + v.z; // one z plane
        case 5: return v.w - v.z; // other z plane
        default: return v.w - 0.0001f; // guard perspective division
    }
}

static void lerp_point(point_t *dst, const point_t &a, const point_t &b, float t) {
    dst->x = interp(a.x, b.x, t);
    dst->y = interp(a.y, b.y, t);
    dst->z = interp(a.z, b.z, t);
    dst->w = interp(a.w, b.w, t);
}

static clip_vertex_t lerp_clip_vertex(const clip_vertex_t &a,
                                     const clip_vertex_t &b, float t) {
    clip_vertex_t result = a;
    lerp_point(&result.clip, a.clip, b.clip, t);
    lerp_point(&result.vertex.pos, a.vertex.pos, b.vertex.pos, t);
    lerp_point(&result.vertex.spos, a.vertex.spos, b.vertex.spos, t);
    result.vertex.tc.u = interp(a.vertex.tc.u, b.vertex.tc.u, t);
    result.vertex.tc.v = interp(a.vertex.tc.v, b.vertex.tc.v, t);
    result.vertex.color.r = interp(a.vertex.color.r, b.vertex.color.r, t);
    result.vertex.color.g = interp(a.vertex.color.g, b.vertex.color.g, t);
    result.vertex.color.b = interp(a.vertex.color.b, b.vertex.color.b, t);
    return result;
}

static vector<clip_vertex_t> clip_frustum(const device_t *device,
                                         const vertex_t *v1,
                                         const vertex_t *v2,
                                         const vertex_t *v3) {
    vector<clip_vertex_t> polygon;
    for (const vertex_t *source : {v1, v2, v3}) {
        clip_vertex_t v = {};
        v.vertex = *source;
        matrix_apply(&v.clip, &source->pos, &device->transform.mvp);
        if (!std::isfinite(v.clip.x) || !std::isfinite(v.clip.y) ||
            !std::isfinite(v.clip.z) || !std::isfinite(v.clip.w))
            return {};
        polygon.push_back(v);
    }

    // Sutherland-Hodgman polygon clipping, preserving UV/color/object-space data.
    for (int plane = 0; plane < 7 && !polygon.empty(); ++plane) {
        vector<clip_vertex_t> input;
        input.swap(polygon);
        clip_vertex_t prev = input.back();
        float dprev = frustum_distance(prev.clip, plane);
        for (const auto &current : input) {
            const float dcurr = frustum_distance(current.clip, plane);
            const bool insidePrev = dprev >= 0.0f;
            const bool insideCurr = dcurr >= 0.0f;
            if (insidePrev != insideCurr) {
                const float t = dprev / (dprev - dcurr);
                polygon.push_back(lerp_clip_vertex(prev, current, t));
            }
            if (insideCurr) polygon.push_back(current);
            prev = current;
            dprev = dcurr;
        }
    }
    return polygon;
}

// Render only the clipped polygon, triangulated as a fan.
void device_draw_triangle(device_t *device,
                          const vertex_t *v1,
                          const vertex_t *v2,
                          const vertex_t *v3) {
    vector<clip_vertex_t> polygon = clip_frustum(device, v1, v2, v3);
    ImGui::Begin("device_draw_triangle");
    ImGui::Text("Clipped vertices: %d", (int)polygon.size());
    ImGui::End();
    if (polygon.size() < 3) return;

    vector<vertex_t> projected;
    projected.reserve(polygon.size());
    for (const auto &v : polygon) {
        vertex_t point = v.vertex;
        // Keep original, undivided attributes until vertex_rhw_init().
        point.spos = v.vertex.pos;
        transform_homogenize(&device->transform, &point.pos, &v.clip);
        point.pos.w = v.clip.w;
        projected.push_back(point);
    }

    if (device->render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
        for (size_t i = 1; i + 1 < projected.size(); ++i)
            device_draw_traps(device, projected[0], projected[i], projected[i + 1]);
    }
    if (device->render_state & RENDER_STATE_WIREFRAME) {
        for (size_t i = 0; i < projected.size(); ++i)
            device_draw_line(device, projected[i].pos,
                             projected[(i + 1) % projected.size()].pos,
                             device->foreground);
    }
}

static void device_draw_traps(device_t *device, vertex_t v1,
                              vertex_t v2, vertex_t v3) {
    vertex_rhw_init(&v1);
    vertex_rhw_init(&v2);
    vertex_rhw_init(&v3);

    trapezoid_t traps[2] = {};
    const int count = trapezoid_init_triangle(traps, &v1, &v2, &v3);
    for (int i = 0; i < count; ++i)
        device_render_trap(device, &traps[i]);
}
