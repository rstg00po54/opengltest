#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "main.h"

// imgui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "SDL2/SDL_ttf.h"
#undef main


// using namespace glm;
using namespace std;
extern vector<vec2> Hodgmanmain(vec2 min, vec2 max, vector<vec2> subjectPolygon);
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* window;

ImGuiIO & imgui_init(SDL_Renderer* renderer) {
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








// 绘制填充圆形的函数
void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 设置绘制颜色为白色
	for (int y = -radius; y <= radius; ++y) {
		for (int x = -radius; x <= radius; ++x) {
			if (x*x + y*y <= radius*radius) { // 如果点在圆内部
				SDL_RenderDrawPoint(renderer, centerX + x, centerY + y); // 绘制点
			}
		}
	}
}
void drawAxes(SDL_Renderer* renderer, int originX, int originY) {
	// 绘制x轴
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 设置颜色为红色
	SDL_RenderDrawLine(renderer, 0, originY, SCREEN_WIDTH, originY); // 绘制x轴线

	// 绘制y轴
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 设置颜色为绿色
	SDL_RenderDrawLine(renderer, originX, 0, originX, SCREEN_HEIGHT); // 绘制y轴线
}


void drawLine(SDL_Renderer* renderer, vec2 p0, vec2 p1)
{
	SDL_RenderDrawLine(renderer, p0.x, SCREEN_HEIGHT-p0.y, p1.x, SCREEN_HEIGHT-p1.y);
	// printf("draw line %f\n", 400-y);
}
void drawTrangle(SDL_Renderer* renderer, vec2 p0, vec2 p1, vec2 p2){
	drawLine(renderer, p0, p1);
	drawLine(renderer, p0, p2);
	drawLine(renderer, p2, p1);
}

/*
	max
min


*/
void drawRect(SDL_Renderer* renderer, vec2 min, vec2 max) {
	SDL_Rect rect;
	rect.x = min.x;
	rect.y = SCREEN_HEIGHT - max.y;
	rect.w = max.x-min.x;
	rect.h = max.y-min.y;
	SDL_RenderDrawRect(renderer, &rect);
}
#define PRINT(X) printf("%f, %f, %f\n", X.x, X.y, X.z)
int main() {
	// 初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL初始化失败: %s\n", SDL_GetError());
		return 1;
	}
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	// 创建窗口
	window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
	if (!window) {
		printf("窗口创建失败: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	// 创建渲染器
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("渲染器创建失败: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	ImGuiIO &io = imgui_init(renderer);
	if (TTF_Init() == -1) {
		pr_debug( "TTF_Init Error: " , TTF_GetError() );
		SDL_Quit();
		return 1;
	}
	// 创建字体
	TTF_Font* font = TTF_OpenFont("arial.ttf", 12);
	// device->font = TTF_OpenFontDPI("simsun.ttc", 16, 500, 500);
	if (font == nullptr) {
		pr_debug( "TTF_OpenFont Error. " ) ;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	SDL_Color textColor = {255, 255, 255};
	// 渲染文本
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "abcd", textColor);
	if (textSurface == nullptr) {
		printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
   SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	if (textureText == nullptr) {
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	// 主循环处理事件
	SDL_Event e;
	bool quit = false;

	ImVec2  downV, v;
	int mouseDown = 0;

	// 定义裁剪窗口
	vec2 min(200, 200, "ld");  // 裁剪窗口的左下角
	vec2 max(400, 400, "ru");  // 裁剪窗口的右上角

	// 定义一个三角形作为要裁剪的多边形
	vector<vec2> subjectPolygon = {
		vec2(150, 400, "p1"),
		vec2(158.064, 300, "p2"),
		vec2(300, 450, "p3")
	};
	vector<vec2> out;

out = Hodgmanmain(min, max, subjectPolygon);

	// return 0;
	while (!quit) {

		while (SDL_PollEvent(&e) != 0) {
			bool ret = ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_MOUSEMOTION) { // 鼠标移动事件
				int x = e.motion.x;
				int y = e.motion.y;
				if(mouseDown) {
					v.x = e.motion.x;
					v.y = e.motion.y;
					// SDL_Log("Mouse Moved: X=%d, Y=%d\n", x, y);
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN) { // 鼠标按下事件
				downV.x = (float)e.motion.x*1.0f+1.0f;
				downV.y = e.motion.y*1.0f;
				mouseDown = 1;
				SDL_Log("Mouse Button downV: X=%f, Y=%f\n", downV.x, downV.y);
				SDL_Log("Mouse Button Down: X=%d, Y=%d\n", e.motion.x, e.motion.y);
				// ImVec2 v;
				// draw_list->AddCircleFilled(v, 5, IM_COL32(255, 255, 0, 255));
			} else if (e.type == SDL_MOUSEBUTTONUP) {  // 鼠标抬起事件
				mouseDown = 0;
				int x = e.button.x;
				int y = e.button.y;
				SDL_Log("Mouse Button Up: X=%d, Y=%d\n", x, y);
			}
		}
		ImVec2 dv,dpoint;
		if (mouseDown) {
			dv.x = v.x-downV.x;
			dv.y = v.y-downV.y;

		}
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::SliderFloat2("p1", (float *)&subjectPolygon[0].x, 0.f, 500.f);
		ImGui::SliderFloat2("p2", (float *)&subjectPolygon[1].x, 0.f, 500.f);
		ImGui::SliderFloat2("p3", (float *)&subjectPolygon[2].x, 0.f, 500.f);
		out = Hodgmanmain(min, max, subjectPolygon);
		ImGui::Text("%d", out.size());
		ImGui::End();

		// 清空屏幕为白色
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 设置颜色为红色

		// drawLine(renderer, vec2(0,0), vec2(100, 200));
		// drawFilledCircle(renderer, 100, 100, 5);



		drawTrangle(renderer, subjectPolygon[0], subjectPolygon[1], subjectPolygon[2]);
		drawRect(renderer, min, max);





		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // 设置颜色为红色


		for(int i = 0;i<(out.size());i++) {
			drawLine(renderer, out[i], out[(i+1)%out.size()]);
		}
		
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		// 更新屏幕
		SDL_RenderPresent(renderer);
		SDL_Delay(20);

	}

	// 释放资源
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
