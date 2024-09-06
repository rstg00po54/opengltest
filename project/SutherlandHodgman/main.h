
#ifndef __MAIN_H__
#define __MAIN_H__
#include <string.h>
using namespace std;
//调试打印开关
// #define __DEBUG
 
/*
\033[30m	黑色
\033[31m	红色
\033[32m	绿色
\033[33m	黄色
\033[34m	蓝色
\033[35m	紫色
\033[36m	浅蓝色
\033[37m	白色
*/

#ifdef __DEBUG
//只取文件名
#define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// windows:
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux:
// #define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define pr_debug(format, ...) printf("[%10s:%3d][]: \033[32m" format "\033[33;0m", filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define pr_debug(format, ...) printf("[%10s:%3d][%s]: \033[32m" format "\033[33;0m", filename(__FILE__), __LINE__, __func__, ##__VA_ARGS__)

#define pr_debug_33(format, ...) printf("[%10s:%3d][]: \033[34m" format "\033[33;0m", filename(__FILE__), __LINE__, ##__VA_ARGS__)

#else
#define pr_debug(format, ...)
#define pr_debug_33(format, ...)
#define error_info(format, ...)
#endif


// 定义一个结构体来表示点
struct vec2 {
	float x, y;
	string name;
	vec2(float x = 0, float y = 0, string name = "def") : x(x), y(y), name(name) {}
};

#endif