#include "SDL2/SDL_ttf.h"
#include "Mini3D.h"
#include "Mini3DDevice.h"
#include "Mini3DRender.h"
#include <string>
void drawCharAt(device_t *device, point_t p, std::string c) {
	drawCharAt(device, p.x, p.y, c);
}
void drawCharAt(device_t *device, int xpos, int ypos, std::string c) {

	// 创建字体颜色
	SDL_Color textColor = {255, 255, 255}; // 白色
	TTF_SetFontSize(device->font, 12);
	// 渲染文本
	SDL_Surface* textSurface = TTF_RenderText_Solid(device->font, c.c_str(), textColor);


	device_point(device, xpos, ypos, 0xff);

// 锁定表面以访问像素数据
	if (SDL_MUSTLOCK(textSurface)) {
		SDL_LockSurface(textSurface);
	}

	// 获取指向像素数据的指针和每行数据的字节数
	uint8_t* pixels = (uint8_t*)textSurface->pixels;
	int pitch = textSurface->pitch;//48

	// 修改像素数据 
	int width = textSurface->w;//25
	int height = textSurface->h;//56

	if(ypos >= (device->height-height))
		ypos = device->height-height;
	if(xpos >= (device->width-width))
		xpos = device->width-width;
	// pr_debug("pitch %d, width %dx%d\n", pitch, width, height);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			uint8_t pixel = pixels[(y * pitch ) + x];
			if(pixel> 0){
				device_pixel(device, xpos+x,ypos+y, 0xffffff);//SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_BGR888), 255, 255, 255));
			}
		}
	}

	// 解锁表面
	if (SDL_MUSTLOCK(textSurface)) {
		SDL_UnlockSurface(textSurface);
	}
}