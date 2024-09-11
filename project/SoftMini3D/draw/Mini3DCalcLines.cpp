#include "Mini3DCalcLines.h"
#include "Mini3DMath.h"
#include <math.h>
#include "SDL2/SDL_ttf.h"
#include <Mini3DDevice.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


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
static void inter(vector_t *pin1, const vector_t *pin0, float t){

	pin1->x = pin1->x+t*(pin0->x-pin1->x);
	pin1->y = pin1->y+t*(pin0->y-pin1->y);
	pin1->z = pin1->z+t*(pin0->z-pin1->z);
	pin1->w = pin1->w+t*(pin0->w-pin1->w);
}

// -w < x < w，-w < y < w，-w < z < w
point_t transform_home0(const device_t *device, 
				const vector_t *from, const vector_t *to) {
	vector_t *pin0, *pin1;
	float hw;
	vector_t tn;
	// ImGui::Text("-------");
	ImGui::SliderFloat4("输入0", (float *)from, -50.0f, 50.0f);
	ImGui::SliderFloat4("输入1", (float *)to, -50.0f, 50.0f);
	if(abs(from->w) < 0.0001f){
		ImGui::Text("---from----");
	}
	if(abs(to->w) < 0.0001f){
		ImGui::Text("---to----");
	}
	pin0 = (vector_t *)malloc(sizeof(vector_t));
	pin1 = (vector_t *)malloc(sizeof(vector_t));
	*pin0 = *from;
	*pin1 = *to;
	static bool v = true;
	// ImGui::Checkbox("clip", &v);
	if(abs(pin1->x)>abs(pin1->w)){
		ImGui::Text("need clip x");
	}
	if(abs(pin1->y)>abs(pin1->w)){
		ImGui::Text("need clip y");
	}
	if(abs(pin1->z)>abs(pin1->w)){
		ImGui::Text("need clip z");
	}


	bool clipZMins = pin1->z < -std::abs(pin1->w);
	bool clipZPlus = pin1->z >  std::abs(pin1->w);

	bool clipXPlus = pin1->x >  std::abs(pin1->w);
	bool clipXMins = pin1->x < -std::abs(pin1->w);

	bool clipYPlus = pin1->y >  std::abs(pin1->w);
	bool clipYMins = pin1->y < -std::abs(pin1->w);
	// ImGui::Checkbox("clipZMins", &clipZMins);

	// if(v){
		if(pin1->z < -abs(pin1->w)){
			ImGui::Text("clip z -");
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)		
		}
		if(pin1->z > abs(pin1->w)){
			ImGui::Text("clip z +, %f", pin1->z - abs(pin1->w));
			//t = (w1-z1)/((w1-z1)-(w2-z2))
			//I = Q1+t*(Q2-Q1)	
			// vector_t ret;
			float t = (pin1->w - pin1->z)/((pin1->w-pin1->z)-(pin0->w-pin0->z));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			ImGui::SliderFloat4("裁剪z+", (float *)pin1, 0.0f, 50.0f);
			ImGui::SliderFloat("裁剪t", (float *)&t, 0.0f, 50.0f);

		}
		if(pin1->x > abs(pin1->w)){
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->x)/((pin1->w-pin1->x)-(pin0->w-pin0->x));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			ImGui::Text("clip x + t = %f", t);
			// ImGui::SliderFloat4("裁剪x+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->x < -abs(pin1->w)){
			ImGui::Text("clip x -");
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->x)/((pin1->w+pin1->x)-(pin0->w+pin0->x));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪x-", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y > abs(pin1->w)){
			ImGui::Text("clip y +, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w - pin1->y)/((pin1->w-pin1->y)-(pin0->w-pin0->y));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪y+", (float *)pin1, 0.0f, 50.0f);
		}
		if(pin1->y < -abs(pin1->w)){
			ImGui::Text("clip y -, pin1->y %f", pin1->y);
			//t = (w1-x1)/((w1-x1)-(w2-x2))
			//I = Q1+t*(Q2-Q1)		
			float t = (pin1->w + pin1->y)/((pin1->w+pin1->y)-(pin0->w+pin0->y));
			if(abs(t)>1.f){
				return tn;
			}
			inter(pin1, pin0, t);
			// ImGui::SliderFloat4("裁剪y-", (float *)pin1, 0.0f, 50.0f);
		}
	// }
	// hw = 1.f/pin1->w;
	// pin1->x = hw*pin1->x;
	// pin1->y = hw*pin1->y;
	// pin1->z = hw*pin1->z;
	// pin1->w = hw*pin1->w;
	
	// ImGui::SliderFloat4("pin1归一化", (float *)pin1, 0.0f, 50.0f);
	// ImGui::SliderFloat4("o", (float *)&o, 0.0f, 50.0f);

	// pout0->x = (pin0->x+1)*ts->w*0.5f;//400
	// pout0->y = (pin0->y+1)*ts->h*0.5f;//300
	// pout0->z = pin0->z;
	// pout1->x = (pin1->x+1)*ts->w*0.5f;
	// pout1->y = (pin1->y+1)*ts->h*0.5f;

	// pout0->y = 600 - pout0->y;
	// pout1->y = 600 - pout1->y;
	
	// ImGui::SliderFloat4("pout0", (float *)pout0, 0.0f, 50.0f);
	// ImGui::SliderFloat4("pout1", (float *)pin1, 0.0f, 50.0f);
	// float t = (pin1->x-pin0->x)/(src.x-pin0->x);
	// pr_debug("t = %3.2f\n", t);
	// ImGui::SliderFloat("tt", &t, 0, 1);

	return *pin1;

}

int  transform_Normalization(device_t *device, vector_t *po1, vector_t *po0, vector_t *pin1, vector_t*pin0) {
	point_t po00,po11;
	float rhw;
	po00 = transform_home0(device, pin0, pin1);
	po11 = transform_home0(device, pin1, pin0);
	if (sizeof(po00) == 1){
		ImGui::Text("po00 is null");
		pr_debug("po00 is null\n");

		
	return -1;
	}
	if (sizeof(po11) == 1){
		ImGui::Text("po00 is null");
		pr_debug("po11 is null\n");

	return -1;
	}
	// *pin0 = po0;
	// *pin1 = po1;
	// pout0->y = 600 - pout0->y;
	// pout1->y = 600 - pout1->y;

	rhw = 1.f/po00.w;
	po0->x = (po00.x * rhw + 1.0f) * device->transform.w * 0.5f;
	po0->y = (1.0f - po00.y * rhw) * device->transform.h * 0.5f;
	rhw = 1.f/po11.w;
	po1->x = (po11.x * rhw + 1.0f) * device->transform.w * 0.5f;
	po1->y = (1.0f - po11.y * rhw) * device->transform.h * 0.5f;
	return 0;
}

static void drawLine(device_t *device, point_t p0, point_t p1, int c = 0) {
	point_t v0,v1;
	point_t vs0,vs1;
	vs0 = {0,0,0,1};
	vs1 = {0,0,0,1};
	matrix_apply( &v0, &p0, &device->transform.mvp);
	matrix_apply( &v1, &p1, &device->transform.mvp);

	if ( v0.w < 0 && v1.w < 0){
		ImGui::Text("do not");
		return;
	}
	int ret = transform_Normalization(device, &vs0, &vs1, &v0, &v1);
	if (ret < 0){
		return;
	}
	float rhw;
	rhw = 1.0f / v0.w;
	v0.x = v0.x*rhw;
	v0.y = v0.y*rhw;
	v0.z = v0.z*rhw;
	v0.w = v0.w*rhw;
	rhw = 1.0f / v1.w;
	v1.x = v1.x*rhw;
	v1.y = v1.y*rhw;
	v1.z = v1.z*rhw;
	v1.w = v1.w*rhw;
	ImGui::SliderFloat4("v0", (float *)&v0, 0, 1);
	ImGui::SliderFloat4("v1", (float *)&v1, 0, 1);
	// int c[] = {0xff,0xff00,0x0000,0,0};
	if(vs0.x < 0 || vs0.y < 0 || vs1.x > device->width || vs1.y > device->height){
		ImGui::Text("error!!");
		return;
	}
	device_draw_line(device, vs0, vs1, c);
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
		{{0,0,0,1},{2,0,0,1}},
		{{0,0,0,1},{0,1,0,1}},
		{{0,0,0,1},{0,0,2,1}},
	};
	createText(device);
	// Render(device);
		// x = psrc[0][0].x;
		// y = psrc[0][0].y;
	matrix_set_rotate(&device->transform.rotate, 0, 1, 0,  0.f);
	transform_update(&device->transform);
	int len = sizeof(psrc)/sizeof(psrc[0]);
	int c[] = {0xff,0xff00,0x0000,0,0};
	for(int i = 0;i<len;i++) {

		drawLine(device, psrc[i][0], psrc[i][1], c[i]);

	}

	point_t v1;
	point_t v2;
	v1 = { 10, 0, 0, 1}; 
	v2 = {-10, 0, 0, 1}; 
	for(int i = 0;i<=10;i++) {
		v1.z = i;
		v2.z = i;
		drawLine(device, v1, v2);
	}
	// v1 = {0, 0, -10, 1}; 
	// v2 = {0, 0,  10, 1}; 
	// for(int i = -10;i<=10;i++) {
	// 	v1.x = i;
	// 	v2.x = i;
	// 	drawLine(device, v1, v2);
	// }


	ImGui::End();

	// SDL_Rect renderQuad = {300,300, 16*4, 16}; // x, y, w, h
	// SDL_RenderCopy(device->renderer, textureText, nullptr, &renderQuad);
	return ret;
}
