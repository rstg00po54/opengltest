#ifndef __MINI3D_H__
#define __MINI3D_H__
#include "Mini3DTransform.h"
#include <stdint.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
typedef unsigned int IUINT32;
//=====================================================================
// 渲染设备
//=====================================================================
typedef struct {
	transform_t transform;      // 坐标变换器
	int width;                  // 窗口宽度
	int height;                 // 窗口高度
	IUINT32 **framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
	float **zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针
	IUINT32 **texture;          // 纹理：同样是每行索引
	int tex_width;              // 纹理宽度
	int tex_height;             // 纹理高度
	float max_u;                // 纹理最大宽度：tex_width - 1
	float max_v;                // 纹理最大高度：tex_height - 1
	int render_state;           // 渲染状态
	IUINT32 background;         // 背景颜色
	IUINT32 foreground;         // 线框颜色
	uint8_t *bmpBuffer;
	float aspect_ratio;
	int module;
	int rgb;
	ImVec4 color;
}	device_t;

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);
#endif

//调试打印开关
#define __DEBUG
 
#ifdef __DEBUG
//只取文件名
#define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// windows:
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux:
// #define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define pr_debug(format, ...) printf("[%10s:%d][%s]: \033[32m" format "\033[32;0m", filename(__FILE__), __LINE__, __func__, ##__VA_ARGS__)
#define pr_info(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[33m" format "\033[32;0m", __LINE__, __func__, ##__VA_ARGS__)
#define pr_err(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[31m" format "\033[32;0m", __LINE__, __func__, ##__VA_ARGS__)
#else
#define pr_debug(format, ...)
#define warn_info(format, ...)
#define error_info(format, ...)
#endif
