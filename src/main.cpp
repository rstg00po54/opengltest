
// #include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 #include "glad.h"
#include "linmath.h"
#include <stb_image.h>
#include <stdlib.h>
#include <stdio.h>
 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
// #include <glm/gtx/simd_vec4.hpp>
// #include <glm/gtx/simd_mat4.hpp>
#include <cstdio>
#include <ctime>
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
static int flag;
#define Size_h (1.0f/6)
static const struct
{
	float x, y, z;
	float r, g, b;
	float tx, ty;
} vertices[] =
{
//	 ---- 位置 ----	   ---- 颜色 ----	 - 纹理坐标 -
	 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, Size_h*3,   // 右上
	 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, Size_h*2,   // 右下
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*2,   // 左下

	-0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*2,	// 左下
	-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,	// 左上
	 0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   1.0f, Size_h*3,	// 右上

	 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0.0f, Size_h*4,   // 左上
	 0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, Size_h*4,   // 右上
	 0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*3,   // 左下

	 0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,	// 左下
	 0.5f,  0.5f,-0.5f,   1.0f, 1.0f, 0.0f,   1.0f, Size_h*4,	// 右上
	 0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   1.0f, Size_h*3,	// 右下


};

unsigned int indices[] = {
	// 注意索引从0开始! 
	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
	// 这样可以由下标代表顶点组合成矩形

	0, 1, 2, // 第一个三角形
	3, 4, 5  // 第二个三角形
};

//  varying变量可以在Vertex Shader和Fragment Shader之间传递数据
static const char* vertex_shader_text =
"#version 330 core\n"
"#pragma optimize(off)\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol0;\n"
"attribute vec2 aTex;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"varying vec2 TexCoord;\n"
"void main()\n"
"{\n"
"	gl_Position = MVP * vec4(vPos, 1.0);\n"
"	color = vCol0;\n"
"	TexCoord = aTex;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330 core\n"
"#pragma optimize(off)\n"
"varying vec3 color;\n"
"varying vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(color, 1.0);\n"
"	gl_FragColor = texture(ourTexture, vec2(TexCoord.x,1.0 -TexCoord.y));\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void getLog(GLint fragment_shader)
{
	GLint infoLen = 0;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen)
		{
			// 获取编译日志
			char* buf = (char*) malloc((size_t)infoLen);
			if (buf)
			{
				glGetShaderInfoLog(fragment_shader, infoLen, NULL, buf);
				printf("GLUtils::LoadShader Could not compile shader :\n%s\n", buf);
				free(buf);
			}
			glDeleteShader(fragment_shader);
			fragment_shader = 0;
		}
}
void testglm();
int main(void)
{
	GLFWwindow* window;
	GLuint VAO, EBO, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location, aTexCoord;
	testglm();
	// return 0;
	glfwSetErrorCallback(error_callback);
		int i = 0;
		glm::vec3 A = glm::normalize(glm::vec3(1));
		glm::vec3 B = glm::cross(A, glm::normalize(glm::vec3(1, 1, 2)));
		glm::mat4 roj_mat = glm::perspective(glm::radians(45.0), 480/320.0, 0.1, 1000.0);
		// glm::mat4 C = glm::rotate(glm::mat4(1.0f), i, B);
		// glm::mat4 D = glm::scale(C, glm::vec3(0.8f, 1.0f, 1.2f));
		// glm::mat4 E = glm::translate(D, glm::vec3(1.4f, 1.2f, 1.1f));
		// glm::mat4 F = glm::perspective(i, 1.5f, 0.1f, 1000.f);
		// glm::mat4 G = glm::inverse(F * E);
		// glm::vec3 H = glm::unProject(glm::vec3(i), G, F, E[3]);
		// glm::vec3 I = glm::any(glm::isnan(glm::project(H, G, F, E[3]))) ? glm::vec3(2) : glm::vec3(1);
		// glm::mat4 J = glm::lookAt(glm::normalize(glm::max(B, glm::vec3(0.001f))), H, I);


	// 三个点的坐标
	glm::vec3 eye(1.0f, 1.0f, 1.0f);	// 观察点
	glm::vec3 center(0.0f, 0.0f, 0.0f); // 观察目标点
	glm::vec3 up(0.0f, 1.0f, 0.0f);	  // 上方向

	// 透视投影矩阵的参数
	float fov = 45.0f;	 // 视场角度
	float aspect = 1.0f;   // 宽高比
	float nearClip = 0.1f;  // 近裁剪面
	float farClip = 100.0f; // 远裁剪面

	// 构建透视投影矩阵
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);

	// 构建观察矩阵（视图矩阵）
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);




	if (!glfwInit())
		exit(EXIT_FAILURE);
 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	glfwSetKeyCallback(window, key_callback);
 
	glfwMakeContextCurrent(window);
	// gladLoadGL(glfwGetProcAddress);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../skin_debug.jpg", &width, &height, &nrChannels, 0);
	printf("%dx%d\n", width, height);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		// std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glGenBuffers(1, &EBO);

	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	GLint compiled = 0;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("create vertex_shader fail\n");
		getLog(vertex_shader);
	}else{
		printf("create vertex_shader ok\n");
	}
	// 若编译失败，获取编译日志

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	 glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("create fragment_shader fail\n");
		getLog(fragment_shader);
	}else{
		printf("create fragment_shader ok\n");
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol0");
	aTexCoord = glGetAttribLocation(program, "aTex");
	printf("mvp_location %d\n", mvp_location);
	printf("vpos_location %d\n", vpos_location);
	printf("vcol_location %d\n", vcol_location);
	printf("aTexCoord %d\n", aTexCoord);

	/*
		index 指定要配置的顶点属性的编号。
		size 指定每个顶点属性的分量数（1、2、3 或 4，就像向量的维度一样）。
		type 指定每个分量的数据类型，可以是 GL_BYTE、GL_UNSIGNED_BYTE、GL_SHORT、GL_UNSIGNED_SHORT、GL_INT、GL_UNSIGNED_INT、GL_FLOAT 或 GL_DOUBLE。
		normalized1 指定是否将数据归一化到 [0,1] 或 [-1,1] 范围内。
		stride （步长）指定连续两个顶点属性间的字节数。如果为 0，则表示顶点属性是紧密排列的。
		pointer 指向缓冲对象中第一个顶点属性的第一个分量的地址。（offset的作用）
	*/
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);

	if(vcol_location > -1){

		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 3));
	}
	if(aTexCoord > -1){
		glEnableVertexAttribArray(aTexCoord);
		glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 6));
	}

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		mat4x4 m, p, mvp;
 
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		mat4x4_identity(m);
		float t = (float)glfwGetTime();
		// t = -90.0f;
		t *= 90;



#if 0
		// mat4x4_rotate_Z(m, m, (float) t);
		// mat4x4_rotate_Z(m, m, (float) 0.1f);
		mat4x4_rotate_X(m, m, (float) t);
		mat4x4_rotate_Y(m, m, (float) t);
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);
		// glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
#else
		glm::vec4 vec2(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		// 逆时针旋转90度。然后缩放0.5倍
		// 旋转
		trans = glm::rotate(trans, glm::radians(t), glm::vec3(0.0, 1.0, 0.0));
#endif
		glUseProgram(program);
		// glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &viewMatrix[0]);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &trans[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glBindVertexArray(VAO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 确保帧缓冲区完整性
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			// 处理帧缓冲区不完整的情况
			// ...
			printf("frame buffer dont \n");
			// 释放资源并退出
			// glDeleteFramebuffers(1, &framebuffer);
			// glDeleteTextures(1, &texture);
			// exit(EXIT_FAILURE);
		}else{
			if (flag == 0){
				printf("frame buffer done \n");
				flag = 1;
				// 读取帧缓冲区数据到内存中
				unsigned char* pixels = (unsigned char*)malloc(3 * width * height);
				glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

				// 保存图像文件
				stbi_write_png("output.png", width, height, 3, pixels, 0);

				// 释放资源
				free(pixels);
			}
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
 