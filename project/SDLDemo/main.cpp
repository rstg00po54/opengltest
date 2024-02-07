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

int main() {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        return 1;
    }
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, window_flags);
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
//

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

//
    // 清空屏幕为白色
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	// int i;
	// for(i = 0;i<1000;i++){
	// 	SDL_RenderDrawPoint(renderer, i/0.01f, sin(i/0.1f)*10+50);
	// }
	// SDL_RenderDrawLine(renderer, 0, 0, 100, 100);
	// SDL_RenderDrawLine(renderer, 0, 0, 100, 200);




 // 设置 Dear ImGui 窗口移动标志
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	float offset = 1.0f;
	float offsetx = 100.0f;
	float angle = 15.0f;
	float angleoffset = 0.0f;
	int count = 1;
	int pointcount = 5;
    // 主循环处理事件
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
			ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // 开始新的ImGui帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // ImGui::Begin("My Window", nullptr, windowFlags);
		ImGui::SetWindowSize(ImVec2(600, 600)); 
		// ImGui::SetWindowPos(ImVec2(00, 00)); 
		// ImGui::Text("Avg fps: %.3f", ImGui::GetIO().Framerate);
		// ImGui::Button("Reset");
		// ImGui::End();

	// 绘制点
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 center = ImGui::GetCursorScreenPos(); // 获取窗口中心点的屏幕坐标
	float point_radius = 3.0f;

	int i,j;
	for(j=0;j<count;j++){

		for(i=0;angle*i<360.0f;i++){
			ImVec2 v;
			// v.x = center.x+100;
			// v.y = center.y+100;

			glm::vec2 position(offsetx+j*10, 0.0f);
			position = glm::rotate(position, glm::radians(angleoffset*j));
			glm::vec2 v3 = glm::rotate(position, glm::radians(angle*i));
			// 	// drawFilledCircle(renderer, 320+position.x, 480-(240+position.y), 5);
			v.x = center.x+v3.x+300;
			v.y = (center.y+v3.y+300);
			// v.y = 600 - (center.y+v3.y+300);
			point_radius = 5.0f*pow(offset, i%pointcount);

			draw_list->AddCircleFilled(v, point_radius, IM_COL32(255, 255, 0, 255)); // 在中心点绘制黄色实心圆点
		}
	}
	ImGui::SliderFloat("size", &offset, 0.2f, 1.0f);
	ImGui::SliderFloat("offsetx", &offsetx, 0.0f, 500.0f);
	ImGui::SliderFloat("angle", &angle, 1.0f, 50.0f);
	ImGui::SliderFloat("angleoffset", &angleoffset, 1.0f, 50.0f);
	ImGui::SliderInt("count", &count, 1, 10);
	ImGui::SliderInt("pointcount", &pointcount, 1, 10);
	printf("center.x %f, %f\n", center.x, center.y);


 		ImGui::Render();
        // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// drawself(renderer);
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SwapWindow(window);
    	// // 更新屏幕
    	// SDL_RenderPresent(renderer);
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
