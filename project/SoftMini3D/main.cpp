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
#include <string.h> //strrchr()函数所需头文件
#include <iostream>
#include <fstream>
// #include <windows.h>
// #include <tchar.h>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


#include "Mini3DCalcLines.h"
#include "Mini3DMath.h"
#include "Mini3DTransform.h"
#include "Mini3DGeometryCalc.h"
#include "Mini3Dbmp.h"
#include "Mini3DDevice.h"
#include "Mini3D.h"
#include "Mini3DRender.h"
#include "Mini3DDrawCube.h"
#include "Mini3DDrawPanel.h"
#include "Mini3DDrawCircle.h"
#include "SDL2/SDL_ttf.h"
// //调试打印开关
// #define __DEBUG
 
// #ifdef __DEBUG


// #define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// // windows:
// #define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// // linux:
// // #define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// #define pr_debug(format, ...) printf("[%10s][Line: %6d][%s]: \033[32m" format "\033[32;0m\n", filename(__FILE__), __LINE__, __func__, ##__VA_ARGS__)
// #define pr_info(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[33m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
// #define pr_err(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[31m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
// #else
// #define normal_info(format, ...)
// #define warn_info(format, ...)
// #define error_info(format, ...)
// #endif




#define MAX_OUTPUT_SIZE 1000
#define SCREEN_W	1440
#define SCREEN_H	600

// 全局变量，用于存储输出结果
char output[MAX_OUTPUT_SIZE] = {0};
int output_length = 0;

char buffer_text[1024];
static void DumpSurfaceToFile(SDL_Surface* surface, const char* filename) ;
// SDL_Texture* texture;
SDL_Texture* textureText;
// SDL_Renderer* renderer;
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
// #define PRINTF_POINT(c1) printf(#c1);printf(":% 3.2f % 3.2f % 3.2f % 3.2f\n", c1.x, c1.y, c1.z, c1.w);
#define PRINTF_POINT(c1) 
IUINT32 device_texture_read(const device_t *device, float u, float v);

//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
// unsigned char *screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

// int screen_init(int w, int h, const char *title, unsigned char **screen_fb);	// 屏幕初始化

SDL_Window* window;
// ImGuiIO &io;
int screen_init(device_t *device, int w, int h, const char *title, unsigned char **screen_fb) {
	SDL_Init(SDL_INIT_VIDEO);
	if (TTF_Init() == -1) {
		pr_debug( "TTF_Init Error: " , TTF_GetError() );
		SDL_Quit();
		return 1;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	device->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	device->sdltexture = SDL_CreateTexture(device->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);

	// 创建字体
    device->font = TTF_OpenFont("arial.ttf", 50);
    // device->font = TTF_OpenFontDPI("simsun.ttc", 16, 500, 500);
	if (device->font == nullptr) {
        pr_debug( "TTF_OpenFont Error: " ) ;
        SDL_DestroyRenderer(device->renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
// TTF_SetFontHinting(font, TTF_HINTING_LIGHT );
	    // 创建字体颜色
    SDL_Color textColor = {255, 255, 255}; // 白色

    // 渲染文本
    SDL_Surface* textSurface = TTF_RenderText_Solid(device->font, "abcd", textColor);
    if (textSurface == nullptr) {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        TTF_CloseFont(device->font);
        SDL_DestroyRenderer(device->renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
   textureText = SDL_CreateTextureFromSurface(device->renderer, textSurface);
   DumpSurfaceToFile(textSurface, "main.yuv");
    SDL_FreeSurface(textSurface);
    if (textureText == nullptr) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        TTF_CloseFont(device->font);
        SDL_DestroyRenderer(device->renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

	// unsigned char **p = &screen_fb;
	SDL_LockTexture(device->sdltexture, NULL, (void **)screen_fb, &pitch);
	SDL_UnlockTexture(device->sdltexture);
	printf("buffer %p\n", *screen_fb);
	return 0;
}
ImGuiIO & imgui_init(device_t *dev) {
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
	ImGui_ImplSDL2_InitForSDLRenderer(window, dev->renderer);
	ImGui_ImplSDLRenderer2_Init(dev->renderer);
	return io;
}

/*

	 ____
	/   /|
	---- |
	|  | /
	|  |/
	----


*/
//=====================================================================
// 主程序
//=====================================================================

static void DumpSurfaceToFile(SDL_Surface* surface, const char* filename) {
    if (!surface) {
        printf("Surface is null\n");
        return;
    }
SDL_LockSurface(surface);
    // 获取像素数据
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch; // 每行字节数
    int width = surface->w;     // 宽度（像素）
    int height = surface->h;    // 高度（像素）
	int BytesPerPixel = surface->format->BytesPerPixel;
	char bufname[100];
	sprintf(bufname, "main_%dx%x_%d.yuv", width, height, pitch);

    // 打开输出文件
    std::ofstream file(bufname, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        printf("Failed to open file %s\n", bufname);
        return;
    }


	pr_debug("pitch %d, width %dx%d BytesPerPixel %d \n", pitch, width, height, BytesPerPixel);

    // 写入每一行的像素数据
    for (int y = 0; y < height; ++y) {
        file.write(reinterpret_cast<const char*>(pixels + ( surface->pitch*y/4)), pitch);
    }

    // 关闭文件
    file.close();
	 SDL_UnlockSurface(surface);
    printf("Surface data dumped to file %s\n", filename);
}

// 根据坐标读取纹理

// 设置当前纹理
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h) {
	char *ptr = (char*)bits;
	int j;
	// assert(w <= 1024 && h <= 1024);
	for (j = 0; j < h; ptr += pitch, j++) {
		device->texture[j] = (IUINT32*)ptr;
		if(j<5)
		printf("j %d %p\n", j, ptr);
	} 	// 重新计算每行纹理的指针
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (float)(w - 1);
	device->max_v = (float)(h - 1);
}
#define TEXTURE_W 256
#define TEXTURE_H 256
/*
纹理加字母
*/

static void drawCharAtTexture(device_t *device, void *texture) {

	int *ptr = (int*)texture;
	SDL_Color textColor = {255, 255, 255}; // 白色
	TTF_SetFontSize(device->font, 20);
	TTF_SetFontStyle(device->font, TTF_STYLE_BOLD);

	for(int c = 0;c<64;c++) {

		char buf[5];
		sprintf(buf, "%d%d", c/8, c%8);
		SDL_Surface* textSurface = TTF_RenderText_Solid(device->font, buf, textColor);
		pr_debug("init text \n");
		if (SDL_MUSTLOCK(textSurface)) {
			SDL_LockSurface(textSurface);
		}
		uint8_t* pixels = (uint8_t*)textSurface->pixels;
		int pitch = textSurface->pitch;//48
		int width = textSurface->w;//25
		int height = textSurface->h;//56
		int xoffset = (c%8)*32;
		int yoffset = (c/8)*32;

		xoffset = xoffset + (32-width)/2;
		yoffset = yoffset + (32-height)/2;
		/*
			256x256 8行8列
			256/8=32
		*/
		pr_debug("pitch %d, width %dx%d\n", pitch, width, height);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				uint8_t pixel = pixels[(y * pitch ) + x];
				if(pixel> 0){
					// device_pixel(device, xpos+x,ypos+y, 0xffffff);//SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_BGR888), 255, 255, 255));
					ptr[x+xoffset+(y+yoffset)*256] = 0;
				}
			}
		}
		if (SDL_MUSTLOCK(textSurface)) {
			SDL_UnlockSurface(textSurface);
		}
		SDL_FreeSurface(textSurface);
	}
}
// 0xe3c08cff 1024=0x400
/*
int a[3][2]
a0	0 0
a1	0 0
a2	0 0
*/
void init_texture(device_t *device, uint8_t *buffer) {
	static IUINT32 texture[TEXTURE_H][TEXTURE_W];//256*128*4 = 128k
	int i, j;
	printf("sizeof %d\n", sizeof(IUINT32));
	for (j = 0; j < TEXTURE_H; j++) {
		for (i = 0; i < TEXTURE_W; i++) {
			int x = i / 32, y = j / 32;
			uint8_t r = 256-i;
			int c = (r<<8)|(r<<16)|(r<<24);
			c = 0xffffff;
			texture[j][i] = ((x + y) & 1)? c : 0xe3c08cff;//8*16=128

			// texture[j][i] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB24), 
			// 	buffer[j*898+i*3], 
			// 	buffer[j*898+i*3+1], 
			// 	buffer[j*898+i*3+2]);
			// texture[j][i] = buffer[j*256+i];
			// if(texture[j][i] == 0){
			// 	printf("error\n");
			// }
		}
		if(j<5)
			printf("texture[%d] = %p\n", j, texture[j]);
	}
	drawCharAtTexture(device, texture);
	device_set_texture(device, texture, TEXTURE_W * 4, TEXTURE_W, TEXTURE_H);
	printf("%s ---\n", __func__);
}
union FloatInt
{
	int    i;
	float           f;
};
int FloatToInt(float f)
{
	FloatInt ret, bias;
	ret.f = f;
	bias.i = (23 + 127) << 23;
	if(f < 0.0f){bias.i = ((23 + 127) << 23) + (1 << 22);}
	ret.f += bias.f;
	ret.i -= bias.i;
	return ret.i;
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
	point_t scale = {1.f, 1.f, 1.f};
	point_t trans = {0, 0, 0};
	// float pos = 3.5;
	BMP_FILE_HEADER bmpFileHeader_p;
	BMP_INFO_HEADER bmpInfoHeader_p;
	unsigned char *screen_fb;
	// uint8_t *bmpBuffer;
	// readBmpFromFile("./skin_debug.bmp", &bmpFileHeader_p, &bmpInfoHeader_p, &bmpBuffer);
	// readBmpFromFile("./ts0.bmp", &bmpFileHeader_p, &bmpInfoHeader_p, &device.bmpBuffer);
	readBmpFromFile("./skin_debug.bmp", &bmpFileHeader_p, &bmpInfoHeader_p, &device.bmpBuffer);
	displayBmpHeader( &bmpFileHeader_p, &bmpInfoHeader_p);
	char title[] = "Mini3d (software render tutorial) ";
		// _T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(&device, SCREEN_W, SCREEN_H, title, &screen_fb)) 
		return -1;
	printf("buffer %p\n", screen_fb);
	ImGuiIO &io = imgui_init(&device);

	ImFont* font = io.Fonts->AddFontFromFileTTF(
		"simsun.ttc", 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	device_init(&device, SCREEN_W, SCREEN_H, screen_fb);
	// camera_at_zero(&device, 3, 0, 0);

	init_texture(&device, device.bmpBuffer);

	device.drawlines = 16;
	device.drawplane = 16;

	matrix_t m;
	bool quit = false;
	SDL_Event e;
	SDL_Keycode key;
	vector_t pos = {0,0,-1,1};
	point_t camera  = { 0, 10, 10, 1 };//相机位置eye
	point_t target  = { 0, 0, 0, 1 };//观察目标 at
	point_t up  = { 0, 1, 0, 1 };
	int mouse_state = 0;
	int downX, downY;
	int dx = 0, dy = 0;
	float thetax = 0.f,thetay = 0.f;
	float dthetax,dthetay;
	float r = camera.z;

	init_cube();
 ImVec4 color = ImVec4(0.5f, 0.5f, 0.5f, 0.5f); // Initial color

	// return 0;
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
				// printf("use mouse\n");
			} else {
				// ImGui没有使用鼠标事件
				// 在这里添加处理应用程序自身的鼠标事件逻辑

				// printf("event %d\n", event.type);
				switch (e.type) {
					case SDL_MOUSEWHEEL:
						// printf("x ==-= %d\n",e.wheel.y);
						r+=(e.wheel.y*0.1f);
							// thetax = dx*0.01f+thetax;
							// thetay = dy*0.01+thetay;
						if(r>0.15f){


							camera.y = r*sinf(thetay);
							camera.x = -r*cosf(thetay)*sinf(thetax);
							camera.z = r*cosf(thetay)*cosf(thetax);
						}else{
							r = 0.15f;
						}
							// printf("thetax %f/%f\n", thetax, thetay);
						break;
					case SDL_MOUSEMOTION:
						// 处理鼠标移动事件
						if(mouse_state) {
							// float r = 3.f;
							dx = e.motion.x-downX;
							dy = e.motion.y-downY;

							thetax = dx*0.01f+dthetax;
							thetay = dy*0.01f+dthetay;

							if(thetay>1.57)
								thetay = 1.57;
							if(thetay<-1.57)
								thetay = -1.57;



							camera.y = r*sinf(thetay);
							camera.x = -r*cosf(thetay)*sinf(thetax);
							camera.z = r*cosf(thetay)*cosf(thetax);

							float ra = sqrtf(pow(camera.x, 2.f)+pow(camera.y, 2.f)+pow(camera.z, 2.f));

							// printf("mouse move %d %d, theta % 3.2f/% 3.2f, ra %3.2f\n", e.motion.x, e.motion.y, thetax, thetay, ra);
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						// 处理鼠标按下事件
						printf("mouse down %d %d\n", e.motion.x, e.motion.y);
						downX = e.motion.x;
						downY = e.motion.y;
						mouse_state = 1;
						dthetax = thetax;
						dthetay = thetay;
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
		if (key == SDLK_SPACE) {
			key = 0;
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				// device.render_state = states[indicator];
			}
		} else {
			kbhit = 0;
		}
		key = 0;
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
		ImGui::Text("中文集");
		// for(int jj = 0;jj<10;jj++)
		// ImGui::Text("this is first ");
		ImGui::SliderFloat3("pos", (float *)&pos, -3.0f, 3.0f);
		ImGui::SameLine();
		if(ImGui::Button("reset")) {
			pos.x = pos.y = pos.z = 0;
			pos.w = 1;
		}
 		if (ImGui::CollapsingHeader("Configuration")){
			ImGui::SliderFloat3("target", (float *)&target, -10.f, 10.0f);
			ImGui::SliderFloat3("scale", (float *)&scale, 0.2f, 2.0f);
			ImGui::SliderFloat3("trans", (float *)&trans, -3.0f, 3.0f);
			ImGui::SliderFloat("rotate", (float *)&rotate, 0.0f, 360.0f);
			ImGui::SliderFloat("fovy", (float *)&fovy, 0.0f, 90.0f);
			ImGui::SliderFloat("r", (float *)&r, 0.0f, 90.0f);
				// point_t p1, p2, p3, c1, c2, c3;
			ImGui::SliderFloat3("camera", (float *)&camera, -10.0f, 10.0f);
			ImGui::SameLine();
			if(ImGui::Button("reset1")) {
				camera.x = camera.y = 0;
				camera.w = 1;
				camera.z = 3;
				thetay = dthetax = 0.f;
				thetax = dthetay = 0.f;
				printf("camera reset\n");
			}
 		}


		char *items[] = {(char *)"渲染纹理", (char *)"渲染颜色", (char *)"渲染线框"};
		static int curIndex = 2;

		ImGui::Combo("mode", &curIndex, items, 3);

		ImGui::SliderFloat4("LightDir", (float *)&device.LightDir, -10, 10);
		ImGui::SliderFloat4("LightPoint", (float *)&device.LightPoint, -10, 10);


		ImGui::ColorEdit4("Color Picker", (float *)&color);
		int value = ((int)(color.x * 255)<<16)|((int)(color.y * 255)<<8)|((int)(color.z * 255)<<0);
		device.color = color;
		ImGui::Text("cur %3.2f/%3.2f/%3.2f/%x", color.x, color.y, color.z, value);

// #define RENDER_STATE_WIREFRAME      1		// 渲染线框
// #define RENDER_STATE_TEXTURE        2		// 渲染纹理
// #define RENDER_STATE_COLOR          4		// 渲染颜色
		device.render_state = states[curIndex];
		if(mouse_state){


		}
		// Rend
		// camera.x = sqrt(9-powf(camera.y, 2));

		// screen_dispatch();
		device_clear(&device, 1);
	//camera target up
		// matrix_set_lookat(&device.transform.view, &camera, &target, &up);
		// transform_update(&device.transform);
		transform_t *t = &device.transform;

		Uint32 currentTime = SDL_GetTicks();

//  缩放 -> 旋转 -> 平移 

		// 锁定纹理以获取像素数据
		void* pixels;
		int pitch;
		SDL_LockTexture(device.sdltexture, NULL, &pixels, &pitch);
		Uint32* pixelData = (Uint32*)pixels;
		// printf("%p\n", pixelData);
		matrix_set_scale(&t->scale, scale.x/2, scale.y/2, scale.z/2);
		matrix_set_rotate(&t->rotate, 0, 1, 0,  currentTime*0.04f);
		matrix_set_rotate(&t->rotate, 0, 1, 0,  rotate);
		matrix_set_perspective(&t->projection, fovy, device.aspect_ratio, 0.1f, 100.0f);
		matrix_set_lookat(&t->view, &camera, &target, &up);
		transform_update(t);




// #define DRAW_1CUBE
#ifdef DRAW_1CUBE
		float rotate_box = 0.f;;
		static char rrr = 0;
		static int time0 = 0.f;
		static float cube1_r = 0.f;
		float sp = 0.06f;
		if(ImGui::Button("旋转U")) {
			rrr = 'U';
			time0 = currentTime;//ms
		}
		ImGui::SameLine();
		if(ImGui::Button("旋转D")) {
			rrr = 'D';
			time0 = currentTime;//ms
		}
		if(rrr){
			uint32_t diff = currentTime - time0;
			if((diff*sp)<90.f){
				rotate_box = cube1_r+diff*sp;
			}else{
				cube1_r = cube1_r+90.f;
				rotate_box = cube1_r;
				rrr = 0;
			}
		}else{
			rotate_box = cube1_r;
		}
		
		ImGui::SliderFloat("rotate_box", (float *)&rotate_box, -10.f, 10.0f);
#endif
		ImGui::CheckboxFlags("cube", &device.drawcube, ImGuiConfigFlags_NoMouse);
		ImGui::SameLine();
		ImGui::CheckboxFlags("line", &device.drawlines, ImGuiConfigFlags_NoMouse);
		ImGui::SameLine();
		ImGui::CheckboxFlags("circle", &device.drawcircle, ImGuiConfigFlags_NoMouse);
		ImGui::SameLine();
		ImGui::CheckboxFlags("palne", &device.drawplane, ImGuiConfigFlags_NoMouse);
		// draw_cube(&device);
		if(device.drawlines)
			drawMyLines(&device);
		if(device.drawcircle)
			draw_circle(&device);
		if(device.drawcube)
			draw_box(&device);
		if(device.drawplane)
			draw_panel(&device);


		matrix_set_translate(&t->trans, trans.x, trans.y, trans.z);
		transform_update(t);
		// point_t pt = drawMyLines(&device);
		// drawMyLines(&device);

		// device_draw_line(&device, 0, 0, 100, 100, device.foreground);
		// 绘制一个绿色的矩形 800*600 pitch = 3200 = 800*4
#if 0
		for (int y = 0; y < device.height; ++y) {
			for (int x = 0; x < 100; ++x) {
				int index = y * (pitch / sizeof(Uint32)) + x;
				int offset = y*300*3*3+x*3*3;
				// pixelData[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, y, x);
				pixelData[y*device.width+x] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_BGR888), //SDL_PIXELFORMAT_RGBA8888
					device.bmpBuffer[offset], 
					device.bmpBuffer[offset+1], 
					device.bmpBuffer[offset+2]);//bmpBuffer[y*300*3+x*3];
					// if(x == 5)
				// printf("0x%x, x %d, y %d\n", pixelData[y*device.width+x],x,y);
			}
		}
#endif
		ImGui::End();

				// printf("index = %d\n", pitch);
		// 解锁纹理
		SDL_UnlockTexture(device.sdltexture);

		ImGui::Render();
		// SDL_Delay(30);
		// 渲染纹理
		SDL_RenderClear(device.renderer);
		SDL_RenderCopy(device.renderer, device.sdltexture, nullptr, nullptr);

		// 设置文本的位置和大小
        // SDL_Rect renderQuad = {pt.x, pt.y, 16*4, 16}; // x, y, w, h
        // SDL_RenderCopy(device.renderer, textureText, nullptr, &renderQuad);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(device.renderer);
		// SDL_Delay(300);

	}
	return 0;
}

