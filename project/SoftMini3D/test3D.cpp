#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#undef main
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	// 锁定纹理以获取像素数据
	void* pixels;
	int pitch;
	// 等待退出事件
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_LockTexture(texture, NULL, &pixels, &pitch);
		Uint32* pixelData = (Uint32*)pixels;
		// 绘制一个绿色的矩形
		for (int y = 100; y < 200; ++y) {
			for (int x = 100; x < 200; ++x) {
				int index = y * (pitch / sizeof(Uint32)) + x;
				pixelData[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, y, x);
			}
		}
		// 解锁纹理
		SDL_UnlockTexture(texture);

		// 渲染纹理
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		}

		// 清理资源
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

	return 0;
}
