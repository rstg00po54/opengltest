#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "Mini3DDevice.h"
#include "Mini3DRender.h"
#include <SDL2/SDL.h>

// 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
void device_init(device_t *device, int width, int height, void *fb) {
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	device->aspect_ratio = width*1.f/height;
	device->framebuffer = (IUINT32**)ptr;
	device->zbuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	device->texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;
	if (fb != NULL) framebuf = (char*)fb;
	for (j = 0; j < height; j++) {
		device->framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
		device->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	device->texture[0] = (IUINT32*)ptr;
	device->texture[1] = (IUINT32*)(ptr + 16);
	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0x0c473cff;
	device->foreground = 0;
	transform_init(&device->transform, width, height);
	device->render_state = RENDER_STATE_WIREFRAME;
}





// 清空 framebuffer 和 zbuffer
void device_clear(device_t *device, int mode) {
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++) {
		IUINT32 *dst = device->framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = device->background;
		cc = 0x0c473c;
		for (x = device->width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < device->height; y++) {
		float *dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}
// 画点
void device_pixel(device_t *device, int x, int y, IUINT32 color) {
	void* pixels;
	// pr_debug("in");
	// SDL_LockTexture(texture, NULL, &pixels, &pitch);
	Uint32* pixelData = (Uint32*)pixels;
	if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) {
		device->framebuffer[y][x] = color;
		// pixelData[y][x] = color;
	}
	// SDL_UnlockTexture(texture);
}

void device_point(device_t *device, int x, int y, IUINT32 color) {
	int i,j;
	int r = 3;
	for(i=-r;i<r;i++){
		for(j=-r;j<r;j++){
			device_pixel(device, x+i, y+j, color);

		}
	}

}
void device_draw_line(device_t *device, point_t x, point_t y, IUINT32 c) {
	for(int i = 0;i<2;i++){

		device_draw_line(device, x.x+i, x.y, y.x+i, y.y, c);
	}
	// device_draw_line(device, x.x-1, x.y, y.x-1, y.y, c);
	// device_draw_line(device, x.x+1, x.y, y.x+1, y.y, c);


}
void check_xy(int v, int max){
	if(v<0){
		pr_debug("error 1\n");
	}
	if(v>max){
		pr_debug("error 2\n");
	}
}
// 绘制线段
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c) {
	// pr_debug("%d %d, %d %d", x1, y1, x2, y2);
	int x, y, rem = 0;
	int w = device->width;
	int h = device->height;

	// check_xy(x1, w);
	// check_xy(x2, w);
	// check_xy(y1, h);
	// check_xy(y2, h);

	if (x1 == x2 && y1 == y2) {
		device_pixel(device, x1, y1, c);
	}	else if (x1 == x2) {
		int inc = (y1 <= y2)? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(device, x1, y, c);
		device_pixel(device, x2, y2, c);
	}	else if (y1 == y2) {
		int inc = (x1 <= x2)? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(device, x, y1, c);
		device_pixel(device, x2, y2, c);
	}	else {
		int dx = (x1 < x2)? x2 - x1 : x1 - x2;
		int dy = (y1 < y2)? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				device_pixel(device, x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1)? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}	else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				device_pixel(device, x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1)? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
	}
}
extern int print;
IUINT32 device_texture_read(const device_t *device, float u, float v) {
	int x, y;
	IUINT32 value;
	float u0 = 0;
	float v0 = 0;
	u0 = u * device->max_u;
	v0 = v * device->max_v;
	x = (int)(u0 + 0.5f);
	y = (int)(v0 + 0.5f);
	x = CMID(x, 0, device->tex_width - 1);
	y = CMID(y, 0, device->tex_height - 1);
	// if(print == 0)
	// printf("x/y= %d/%d,u/v = %f/%f\n",x,y,u,v);

	if(device->module == 0) {
		value = device->texture[y][x];
	}else if(device->module == 1) {
		ImVec4 color = device->color;
		// value = 0x11111111;
		value = ((int)(color.x * 255)<<16)|((int)(color.y * 255)<<8)|((int)(color.z * 255)<<0);
		// printf("value 0x%x\n", value);
	}else if(device->module == 2) {
		ImVec4 color = device->color;
		value = 0x5b5b5b;
		// value = ((int)(color.x * 255)<<16)|((int)(color.y * 255)<<8)|((int)(color.z * 255)<<0);
		// printf("value 0x%x\n", value);
	}
	value = device->texture[y][x];
	// if(value==0){
	// 	printf("y %d ,x %d, texture[%d] = %p\n", y, x, y,  device->texture[y]);
	// }
	return value;
}
/*

光源方向通常由一个单位向量 D
目标点到光源的向量 L
P是目标点的坐标，
S是光源的位置。
L=P−S
cos(θ)= L⋅D / |L||D|
I = I0 * cos(θ)²/d²
L⋅D
​
*/
 
// vector_sub(&light, &light, &lightpoint);
// vector_normalize(&light);
// vector_normalize(&lightdir);
// float vout = vector_dotproduct(&lightdir, &normal);
float calcLight(point_t pos, point_t lightpoint, point_t lightdir) {
	float I;
	float I0 = 10;
	point_t light;
	vector_sub(&light, &pos, &lightpoint);
	vector_normalize(&light);
	vector_normalize(&lightdir);
	float theta = vector_dotproduct(&lightdir, &light);
	if(theta<0)
		theta = 0;
	float d = vector_length(&light);
	I =  pow(theta, 6.f)/(d*d);

	return I;
}

extern int print;
// 绘制扫描线
void device_draw_scanline(device_t *device, scanline_t *scanline) {
	IUINT32 *framebuffer = device->framebuffer[scanline->y];
	float *zbuffer = device->zbuffer[scanline->y];
	int x = scanline->x;
	int w0 = scanline->w;
	int width = device->width;
	int render_state = device->render_state;
	int i = 0;



	for (; w0 > 0; x++, w0--) {
		if (x >= 0) {
			float rhw = scanline->v.rhw;
			float t;
			// if((print == 0) && (x < (scanline->x+1)))
			// 	printf("x = %d, rhw = %3.2f\n", x, 1.0f/rhw);

			if(print == 0){
				// pr_debug("t %3.2f\n", t);
			}
			if (rhw < zbuffer[x])
				continue;
			// if (rhw >= zbuffer[x]) {	
				float w = 1.0f / rhw;
				zbuffer[x] = rhw;
				float p = (x - scanline->x)*1.f/scanline->w;


				vertex_t pos0 = interp_spos(scanline->p0, scanline->p1, p);
				vector_t lightpoint = scanline->lightpoint;
				vector_t lightdir = scanline->lightdir;
				point_t normal = {0,1.f,0};

				// pr_debug("mmm %3.2f\n", mmm);
				// framebuffer[x] *= mmm;
				float mmm = calcLight(pos0.spos, lightpoint, lightdir);
				if(mmm > 1)
					pr_debug("%3.2f\n", mmm);
				if (render_state & RENDER_STATE_COLOR) {
					float r = scanline->v.color.r * w;
					float g = scanline->v.color.g * w;
					float b = scanline->v.color.b * w;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
				if (render_state & RENDER_STATE_TEXTURE) {
					float u = scanline->v.tc.u * w;
					float v = scanline->v.tc.v * w;
					if(print == 0 && w0 == 1){
						// pr_debug("u %f, v %f, w %f, scanline uv %f %f\n", u, v, w, scanline->v.tc.u, scanline->v.tc.v);
					}
					IUINT32 cc;
					IUINT32 z;
					// if(device->module != 2) {
						cc = device_texture_read(device, u, v);
						// float m0 = pow(cosf(vout/15.f),5.f);
						float m0 = mmm+0.2f;

						m0 = 1.f;
						int B = (cc & 0xff)*m0;
						int G = ((cc>>8) & 0xff)*m0;
						int R = ((cc>>16) & 0xff)*m0;
						framebuffer[x] = (R << 16) | (G << 8) | (B);
						// framebuffer[x] = mmm * 255.f;

					// }else{
						// z = rhw*255.0f*5;
						// cc = z|(z<<8)|(z<<16);

					// }

					// if(x == scanline->x+10){
					// // device_point(device, scanline->p1.pos.x, scanline->p1.pos.y, 0xff00);
					// 	framebuffer[x] = 0xff;
					// }


					// if (pos0.spos.x < 0.f){
					// 	framebuffer[x] = 0xff00;
					// }
					// if (pos0.spos.z < 0.f){
					// 	framebuffer[x] = 0xff00;
					// }


				}
			// }
		}
		vertex_add(&scanline->v, &scanline->step);
		if (x >= width) break;
	}

	// for(i = 0;i<10;i++) {
	// 	framebuffer[scanline->x+i-20] = 0xff00;
	// }
}