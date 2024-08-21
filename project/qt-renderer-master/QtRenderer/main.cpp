#include "QtRenderer.h"
// #include <QtWidgets/QApplication>

#include <SDL2/SDL.h>





SDL_Window* window;
SDL_Texture* texture;
SDL_Renderer* renderer;
int pitch;
int screen_init(int w, int h, const char *title, unsigned char **screen_fb) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);
	// unsigned char **p = &screen_fb;
	SDL_LockTexture(texture, NULL, (void **)screen_fb, &pitch);
	SDL_UnlockTexture(texture);
	printf("buffer %p\n", *screen_fb);
	return 0;
}
#undef main
int main(int argc, char *argv[])
{
	Renderer render;
	int count = 10;
	string m("bunny.obj");
	// string m("bunny.obj");
	string t,n;
	int ret = render.addModel(m, t, n);
	bool quit = false;
	SDL_Event e;
	unsigned char *framebuffer;

	screen_init(800, 600, "render", &framebuffer);
	printf("run... %d\n", ret);
	// QApplication a(argc, argv);
	// QtRenderer w;
	// w.show();
	// return a.exec();
	render.scene.translateModel(0, { 0,0, 20});
	render.scene.scaleModel(0, {0.2, 0.2, 0.2});
	while(!quit){
		SDL_Keycode key;

		while (SDL_PollEvent(&e) != 0) {
			// bool ret = ImGui_ImplSDL2_ProcessEvent(&e);
			// printf("ret %d \n", e.type);
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			switch (e.type) {
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
			}

		}
		float s = 0.5f;
		switch (key)
		{
		case SDLK_w:
			/* code */
			render.scene.translateModel(0, { 0,0,s });
			break;
		case SDLK_s:
			render.scene.translateModel(0, { 0,0,-s });
			break;
		case SDLK_a:
			render.scene.translateModel(0, { -s,0,0 });
			break;
		case SDLK_d:
			render.scene.translateModel(0, { s,0,0 });
			break;
		case SDLK_q:
			render.scene.rotateModel(0, -1, { 0,1,0 });
			break;
		case SDLK_e:
			render.scene.rotateModel(0, 1, { 0,1,0 });
			break;
		case SDLK_z:
			render.scene.scaleModel(0, { 1.2,1.2,1.2 });
			break;
		case SDLK_x:
			render.scene.scaleModel(0, { 0.8,0.8,0.8 });
			break;
		default:
			break;
		}
		key = 0;
		void* pixels;
		int pitch;
		// render.scene.translateModel(0, { 0,0,0.1 });
		SDL_LockTexture(texture, NULL, &pixels, &pitch);
		Uint32* pixelData = (Uint32*)pixels;
		static unsigned char * b ;
		static int flag = 0;
		if(flag == 0){
			// flag = 1;
			b = render.renderThread();
		}



		// memcpy(pixelData, b, 800*600*3);

		for (int y = 0; y < 600; ++y) {
			for (int x = 0; x < 800; ++x) {
		// pixel 4 byte
		// b 3byte
		// 		// pixelData[index] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, y, x);
				pixelData[y*800+x] = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_BGR888), //SDL_PIXELFORMAT_RGBA8888
					b[y*800*3+x*3], 
					b[y*800*3+x*3+1], 
					b[y*800*3+x*3+2]);
		//bmpBuffer[y*300*3+x*3];
		// 			// if(x == 5)
		// 		// printf("0x%x, x %d, y %d\n", pixelData[y*device.width+x],x,y);
			}
		}

				// 解锁纹理
		SDL_UnlockTexture(texture);
		// 渲染纹理
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		// printf("re... %d\n", count);
	}
	return 0;
}
