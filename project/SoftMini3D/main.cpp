//=====================================================================
// 
// mini3d.c - Mini Software Render All-In-One
//
// build:
//   mingw: gcc -O3 mini3d.c -o mini3d.exe -lgdi32
//   msvc:  cl -O2 -nologo mini3d.c 
//
// history:
//   2007.7.01  skywind  create this file as a tutorial
//   2007.7.02  skywind  implementate texture and color render
//   2008.3.15  skywind  fixed a trapezoid issue
//   2015.8.09  skywind  rewrite with more comment
//   2015.8.12  skywind  adjust interfaces for clearity 
// 
//=====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

// #include <windows.h>
// #include <tchar.h>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
typedef unsigned int IUINT32;


#include "Mini3DMath.h"
#include "Mini3DTransform.h"
#include "Mini3DGeometryCalc.h"
//调试打印开关
#define __DEBUG
 
#ifdef __DEBUG

#include <string.h> //strrchr()函数所需头文件

#define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// windows:
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux:
// #define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define pr_debug(format, ...) printf("[%10s][Line: %6d][%s]: \033[32m" format "\033[32;0m\n", filename(__FILE__), __LINE__, __func__, ##__VA_ARGS__)
#define pr_info(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[33m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
#define pr_err(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[31m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
#else
#define normal_info(format, ...)
#define warn_info(format, ...)
#define error_info(format, ...)
#endif




#define MAX_OUTPUT_SIZE 1000

// 全局变量，用于存储输出结果
char output[MAX_OUTPUT_SIZE] = {0};
int output_length = 0;


SDL_Texture* texture;
SDL_Renderer* renderer;
int pitch;

// 定义一个函数，将字符串追加到全局数组中
void append_to_output(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // 在output中追加字符串
    output_length += vsnprintf(output + output_length, MAX_OUTPUT_SIZE - output_length, format, args);

    va_end(args);
}
#define PRINT_POINT(c1) append_to_output(#c1);append_to_output(":%3.2f %3.2f %3.2f %3.f\n", c1.x, c1.y, c1.z, c1.w);
//=====================================================================
// 渲染设备
//=====================================================================
typedef struct {
	transform_t transform;      // 坐标变换器
	int width;                  // 窗口宽度
	int height;                 // 窗口高度
	IUINT32 **framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
	float **zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针
	IUINT32 **texture;          // 纹理：同样是每行索引
	int tex_width;              // 纹理宽度
	int tex_height;             // 纹理高度
	float max_u;                // 纹理最大宽度：tex_width - 1
	float max_v;                // 纹理最大高度：tex_height - 1
	int render_state;           // 渲染状态
	IUINT32 background;         // 背景颜色
	IUINT32 foreground;         // 线框颜色
}	device_t;

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色


// 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
void device_init(device_t *device, int width, int height, void *fb) {
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	device->framebuffer = (IUINT32**)ptr;
	device->zbuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	device->texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;
	if (fb != NULL) framebuf = (char*)fb;
	for (j = 0; j < height; j++) {
		device->framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
		device->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	device->texture[0] = (IUINT32*)ptr;
	device->texture[1] = (IUINT32*)(ptr + 16);
	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0x0c473cff;
	device->foreground = 0;
	transform_init(&device->transform, width, height);
	device->render_state = RENDER_STATE_WIREFRAME;
}



// 设置当前纹理
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h) {
	char *ptr = (char*)bits;
	int j;
	assert(w <= 1024 && h <= 1024);
	for (j = 0; j < h; ptr += pitch, j++) 	// 重新计算每行纹理的指针
		device->texture[j] = (IUINT32*)ptr;
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (float)(w - 1);
	device->max_v = (float)(h - 1);
}

// 清空 framebuffer 和 zbuffer
void device_clear(device_t *device, int mode) {
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++) {
		IUINT32 *dst = device->framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = device->background;
		cc = 0x0c473cff;
		for (x = device->width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < device->height; y++) {
		float *dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

// 画点
void device_pixel(device_t *device, int x, int y, IUINT32 color) {
	void* pixels;
	// pr_debug("in");
	// SDL_LockTexture(texture, NULL, &pixels, &pitch);
	Uint32* pixelData = (Uint32*)pixels;
	if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) {
		device->framebuffer[y][x] = color;
		// pixelData[y][x] = color;
	}
	// SDL_UnlockTexture(texture);
}

// 绘制线段
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c) {
	// pr_debug("%d %d, %d %d", x1, y1, x2, y2);
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {
		device_pixel(device, x1, y1, c);
	}	else if (x1 == x2) {
		int inc = (y1 <= y2)? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(device, x1, y, c);
		device_pixel(device, x2, y2, c);
	}	else if (y1 == y2) {
		int inc = (x1 <= x2)? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(device, x, y1, c);
		device_pixel(device, x2, y2, c);
	}	else {
		int dx = (x1 < x2)? x2 - x1 : x1 - x2;
		int dy = (y1 < y2)? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				device_pixel(device, x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1)? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}	else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				device_pixel(device, x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1)? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
	}
}

// 根据坐标读取纹理
IUINT32 device_texture_read(const device_t *device, float u, float v) {
	int x, y;
	u = u * device->max_u;
	v = v * device->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = CMID(x, 0, device->tex_width - 1);
	y = CMID(y, 0, device->tex_height - 1);
	return device->texture[y][x];
}


//=====================================================================
// 渲染实现
//=====================================================================

// 绘制扫描线
void device_draw_scanline(device_t *device, scanline_t *scanline) {
	IUINT32 *framebuffer = device->framebuffer[scanline->y];
	float *zbuffer = device->zbuffer[scanline->y];
	int x = scanline->x;
	int w = scanline->w;
	int width = device->width;
	int render_state = device->render_state;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width) {
			float rhw = scanline->v.rhw;
			if (rhw >= zbuffer[x]) {	
				float w = 1.0f / rhw;
				zbuffer[x] = rhw;
				if (render_state & RENDER_STATE_COLOR) {
					float r = scanline->v.color.r * w;
					float g = scanline->v.color.g * w;
					float b = scanline->v.color.b * w;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
				if (render_state & RENDER_STATE_TEXTURE) {
					float u = scanline->v.tc.u * w;
					float v = scanline->v.tc.v * w;
					IUINT32 cc = device_texture_read(device, u, v);
					framebuffer[x] = cc;
				}
			}
		}
		vertex_add(&scanline->v, &scanline->step);
		if (x >= width) break;
	}
}

// 主渲染函数
void device_render_trap(device_t *device, trapezoid_t *trap) {
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < device->height) {
			trapezoid_edge_interp(trap, (float)j + 0.5f);
			trapezoid_init_scan_line(trap, &scanline, j);
			device_draw_scanline(device, &scanline);
		}
		if (j >= device->height) break;
	}
}
char buffer_text[1024];

// #define PRINT_POINT(c1) printf(#c1);printf(":%f %f %f\n", c1.x, c1.y, c1.z);
// #define PRINTF_NAME_VALUE(X) {sprintf(&buffer_text[strlen(buffer_text)-2], #X); sprintf(buffer_text, "   %f %f %f\n", X.x, X.y, X.z);}
// 根据 render_state 绘制原始三角形
void device_draw_triangle(device_t *device,
	const vertex_t *v1,
	const vertex_t *v2,
	const vertex_t *v3) {
	point_t p1, p2, p3, c1, c2, c3;

	int render_state = device->render_state;
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
	// if (transform_check_cvv(&c1) != 0) return;
	// if (transform_check_cvv(&c2) != 0) return;
	// if (transform_check_cvv(&c3) != 0) return;

	// 归一化
	transform_homogenize(&device->transform, &p1, &c1);
	transform_homogenize(&device->transform, &p2, &c2);
	transform_homogenize(&device->transform, &p3, &c3);
	PRINT_POINT(p1);
	PRINT_POINT(p2);
	PRINT_POINT(p3);

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

		if (n >= 1) device_render_trap(device, &traps[0]);
		if (n >= 2) device_render_trap(device, &traps[1]);
	}

	if (render_state & RENDER_STATE_WIREFRAME) {		// 线框绘制
		device_draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->foreground);
		device_draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->foreground);
		device_draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->foreground);
	}
}


//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
unsigned char *screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

int screen_init(int w, int h, const char *title);	// 屏幕初始化

SDL_Window* window;
// ImGuiIO &io;
int screen_init(int w, int h, const char *title) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
	unsigned char **p = &screen_fb;
	SDL_LockTexture(texture, NULL, (void **)p, &pitch);
	SDL_UnlockTexture(texture);
	return 0;
}
ImGuiIO & imgui_init() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
	return io;
}
//=====================================================================
// 主程序
//=====================================================================
vertex_t mesh[] = {
	//  point_t pos; texcoord_t tc; color_t color; float rhw;
	{ { -1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	{ {  1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	{ {  1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	{ { -1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	{ { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	{ {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
	{ {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
	{ { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	{ {  0,  0,  0, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};

void draw_plane(device_t *device, int a, int b, int c, int d) {
	vertex_t p1 = mesh[a];
	vertex_t p2 = mesh[b];
	vertex_t p3 = mesh[c];
	vertex_t p4 = mesh[d];
	p1.tc.u = 0;
	p1.tc.v = 0;
	p2.tc.u = 0;
	p2.tc.v = 1;
	p3.tc.u = 1;
	p3.tc.v = 1;
	p4.tc.u = 1;
	p4.tc.v = 0;
	device_draw_triangle(device, &p1, &p2, &p3);
	device_draw_triangle(device, &p3, &p4, &p1);
}
#define PRINT_M(ts) \
	printf(#ts"\n");\
	for(int i = 0;i<4;i++){\
		printf("%f, %f, %f, %f\n",\
			ts.m[i][0],\
			ts.m[i][1],\
			ts.m[i][2],\
			ts.m[i][3]);\
	}
void draw_box(device_t *device, float theta) {
	// pr_debug("in");
	matrix_t m;
	// matrix_set_rotate(&m, 1, 1, 1, 0);
	// matrix_set_identity(&m);
	// matrix_set_rotate(&m, -1, -0.5, 1, theta);
	// device->transform.world = m;
	// transform_update(&device->transform);
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 7, 6, 5, 4);
	draw_plane(device, 0, 4, 5, 1);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 3, 7, 4, 0);
	// for(int i=0;i<4;i++){
	// 	for(int j=0;j<4;j++){
	// 		printf("%d %d = %f\n", i, j, m.m[i][j]);
	// 	}

	// }



	
		// vertex_t p1 = mesh[i];
		point_t p[2];
		point_t po[2];
		matrix_t m_out[2];
		point_t psrc[][2] = {
			{{0,0,0,1},{100,0,0,1}},
			{{0,0,0,1},{0,100,0,1}},
			{{0,0,0,1},{0,0,100,1}},

			{{1,1,0,1},{1,1,-100,1}},
			{{1,-1,0,1},{1,-1,-100,1}},

			{{1,0,1,1},{1,-100,1,1}},
			{{-1,0,1,1},{-1,-100,1,1}},

			{{1,1,1,1},{-100,1,1,1}},
			{{1,-1,1,1},{-100,-1,1,1}}
			
			};
		for(int i = 0;i<9;i++) {

		
			// matrix_mul(&m_out[0], &device->transform.projection, &device->transform.mvp);
			matrix_apply( &p[0], &psrc[i][0], &device->transform.mvp);
			matrix_apply( &p[1], &psrc[i][1], &device->transform.mvp);

			// 归一化
			transform_homogenize(&device->transform, &po[0], &p[0]);
			transform_homogenize(&device->transform, &po[1], &p[1]);
			// PRINT_POINT(p);
			// PRINT_POINT(po);

			// printf("% 3.2f % 3.2f % 3.2f % 3.2f\t", p.x, p.y, p.z, p.w);
			// printf("% 3.2f % 3.2f % 3.2f % 3.2f\n", po.x, po.y, po.z, po.w);
			// SDL_RenderDrawLine(renderer,0,0,po.x, po.y);
			device_draw_line(device, po[1].x, po[1].y, po[0].x, po[0].y, 0xffffffff);
		}

}

// void camera_at_zero(device_t *device, float x, float y, float z) {
// void camera_at_zero(device_t *device, point_t camera, point_t target, point_t up) {
// 	// point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };

// 	//camera target up
// 	matrix_set_lookat(&device->transform.view, &camera, &target, &up);
// 	transform_update(&device->transform);
// }

void init_texture(device_t *device) {
	static IUINT32 texture[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			texture[j][i] = ((x + y) & 1)? 0xffffffff : 0xe3c08cff;
		}
	}
	device_set_texture(device, texture, 256 * 4, 256, 256);
}
#undef main
int main(void)
{
	device_t device;
	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int indicator = 0;
	int kbhit = 0;
	float rotate = 0;
	float fovy = 45.f;
	point_t scale = {1, 1, 1};
	point_t trans = {0, 0, 0};
	// float pos = 3.5;

	char title[] = "Mini3d (software render tutorial) ";
		// _T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title)) 
		return -1;
	ImGuiIO &io = imgui_init();

	device_init(&device, 800, 600, screen_fb);
	// camera_at_zero(&device, 3, 0, 0);

	init_texture(&device);
	device.render_state = RENDER_STATE_TEXTURE;

	matrix_t m;
	bool quit = false;
	SDL_Event e;
	SDL_Keycode key;
	vector_t pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = -1;
	pos.w = 1;
	point_t camera  = { 0, 0, -3, 1 };//相机位置eye
	point_t target  = { 0, 0, 0, 1 };//观察目标 at
	point_t up  = { 0, 1, 0, 1 };
	int mouse_state;
	int downX, downY;
	int dx, dy;
	float thetax = 0.f,thetay = 0.f;
	float dthetax,dthetay;
	while (!quit) {
		
		// printf("\ec");
		// system("cls");
		// printf("==============================\n");
		while (SDL_PollEvent(&e) != 0) {
			bool ret = ImGui_ImplSDL2_ProcessEvent(&e);
			// printf("ret %d \n", ret);
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			if (io.WantCaptureMouse) {
				// ImGui正在使用鼠标事件
				// 在这里添加处理ImGui使用鼠标事件的逻辑
				printf("use mouse\n");
			} else {
				// ImGui没有使用鼠标事件
				// 在这里添加处理应用程序自身的鼠标事件逻辑

			// printf("event %d\n", event.type);
			switch (e.type) {
				case SDL_MOUSEMOTION:
					// 处理鼠标移动事件
					if(mouse_state) {

						dx = e.motion.x-downX;
						dy = e.motion.y-downY;
						// append_to_output("mouse move %d,%d\n", e.motion.x, e.motion.y);
						// printf("mouse move %d,%d\n", dx, dy);
						// float dtheta = dx*0.01f;
						thetax = -dx*0.01f+dthetax;
						camera.x = cosf(thetax)*3.0f;
						camera.z = sinf(thetax)*3.0f;
						camera.y = cosf(thetax)*3.0f;
						// printf("theta %f\n", theta);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					// 处理鼠标按下事件
					printf("mouse down %d %d\n", e.motion.x, e.motion.y);
					downX = e.motion.x;
					downY = e.motion.y;
					mouse_state = 1;
					dthetax = thetax;
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_state = 0;
					printf("mouse up\n");
					// g_touch_button = 0;
					// 处理鼠标释放事件
					break;
				case SDL_KEYDOWN:
					// 按下键盘按键
					printf("Key pressed: %d\n", e.key.keysym.sym);
					// key_code = event.key.keysym.sym;
					break;
				case SDL_KEYUP:
					// 松开键盘按键
					printf("Key released: %d\n", e.key.keysym.sym);
					key = e.key.keysym.sym;
					break;
				default:
					append_to_output("mouse move\n");
					break;
			}
			}
		}
		// append_to_output("mouse move \n");
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		// ImGui::Text(output);

		// printf("%s", output);
		output_length = 0;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		// for(int jj = 0;jj<10;jj++)
		// ImGui::Text("this is first ");
		ImGui::SliderFloat3("pos", (float *)&pos, -3.0f, 3.0f);
		ImGui::SameLine();
		if(ImGui::Button("reset")) {
			pos.x = pos.y = pos.z = 0;
			pos.w = 1;
		}
		ImGui::SliderFloat("alpha", (float *)&rotate, 0.0f, 5.0f);
		ImGui::SliderFloat("fovy", (float *)&fovy, 30.0f, 60.0f);
			// point_t p1, p2, p3, c1, c2, c3;
		ImGui::SliderFloat3("camera", (float *)&camera, -20.0f, 0.0f);
		ImGui::SliderFloat3("scale", (float *)&scale, 0.0f, 3.0f);
		ImGui::SliderFloat3("trans", (float *)&trans, -3.0f, 3.0f);
		ImGui::End();
		if(mouse_state){


		}
		// Rend
		// camera.x = sqrt(9-powf(camera.y, 2));

		// screen_dispatch();
		device_clear(&device, 1);
	//camera target up
		matrix_set_lookat(&device.transform.view, &camera, &target, &up);
		// transform_update(&device.transform);

		matrix_t m_scale, m_rotate, m_trans, m_out;
		transform_t *t = &device.transform;

		matrix_set_rotate(&t->model, 0, 1, 0, rotate);
		matrix_set_lookat(&t->view, &camera, &target, &up);
		// transform_update(&device.transform);
//  缩放 -> 旋转 -> 平移 
		matrix_set_scale(&t->scale, scale.x, scale.y, scale.z);
		matrix_set_rotate(&t->rotate, 0, 1, 0, rotate);
		matrix_set_translate(&t->trans, trans.x, trans.y, trans.z);

		matrix_mul(&m_out, &t->rotate, &t->scale);
		// matrix_mul(&m_out, &m_scale, &m_rotate);
		matrix_mul(&t->model, &t->trans, &m_out);
		float aspect = (float)800 / ((float)600);
		matrix_set_perspective(&t->projection, fovy, aspect, 1.0f, 100.0f);
{
		vertex_t p1 = mesh[8];
		static point_t p,pr,po;
		matrix_apply( &p, &p1.pos, &device.transform.mvp);
		// matrix_apply_r( &pr, &p1.pos, &device.transform.transform);

		// 归一化
		transform_homogenize(&device.transform, &po, &p);
		// device.transform.world = m;
		transform_update(&device.transform);
		ImGui::Begin("trans");
		ImGui::Text("scale");
		ImGui::SliderFloat4("m_scale", (float *)&m_scale.m[0], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_scale", (float *)&m_scale.m[1], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_scale", (float *)&m_scale.m[2], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_scale", (float *)&m_scale.m[3], 0.0f, 5.0f);

		ImGui::Text("trans");
		ImGui::SliderFloat4("m_trans", (float *)&m_trans.m[0], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_trans", (float *)&m_trans.m[1], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_trans", (float *)&m_trans.m[2], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_trans", (float *)&m_trans.m[3], 0.0f, 5.0f);


		ImGui::SliderFloat4("m_rotate", (float *)&m_rotate.m[0], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_rotate", (float *)&m_rotate.m[1], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_rotate", (float *)&m_rotate.m[2], 0.0f, 5.0f);
		ImGui::SliderFloat4("m_rotate", (float *)&m_rotate.m[3], 0.0f, 5.0f);


		ImGui::SliderFloat4("trans", (float *)&device.transform.mvp.m[0], 0.0f, 5.0f);
		ImGui::SliderFloat4("trans", (float *)&device.transform.mvp.m[1], 0.0f, 5.0f);
		ImGui::SliderFloat4("trans", (float *)&device.transform.mvp.m[2], 0.0f, 5.0f);
		ImGui::SliderFloat4("trans", (float *)&device.transform.mvp.m[3], 0.0f, 5.0f);
		ImGui::SliderFloat4("po", (float *)&po, 0.0f, 5.0f);
		// ImGui::SliderFloat4("pr", (float *)&pr, 0.0f, 5.0f);
		ImGui::End();
}


		if (key == SDLK_SPACE) {
			key = 0;
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				device.render_state = states[indicator];
			}
		} else {
			kbhit = 0;
		}
		key = 0;

		// screen_update();
#if 1
		// 锁定纹理以获取像素数据
		void* pixels;
		int pitch;
		SDL_LockTexture(texture, NULL, &pixels, &pitch);
		Uint32* pixelData = (Uint32*)pixels;
		// printf("%p\n", pixelData);
		draw_box(&device, rotate);
		// device_draw_line(&device, 0, 0, 100, 100, device.foreground);
		// 绘制一个绿色的矩形
		// for (int y = 100; y < 200; ++y) {
		// 	for (int x = 100; x < 200; ++x) {
		// 		int index = y * (pitch / sizeof(Uint32)) + x;
		// 		pixelData[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, y, x);
		// 	}
		// }
		// 解锁纹理
		SDL_UnlockTexture(texture);
#endif
		ImGui::Render();
		// SDL_Delay(30);
		// 渲染纹理
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
		// SDL_Delay(300);

	}
	return 0;
}

