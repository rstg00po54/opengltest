#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
void testglm()
{
	// 先进行缩放操作，然后是旋转，最后才是位移
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// 向量(1, 0, 0)位移(1, 1, 0)个单位
	// 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
	// 下面这行代码就需要改为:
	glm::mat4 trans = glm::mat4(1.0f);
	// 之后将不再进行提示
	// glm::mat4 trans;
	// 平移
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	// std::cout << vec.x << vec.y << vec.z << std::endl;
	printf("%f.%f.%f\n", vec.x, vec.y, vec.z);

	glm::vec4 vec2(1.0f, 0.0f, 0.0f, 1.0f);
	trans = glm::mat4(1.0f);
	// 逆时针旋转90度。然后缩放0.5倍
	// 旋转
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	// 缩放
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	vec2 = trans * vec2;
	printf("%f %f %f\n", vec2.x, vec2.y, vec2.z);
	float time = (float)glfwGetTime();
	// printf("%f\n", time%2);
}