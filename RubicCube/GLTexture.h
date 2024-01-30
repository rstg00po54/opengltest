#pragma once
#include <gl/GL.h>
#include <gl/GLU.h>

#define BMP_Header_Length 54

class GLTexture
{
private:
	// 用于判断一个整数是不是 2 的整数次幂
	static BOOL PowerOfTwo(int n);

public:
	static GLuint LoadTexture(const char* fileName);
};
