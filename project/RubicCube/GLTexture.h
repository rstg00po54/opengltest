#pragma once
#include <gl/GL.h>
#include <gl/GLU.h>

#define BMP_Header_Length 54

class GLTexture
{
private:
	// �����ж�һ�������ǲ��� 2 ����������
	static BOOL PowerOfTwo(int n);

public:
	static GLuint LoadTexture(const char* fileName);
};
