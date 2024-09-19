#include "Mini3DCalcLines.h"
#include "Mini3DMath.h"
#include <math.h>
#include "SDL2/SDL_ttf.h"
#include <Mini3DDevice.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Mini3DDraw.h"
#include "Mini3DRender.h"

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
	sprintf(bufname, "line_%dx%x_%d.yuv", width, height, pitch);

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
/*
	Text
*/
// static SDL_Texture* textureText;
static SDL_Surface* textSurface;
void createText(device_t *device) {

		// 创建字体颜色
	SDL_Color textColor = {255, 255, 255}; // 白色
	if(textSurface)
	return;
	TTF_SetFontSize(device->font, 16);
	// 渲染文本
	textSurface = TTF_RenderText_Solid(device->font, "x", textColor);
	if (textSurface == nullptr) {
		// printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
		TTF_CloseFont(device->font);
		// SDL_DestroyRenderer(renderer);
		// SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return;
	}
//    textureText = SDL_CreateTextureFromSurface(device->renderer, textSurface);
// 	// SDL_FreeSurface(textSurface);
// 	if (textureText == nullptr) {
// 		// printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
// 		TTF_CloseFont(device->font);
// 		// SDL_DestroyRenderer(renderer);
// 		// SDL_DestroyWindow(window);
// 		TTF_Quit();
// 		SDL_Quit();
// 		return;
// 	}
// 	Uint32 format;
// 	int access;
// 	int width, height;
// 	if (SDL_QueryTexture(textureText, &format, &access, &width, &height) != 0) {
// 		printf("SDL_QueryTexture Error: %s\n", SDL_GetError());
// 		return;
// 	}
	// 打印纹理的属性
	// printf("Texture format: %u\n", format);
	// printf("Texture access: %d\n", access);
	// printf("Texture width: %d\n", width);
	// printf("Texture height: %d\n", height);
}

/*
persp->ortho

n 0 0 0
0 n 0 0
0 0 n+f -nf
0 0 1 0
*/

// fovy 角度
void matrix_set_perspective_to_ortho(matrix_t *m, float zn, float zf) {
	// float fax = 1.0f / (float)tan(2.0f*3.14f*fovy/360.0f);
	matrix_set_zero(m);
	m->m[0][0] = zn;
	m->m[1][1] = zn;
	m->m[2][2] = (zn + zf);
	m->m[3][2] = 1;
	m->m[2][3] = -zn*zf;

}

/*
ortho
2/(r-l) 0 0 0
0 2/(t-b) 0 0
0 0 2/(n-f) -(n+f)/(n-f)
0 0 0 1

2/(r-l) = 1/(nA*tan 22.5)
2/(t-b) = 1/(n*tan 22.5)
*/
void matrix_set_ortho(matrix_t *m, device_t *device, float zn, float zf) {
	float fax = 1.0f / (float)tan(2.0f*3.14f*45.0f/360.0f);
	float a = device->aspect_ratio;
	matrix_set_zero(m);
	m->m[0][0] = fax/(zn*a);
	m->m[1][1] = fax/(zn);
	m->m[2][2] = 2.f/(zn-zf);
	m->m[2][3] = -(zn+zf)/(zn-zf);
	m->m[3][3] = 1.f;

}


/*

o X p->r
2n/(r-l) 0 0 0
0 2n/(t-b) 0 0
0 0 (n+f)/(n-f) -2nf/(n-f)
0 0 1 0

*/
point_t calcM(point_t t, device_t *device) {
	point_t out;
	matrix_t pto,o,mvp,m;

	point_t a1,a2;

	matrix_set_perspective_to_ortho(&pto, 1.f, 100.f);
	matrix_set_ortho(&o, device, 1.f, 100.f);

	matrix_mul(&m, &o, &pto);
	matrix_mul(&mvp, &m, &device->transform.view);
	// out = o x pto x view x point

	matrix_apply(&a1, &t, &device->transform.view);
	matrix_apply(&a2, &a1, &pto);
	matrix_apply(&out, &a2, &o);
	// if(t.z!=0){
	// ImGui::SliderFloat4("a2", (float *)&a2, -3.0f, 3.0f);
	// ImGui::SliderFloat4("out", (float *)&out, -3.0f, 3.0f);

	// }
	// matrix_apply(&out, &t, &device->transform.mvp);


	return out;

}

point_t drawMyLines(device_t *device) {

	ImGui::Begin("draw_line");
	point_t ret;

	static float x = 1.0f;
	static float y = 1.0f;
	// vertex_t p1 = mesh[i];
	point_t p[2],phome[2];
	// point_t po2[2];
	matrix_t m_out[2];
	point_t psrc[][2] = {
		{{-15,0,-15,1},{0,0,-15,1}},
		{{-15,0,-15,1},{-15,15,-15,1}},
		{{-15,0,-15,1},{-15,0,0,1}},
	};
	createText(device);
	// Render(device);
		// x = psrc[0][0].x;
		// y = psrc[0][0].y;
	matrix_set_rotate(&device->transform.rotate, 0, 1, 0,  0.f);
	transform_update(&device->transform);
	int len = sizeof(psrc)/sizeof(psrc[0]);
	int c[] = {0xff,0xff00,0x0000,0,0};
	char *str[3] = {"x", "y", "z"};
	for (int i = 0; i < len; i++) {

		VertDraw r = drawLine(device, psrc[i][0], psrc[i][1], c[i]);
		point_t p = r.screenPosition[1];
		drawCharAt(device, p.x, p.y, str[i]);
	}

	point_t v1;
	point_t v2;
	v1 = { 10, 0, 0, 1}; 
	v2 = {-10, 0, 0, 1}; 
	for (int i = -10; i <= 10; i++) {
		v1.z = i*1.25f;
		v2.z = i*1.25f;
		drawLine(device, v1, v2);
	}
	v1 = {0, 0, -10, 1}; 
	v2 = {0, 0,  10, 1}; 
	for(int i = -10;i<=10;i++) {
		v1.x = i*1.25f;
		v2.x = i*1.25f;
		drawLine(device, v1, v2);
	}


	ImGui::End();

	// SDL_Rect renderQuad = {300,300, 16*4, 16}; // x, y, w, h
	// SDL_RenderCopy(device->renderer, textureText, nullptr, &renderQuad);
	return ret;
}
