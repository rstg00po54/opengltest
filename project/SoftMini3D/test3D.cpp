#include <SDL2/SDL.h>
#include <stdio.h>
#include <Math.h>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#undef main
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
SDL_Window* window;
SDL_Renderer* renderer;
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



typedef struct {
    float x;
    float y;
} Vector2D;

typedef struct {
    float matrix[2][2];
} Matrix2x2;

// 创建一个旋转矩阵，angle为角度（弧度制）
Matrix2x2 createRotationMatrix(float angle) {
    Matrix2x2 rotationMatrix;
    float cosTheta = cosf(angle);
    float sinTheta = sinf(angle);
    rotationMatrix.matrix[0][0] = cosTheta;
    rotationMatrix.matrix[0][1] = -sinTheta;
    rotationMatrix.matrix[1][0] = sinTheta;
    rotationMatrix.matrix[1][1] = cosTheta;
    return rotationMatrix;
}

// 创建一个缩放矩阵，scale为缩放因子
Matrix2x2 createScaleMatrix(Vector2D scale) {
    Matrix2x2 scaleMatrix;
    scaleMatrix.matrix[0][0] = scale.x;
    scaleMatrix.matrix[0][1] = 0.0f;
    scaleMatrix.matrix[1][0] = 0.0f;
    scaleMatrix.matrix[1][1] = scale.y;
    return scaleMatrix;
}

Matrix2x2 multiplyMatrix2x2(Matrix2x2 a, Matrix2x2 b) {
    Matrix2x2 result;

    result.matrix[0][0] = a.matrix[0][0] * b.matrix[0][0] + a.matrix[0][1] * b.matrix[1][0];
    result.matrix[0][1] = a.matrix[0][0] * b.matrix[0][1] + a.matrix[0][1] * b.matrix[1][1];
    result.matrix[1][0] = a.matrix[1][0] * b.matrix[0][0] + a.matrix[1][1] * b.matrix[1][0];
    result.matrix[1][1] = a.matrix[1][0] * b.matrix[0][1] + a.matrix[1][1] * b.matrix[1][1];

    return result;
}


/*
m00 m01	|v0		m00*v0+m01*v1
m10 m11	|v1		m10*v0+m11*v1
*/
// 将矩阵应用于向量
Vector2D applyMatrix(Vector2D vector, Matrix2x2 matrix) {
    Vector2D result;
    result.x = matrix.matrix[0][0] * vector.x + matrix.matrix[0][1] * vector.y;
    result.y = matrix.matrix[1][0] * vector.x + matrix.matrix[1][1] * vector.y;
    return result;
}
// SCREEN_WIDTH  680
// SCREEN_HEIGHT 480
/*
-1 - 1
0  - 680
*/
void drawvec(Vector2D p1, Vector2D p2){
	SDL_RenderDrawLine(renderer, (p1.x+1)*320, (p1.y+1)*240, (p2.x+1)*320, (p2.y+1)*240);
	// printf("draw %f %f/%f %f\n", (p1.x+1)*320, (p1.y+1)*240, (p2.x+1)*320, (p2.y+1)*240);
}
float angleRadians;// = M_PI / 2; // 90度的弧度制角度
Vector2D scale ;//= 2.0f;
// 创建一个向量
Vector2D vector[4] = {
		{0.5f, 0.5f},
		{0.5f,-0.5f},
		{-0.5f, 0.5f},
		{-0.5f, -0.5f}};
Vector2D rotatedVector[4];// = applyMatrix(vector, pro);
void test(){
	int i = 0;
	if(scale.x == 0){

		scale.x = 1;
	}
	if(scale.y == 0){

		scale.y = 1;
	}
    // 创建一个旋转矩阵（90度）
    Matrix2x2 rotationMatrix = createRotationMatrix(angleRadians);

    // 创建一个缩放矩阵（2倍）
    Matrix2x2 scaleMatrix = createScaleMatrix(scale);

	// Matrix2x2 pro = multiplyMatrix2x2(scaleMatrix, rotationMatrix);
	Matrix2x2 pro = multiplyMatrix2x2(rotationMatrix, scaleMatrix);//right


	for(i=0;i<4;i++){
    	// 将向量应用于旋转矩阵
		rotatedVector[i] = applyMatrix(vector[i], pro);
	}


	drawvec(rotatedVector[0], rotatedVector[1]);
	drawvec(rotatedVector[1], rotatedVector[2]);
	drawvec(rotatedVector[2], rotatedVector[3]);
	drawvec(rotatedVector[3], rotatedVector[0]);
	drawvec(rotatedVector[2], rotatedVector[0]);
	drawvec(rotatedVector[1], rotatedVector[3]);


    // 打印结果
    // printf("Original Vector: (%.2f, %.2f)\n", vector.x, vector.y);
    // printf("Rotated Vector: (%.2f, %.2f)\n", rotatedVector.x, rotatedVector.y);
    // printf("Scaled Vector: (%.2f, %.2f)\n", scaledVector.x, scaledVector.y);
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	ImGuiIO &io = imgui_init();
	// 锁定纹理以获取像素数据
	void* pixels;
	int pitch;
	// 等待退出事件
	bool quit = false;
	float x;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		// SDL_LockTexture(texture, NULL, &pixels, &pitch);
		// Uint32* pixelData = (Uint32*)pixels;
		// printf("%p\n", pixelData);
		// // 绘制一个绿色的矩形
		// for (int y = 100; y < 200; ++y) {
		// 	for (int x = 100; x < 200; ++x) {
		// 		int index = y * (pitch / sizeof(Uint32)) + x;
		// 		pixelData[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, y, x);
		// 	}
		// }
		// // 解锁纹理
		// SDL_UnlockTexture(texture);
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::SliderFloat("rotate", (float *)&x, 0.0f, 100.0f);


		ImGui::SliderFloat("angleRadians", (float *)&angleRadians, 0.0f, 50.0f);
		ImGui::SliderFloat2("scale", (float *)&scale, 0.0f, 2.0f);

		for(int i = 0;i<4;i++){
			ImGui::Text("rotatedVector[%d]: (%.2f, %.2f)",  i, rotatedVector[i].x, rotatedVector[i].y);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置颜色为黑色
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 设置颜色为红色
		test();
	



		SDL_RenderDrawLine(renderer, 0, 0, x, 200);

		// SDL_Delay(30);
		// 渲染纹理
		ImGui::Render();
		// SDL_RenderCopy(renderer, texture, NULL, NULL);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	// 清理资源
	// SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
