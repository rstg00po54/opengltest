#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
static int flag;
// 垂直方向的视场角 fovy、纵横比 aspect、近平面距离 near、远平面距离 far，以及两个透视点的坐标 u 和 v
glm::mat4 customPerspectiveMatrix(float fovy, float aspect, float near, float far, float u, float v) {
    float r = tan(glm::radians(fovy) / 2.0f) * near;
    float t = r / aspect;

    return glm::mat4(
        near / r, 	0.0f, 			u * near / r, 					0.0f,
        0.0f, 		near / t, 		v * near / t, 					0.0f,
        0.0f, 		0.0f, 			(far + near) / (near - far), 	2.0f * far * near / (near - far),
        0.0f, 		0.0f, 			-1.0f, 							0.0f
    );
}

// 使用自定义的二点透视矩阵
// glm::mat4 perspectiveMatrix = customPerspectiveMatrix(fovy, aspect, near, far, u, v);


glm::mat4 customThreePointPerspectiveMatrix(float fovy, float aspect, float near, float far, 
                                           glm::vec3 eye, glm::vec3 target, glm::vec3 up) {
    glm::mat4 viewMatrix = glm::lookAt(eye, target, up);

    // Calculate three points in eye space
    glm::vec4 eyePoint1 = viewMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 eyePoint2 = viewMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 eyePoint3 = viewMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    // Adjust parameters based on three points
    // ...

    // Build the perspective matrix
    return glm::perspective(fovy, aspect, near, far) * glm::lookAt(eye, target, up);
}

// 使用自定义的三点透视矩阵
// glm::mat4 perspectiveMatrix = customThreePointPerspectiveMatrix(fovy, aspect, near, far, eye, target, up);

// fovy: 垂直视场角，定义了从相机观察点看到投影平面的垂直视野角度。

// aspect: 宽高比，投影平面的宽度除以高度。

// near: 近裁剪平面的距离。

// far: 远裁剪平面的距离。

// eye: 相机的位置，即观察点的坐标。

// target: 观察点的目标位置，相机视线的方向。

// up: 相机的上方向。


void saveImg(int width, int height)
{
	
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
}