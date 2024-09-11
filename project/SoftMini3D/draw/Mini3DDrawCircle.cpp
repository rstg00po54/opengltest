#include "Mini3DDrawPanel.h"
#include <math.h>
#include <stdio.h>
static vertex_t mesh[] = {
	//  point_t pos;						 texcoord_t tc; color_t color; float rhw;
	{ { -10, -0,  10, 1 }, { -10, -0,  10, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ {  10, -0,  10, 1 }, {  10, -0,  10, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ {  10, -0, -10, 1 }, {  10, -0, -10, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { -10, -0, -10, 1 }, { -10, -0, -10, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
};
static int loop;



float pointLen(point_t v) {
	float lenght;
	lenght = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return lenght;
}
// 归一化函数
point_t normalize(point_t v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    point_t normalized = { v.x / length, v.y / length, v.z / length , v.w / length};
	normalized.w = 1.f;
	length = sqrt(normalized.x * normalized.x + normalized.y * normalized.y + normalized.z * normalized.z);
	// pr_debug("len: %3.2f\n", length);
    return normalized;
}

// 计算两个顶点的中点
point_t midpoint(point_t v1, point_t v2) {
    point_t mid = { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 , (v1.w + v2.w) / 2 };
    return normalize(mid);
	// return mid;
}

void printVector(vertex_t v1) {
	pr_debug("%3.2f/%3.2f/%3.2f/%3.2f/ %3.2f\n", v1.pos.x, v1.pos.y, v1.pos.z, v1.pos.w, pointLen(v1.pos));
}

// 递归细分三角形
void subdivide(device_t *device, vertex_t v1, vertex_t v2, vertex_t v3, int depth) {
	// pr_debug("------\n");
	if (depth == 0) {
		// 到达细分深度，打印三角形
		// pr_debug("Triangle:\n");
		// printVector(v1);
		// printVector(v2);
		// printVector(v3);
		float length = 10.f;

		v1.pos = normalize(v1.pos);
		v2.pos = normalize(v2.pos);
		v3.pos = normalize(v3.pos);
		v1.spos = v1.pos;
		v2.spos = v2.pos;
		v3.spos = v3.pos;

		v1.tc.u = 0.f;
		v1.tc.v = 0.f;
		v1.color.b = 0.5f;
		v1.color.g = 0.f;
		v1.color.r = 0.f;

		v2.tc.u = 1.f;
		v2.tc.v = 0.f;
		v2.color.b = 0.f;
		v2.color.g = 0.5f;
		v2.color.r = 0.f;


		v3.tc.u = 0.f;
		v3.tc.v = 1.f;
		v3.color.b = 0.f;
		v3.color.g = 0.f;
		v3.color.r = 0.5f;


		v1.pos = vector_dotfloat(&v1.pos, &length);
		v1.pos.w = 1.f;
		v1.spos = v1.pos;

		v2.pos = vector_dotfloat(&v2.pos, &length);
		v2.pos.w = 1.f;
		v2.spos = v2.pos;

		v3.pos = vector_dotfloat(&v3.pos, &length);
		v3.pos.w = 1.f;
		v3.spos = v3.pos;


		device_draw_triangle(device, &v1, &v2, &v3);
		return;
	}

	// 计算每条边的中点并归一化
	vertex_t m12 = v1;
	vertex_t m13 = v2;
	vertex_t m23 = v3;
	m12.pos = midpoint(v1.pos, v2.pos);
	m13.pos = midpoint(v1.pos, v3.pos);
	m23.pos = midpoint(v2.pos, v3.pos);

	m12.spos = m12.pos;
	m13.spos = m13.pos;
	m23.spos = m23.pos;

	v1.pos = normalize(v1.pos);
	v2.pos = normalize(v2.pos);
	v3.pos = normalize(v3.pos);
	v1.spos = v1.pos;
	v2.spos = v2.pos;
	v3.spos = v3.pos;

	// 递归细分生成的四个三角形
	subdivide(device, v1, m12, m13, depth - 1);
	subdivide(device, m12, v2, m23, depth - 1);
	subdivide(device, m13, m23, v3, depth - 1);
	subdivide(device, m12, m13, m23, depth - 1);
}

static void draw_cube4(device_t *device, int a, int b, int c, int d) {

	/*
	0,          0,  sqrt(2/3)/2
	1/2,sqrt(3)/4, -sqrt(2/3)/6
   -1/2,sqrt(3)/4, -sqrt(2/3)/6
	0,  sqrt(3)/2, -sqrt(2/3)/6
	*/
	float length = 10;
	float sqrt3 = sqrt(3.f);
	float sqrt2d3 = sqrt(2.f/3.f);
	float sqrt1d6 = 1.f/sqrt(6.f);

	mesh[0].pos = {		0,		 sqrt2d3,	0, 1};
	mesh[1].pos = { 	0,		-sqrt1d6,	1, 1};
	mesh[2].pos = {	-sqrt3/2,		-sqrt1d6,-1.f/2, 1};
	mesh[3].pos = {	sqrt3/2,		-sqrt1d6, -1.f/2, 1};

	float l0 = vector_length(&mesh[0].pos);
	float l1 = vector_length(&mesh[1].pos);
	float l2 = vector_length(&mesh[2].pos);
	float l3 = vector_length(&mesh[3].pos);

	// for(int i = 0;i<4;i++){
	// 	mesh[i].pos = vector_dotfloat(&mesh[i].pos, &length);
	// 	mesh[i].pos.w = 1.f;
	// 	mesh[i].spos = mesh[i].pos;
	// }

	vertex_t p1 = mesh[a];
	vertex_t p2 = mesh[b];
	vertex_t p3 = mesh[c];
	vertex_t p4 = mesh[d];
	float h = 1.f;
	/*
	v
	|
	p2-p3
	|  |
	p1-p4-u

	p01 p11
	p00 p10

	*/
	float u[2],v[2];

	u[0] = 0;
	u[1] = 1.f;
	v[0] = 0;
	v[1] = 1.f;

	p1.tc.u = u[0];
	p1.tc.v = v[0];

	p2.tc.u = u[0];
	p2.tc.v = v[1];

	p3.tc.u = u[1];
	p3.tc.v = v[1];

	p4.tc.u = u[1];
	p4.tc.v = v[0];
	// device_draw_triangle(device, &p1, &p2, &p3);
	subdivide(device, p1, p2, p3, loop);
	// device_draw_triangle(device, &p3, &p4, &p1);
}
/*
        // 正二十面体顶点公式（度娘可查）
        float m = Mathf.Sqrt(50 - 10 * Mathf.Sqrt(5)) / 10 * size;
        float n = Mathf.Sqrt(50 + 10 * Mathf.Sqrt(5)) / 10 * size;

        // 按公式顺序对顶点坐标赋值
        vertices[0]  = new Vector3(m, 0, n);
        vertices[1]  = new Vector3(m, 0, -n);
        vertices[2]  = new Vector3(-m, 0, n);
        vertices[3]  = new Vector3(-m, 0, -n);
        vertices[4]  = new Vector3(0, n, m);
        vertices[5]  = new Vector3(0, -n, m);
        vertices[6]  = new Vector3(0, n, -m);
        vertices[7]  = new Vector3(0, -n, -m);
        vertices[8]  = new Vector3(n, m, 0);
        vertices[9]  = new Vector3(-n, m, 0);
        vertices[10] = new Vector3(n, -m, 0);
        vertices[11] = new Vector3(-n, -m, 0);

        // 正二十面体三角形的点序列
        triangles = new int[] {6,4,8, 9,4,6, 6,3,9, 6,1,3, 6,8,1,
            8,10,1, 8,0,10, 8,4,0, 4,2,0, 4,9,2,
            9,11,2, 9,3,11, 3,1,7, 1,10,7, 10,0,5,
            0,2,5, 2,11,5, 3,7,11, 5,11,7, 10,5,7};
*/
static void draw_cube20(device_t *device, int a, int b, int c, int d) {
	float m = sqrtf(50.f-10*sqrt(5.f));
	float n = sqrtf(50.f+10*sqrt(5.f));
	float length = 10.f;

	// float length = 10;
	// float sqrt3 = sqrt(3.f);
	// float sqrt2d3 = sqrt(2.f/3.f);
	// float sqrt1d6 = 1.f/sqrt(6.f);

	vertex_t vertices[12];
	vertices[0].pos  = { m,  0,  n, 1};
	vertices[1].pos  = { m,  0, -n, 1};
	vertices[2].pos  = {-m,  0,  n, 1};
	vertices[3].pos  = {-m,  0, -n, 1};
	vertices[4].pos  = { 0,  n,  m, 1};
	vertices[5].pos  = { 0, -n,  m, 1};
	vertices[6].pos  = { 0,  n, -m, 1};
	vertices[7].pos  = { 0, -n, -m, 1};
	vertices[8].pos  = { n,  m,  0, 1};
	vertices[9].pos  = {-n,  m,  0, 1};
	vertices[10].pos = { n, -m,  0, 1};
	vertices[11].pos = {-n, -m,  0, 1};

	int triangles[][3] =  {	6,4,8,  9,4,6,  6,3,9, 6,1,3, 6,8,1,
							8,10,1, 8,0,10, 8,4,0, 4,2,0, 4,9,2,
							9,11,2, 9,3,11, 3,1,7, 1,10,7, 10,0,5,
							0,2,5,  2,11,5, 3,7,11, 5,11,7, 10,5,7};


	for(int i = 0;i<12;i++){
		vertices[i].pos = normalize(vertices[i].pos);

		vertices[i].pos = vector_dotfloat(&vertices[i].pos, &length);
		vertices[i].rhw = 1.f;
		vertices[i].pos.w = 1.f;
		vertices[i].spos = vertices[i].pos;
	}
	for(int i = 0;i<10;i++) {
		vertex_t p1 = vertices[triangles[i][0]];
		vertex_t p2 = vertices[triangles[i][1]];
		vertex_t p3 = vertices[triangles[i][2]];
		subdivide(device, p1, p2, p3, loop);
	}
}


void draw_circle(device_t *device) {
	// pr_debug("--\n");
	// ImGui::Begin("draw_circle");
	// ImGui::Text("111");
	
	ImGui::SliderInt("loop", &loop, 0, 5);
	device->module = 1;
	matrix_set_translate(&device->transform.trans, 0, 0, 0);
	transform_update(&device->transform);
	draw_cube20(device, 0, 1, 2, 3);
	// draw_cube4(device, 0, 1, 2, 3);
	// draw_plane(device, 0, 1, 3, 3);
	// draw_plane(device, 0, 2, 3, 3);
	// draw_plane(device, 1, 2, 3, 3);

	// ImGui::End();

	// device->module = 2;
	// matrix_set_translate(&device->transform.trans, 0, -1, 0);
	// transform_update(&device->transform);
	// draw_plane(device, 0, 1, 2, 3, 0);
}