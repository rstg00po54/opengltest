#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// imgui
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#undef main
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
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
// 在SDL中，原点（0,0）通常位于窗口的左上角，x轴向右增长，y轴向下增长
void drawself(SDL_Renderer* renderer)
{
	int i,j;
	SDL_RenderSetScale(renderer, 1, -1);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 480;
	SDL_RenderSetViewport(renderer, &rect);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
	  // 定义原始位置向量
    // glm::vec2 position(150.0f, 0.0f);
	




	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	// SDL_RenderDrawPoint(renderer, 320+position.x, 480-(240+position.y));

	// 红色
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	// SDL_RenderDrawPoint(renderer, 200+v3.x, 200+v3.y);
	SDL_RenderDrawPoint(renderer, 320+10, 480-(240+10));

	drawAxes(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	for(j=0;j<3;j++){
		glm::vec2 position(20.0f+j*2.0f, 50.0f*j);
		for(i=0;i<20;i++){
			glm::vec2 v3 = glm::rotate(position, glm::radians(15.0f*i));
			// drawFilledCircle(renderer, 320+position.x, 480-(240+position.y), 5);
			drawFilledCircle(renderer, 320+v3.x, 480-(240+v3.y), 3.0f*pow(0.8f, i%6));
		}
	}



    // // 执行平移操作
    // glm::vec2 translation(2.0f, 3.0f);
    // glm::mat3 translateMatrix = glm::translate(glm::mat3(1.0f), translation);
    // glm::vec2 translatedPosition = translateMatrix * glm::vec3(position, 1.0f);

    // // 执行旋转操作
    // float rotationAngle = glm::radians(45.0f); // 将角度转换为弧度
    // glm::mat3 rotateMatrix = glm::rotate(glm::mat3(1.0f), rotationAngle);
    // glm::vec2 rotatedPosition = rotateMatrix * glm::vec3(translatedPosition, 1.0f);

    // // 执行缩放操作
    // glm::vec2 scale(2.0f, 0.5f);
    // glm::mat3 scaleMatrix = glm::scale(glm::mat3(1.0f), scale);
    // glm::vec2 scaledPosition = scaleMatrix * glm::vec3(rotatedPosition, 1.0f);


}
SDL_Window* window;
void drawselfLine(SDL_Renderer* renderer, float y)
{
	SDL_RenderDrawLine(renderer, 0, 400-y, 800, 400-y);
	// printf("draw line %f\n", 400-y);
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
    window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 400, window_flags);
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

    // 清空屏幕为白色
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	// int i;
	// for(i = 0;i<1000;i++){
	// 	SDL_RenderDrawPoint(renderer, i/0.01f, sin(i/0.1f)*10+50);
	// }
	SDL_RenderDrawLine(renderer, 0, 0, 100, 100);
	SDL_RenderDrawLine(renderer, 0, 0, 100, 200);

	// glm::rotate(glm::vec2(0.f, 0.f),glm::)
	glm::vec3 v1(1,0,0);
	glm::vec3 v2(1,1,0);

	glm::vec3 o = v1-v2;
	// printf("%f, %f, %f\n",o.x, o.y,o.z);
	PRINT(o);

	glm::vec3 v11 = glm::rotateX(v1, glm::radians(90.0f));
	glm::vec3 v22 = glm::rotateX(v2, glm::radians(-90.0f));

	glm::vec3 o2 = v11-v2;
	glm::vec3 o3 = v1-v22;

	PRINT(o2);
	PRINT(o3);



	// 主循环处理事件
	SDL_Event e;
	bool quit = false;

	ImVec2 v, downV;
	int mouseDown = 0;
	while (!quit) {

		while (SDL_PollEvent(&e) != 0) {
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
			// dpoint.x = 50.0f*cosf(dv.x*0.1f)+400;
			// dpoint.y = 50.0f*sinf(dv.x*0.1f)+300;
			// SDL_Log("dx %f dy %f\n", dv.x, dv.y);
			// SDL_Log("dx %f dy %f\n", dv.x, dv.y);

		}
		    // 清空屏幕为白色
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 设置颜色为红色
	SDL_RenderDrawLine(renderer, 0, 0, 100, 100);
	SDL_RenderDrawLine(renderer, 0, 0, 100, 200);
		    // 将角度转换为弧度
		float angleInDegrees = 45.0f;
		float angleInRadians = glm::radians(angleInDegrees);
		for(int i = 0;i<10;i++) {

			float x0 = 100.0f*(tan(glm::radians(dv.y+i*5))-1);
			float x1 = 100.0f*(x0-100)/x0;

			drawselfLine(renderer,  x1);
			// printf("dv.y %f, x0 %f, x1 %f\n", dv.y, x0, x1);
		}
		// // 更新屏幕
		SDL_RenderPresent(renderer);
		SDL_Delay(20);
		// Rendering

		// ImGui::Render();
		// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
