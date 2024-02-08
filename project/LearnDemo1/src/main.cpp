
// #include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>
// #include "linmath.h"
#include <stb_image.h>
#include <stdlib.h>
#include <stdio.h>
 #include <sys/time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
// #include <glm/gtx/simd_vec4.hpp>
// #include <glm/gtx/simd_mat4.hpp>
#include <cstdio>
#include <ctime>
#include <vector>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <shader.hpp>


// imgui
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#define Size_h (1.0f/6)
extern void saveImg(int width, int height);
extern glm::mat4 customPerspectiveMatrix(float fovy, float aspect, float near, float far, float u, float v);
typedef struct
{
	float x, y, z;
	float r, g, b;
	float tx, ty;
} vertices;

/*
		      Y
		      |
          1--------2
         /        /|
        /        / |
       0--------3  6-----X
       |  5     | /
       |        |/
       4--------7
	  	   /
		  Z
*/
static vertices vertices0[] =
{
//	 ---- 位置 ----	   ---- 颜色 ----	 - 纹理坐标 -
	 0.f,  0.f,  0.f, 1.0f, 1.0f, 1.0f,   0.0f, Size_h*1,   // 左下 4
	1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, Size_h*1,   // 左下 4

	 0.f,  0.f,  0.f, 0.0f, 1.0f, 0.0f,   0.0f, Size_h*1,   // 左下 4
	0.0f, 1000.0f, 0.0f, 0.0f, 1.0f, 0.0f,   0.0f, Size_h*1,   // 左下 4

	 0.f,  0.f,  0.f, 1.0f, 0.0f, 0.0f,   0.0f, Size_h*1,   // 左下 4
	0.0f, 0.0f, 1000.0f, 1.0f, 0.0f, 0.0f,   0.0f, Size_h*1,   // 左下 4
#if 1
// 下
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*1,   // 左下 4
	-0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0.0f, Size_h*2,   // 左上 5
	 0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   1.0f, Size_h*2,   // 右上 6

	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*1,	// 左下 4
	 0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   1.0f, Size_h*2,	// 右上 6
	 0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, Size_h*1,	// 右下 7	
// 后
	-0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   1.0f, Size_h*1,   // 右下 5
	 0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, Size_h*2,   // 左上 2
	-0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, Size_h*2,   // 右上 1

	-0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   1.0f, Size_h*1,   // 右下 5
	 0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   0.0f, Size_h*1,   // 左下 6
	 0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, Size_h*2,   // 左上 2
// 左
	-0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0.0f, Size_h*2,   // 左下 5
	-0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, Size_h*3,   // 左上 1
	-0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, Size_h*3,   // 右上 0

	-0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0.0f, Size_h*2,   // 左下 5
	-0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, Size_h*3,   // 右上 0
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, Size_h*2,   // 右下 4
// 前
	 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, Size_h*3,   // 右上 3
	 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, Size_h*2,   // 右下 7
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*2,   // 左下 4

	-0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*2,	// 左下 4
	-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,	// 左上 0
	 0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   1.0f, Size_h*3,	// 右上 3
// 右
	 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0.0f, Size_h*4,   // 左上 3
	 0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, Size_h*4,   // 右上 2
	 0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, Size_h*3,   // 左下 7

	 0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,	// 左下 7
	 0.5f,  0.5f,-0.5f,   1.0f, 1.0f, 0.0f,   1.0f, Size_h*4,	// 右上 2
	 0.5f, -0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   1.0f, Size_h*3,	// 右下 6
// 上
	 -0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,   // 左下 0
	 -0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, Size_h*4,   // 左上 1
	  0.5f, 0.5f,-0.5f,   0.0f, 0.0f, 1.0f,   1.0f, Size_h*4,   // 右上 2

	 -0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, Size_h*3,	// 左下 0
	  0.5f, 0.5f,-0.5f,   1.0f, 1.0f, 0.0f,   1.0f, Size_h*4,	// 右上 2
	  0.5f, 0.5f,0.5f,    1.0f, 1.0f, 1.0f,   1.0f, Size_h*3,	// 右下 3	
#endif
};

// typedef struct
// {
// 	float x, y, z;
// 	float r, g, b;
// 	float tx, ty;
// } vertices0;


/*
		  Y
		  |
          1--------2
         /        /|
        /        / |
       0--------3  6-----X
       |  5     | /
       |        |/
       4--------7
	  /
	 Z
*/



unsigned int indices[42] = {
	// 注意索引从0开始! 
	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
	// 这样可以由下标代表顶点组合成矩形
	0, 1, 0,
	3, 0, 5, 
	6, 7, 8,  // 第二个三角形
	9, 10, 11,  // 第二个三角形
	12, 13, 14,  // 第二个三角形
	15, 16, 17  // 第二个三角形
};

//  varying变量可以在Vertex Shader和Fragment Shader之间传递数据

void processInput(GLFWwindow *window);


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
// 	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, GLFW_TRUE);
// }

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float lastdeltaX;
float downX;
float nowX;

float lastdeltaY;
float downY;
float nowY;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float cameraSpeed = static_cast<float>(20 * deltaTime * 0.001);
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {

        printf("Key A pressed!\n");
    }
	if(action != GLFW_PRESS)
		return;
	switch (key)
	{
		case GLFW_KEY_W:
			cameraPos += cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_S:
			cameraPos -= cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_A:
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		case GLFW_KEY_D:
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		default:
			break;
	}
}
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f %f\n", xpos, ypos);
	nowX = xpos;
	nowY = ypos;
	if(downX)
	{
		lastdeltaX = xpos - downX;
		// downX = xpos;
		cameraFront.x = sin(glm::radians(lastdeltaX));
		// cameraFront.x += lastdeltaX;
		printf("x-=- %f %f\n", lastdeltaX, cameraFront.x);
	}
	if(downY)
	{
		lastdeltaY = ypos - downY;
		// downY = ypos;
		cameraFront.y = sin(glm::radians(lastdeltaY));
		// cameraFront.x += lastdeltaX;
		printf("x-=- %f %f\n", lastdeltaY, cameraFront.y);
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// yoffset 表示垂直滚动的偏移量，通常用于获取鼠标滚轮的状态
	printf("Mouse scrolled: %f\n", yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	printf("%s in button %d, action %d, mods %d\n", __func__, button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        printf("Mouse scroll button pressed!\n");
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		downX = nowX;
		downY = nowY;
	}
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		downX = 0;
		downY = 0;
	}
	
}
void processInput(GLFWwindow *window)
{
	int flag = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = static_cast<float>(20 * deltaTime * 0.001);
	// 查询窗口大小
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		printf("w key down\n");
		flag = 1;
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(flag){
		printf("%f,%f,%f, deltaTime %f\n", cameraPos.x, cameraPos.y, cameraPos.z, deltaTime);
	}
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		printf("left press\n");
	}
	// 查询鼠标位置
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// 检查鼠标位置是否在窗口范围内
	if (xpos >= 0 && xpos < windowWidth && ypos >= 0 && ypos < windowHeight) {
		// 鼠标在窗口范围内
		// printf("Mouse position: (%.2f, %.2f)\n", xpos, ypos);
	} else {
		// 鼠标在窗口范围外
		// printf("Mouse is outside the window\n");
	}
}

void testglm();
int main(void)
{
	GLFWwindow* window;
	GLuint VAO, VBO, EBO, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location, aTexCoord;
	testglm();



	// return 0;
	glfwSetErrorCallback(error_callback);
		int i = 0;

	for(i=6;i<42;i++){
		indices[i] = i;
	} 

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
 
	// glfwSetKeyCallback(window, key_callback);
 
	glfwMakeContextCurrent(window);
	    // 设置滚轮回调函数
    // glfwSetScrollCallback(window, scroll_callback);
#if 0
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
#endif

	IMGUI_CHECKVERSION();
	ImGui::CreateContext(); //创建上下文
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 允许键盘控制
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // 允许游戏手柄控制

	// 设置渲染器后端
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// gladLoadGL(glfwGetProcAddress);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

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
	unsigned char *data = stbi_load("./skin_debug.jpg", &width, &height, &nrChannels, 0);
	printf("%dx%d\n", width, height);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
		return 0;
	}
	stbi_image_free(data);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
/*
 // 分配足够的空间
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BUFFER_SIZE, NULL, GL_STATIC_DRAW);

	// 设置第一块数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3, data1);

	// 设置第二块数据
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 3, sizeof(float) * 3, data2);
*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#if 1
	program = LoadShaders( "VertexShader.vs", "FragmentShader.fs" );
#else

#endif
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
	if(vpos_location > -1){
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*) 0);
	}

	if(vcol_location > -1){

		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*) (sizeof(float) * 3));
	}
	if(aTexCoord > -1){
		glEnableVertexAttribArray(aTexCoord);
		glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*) (sizeof(float) * 6));
	}
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int viewLoc  = glGetUniformLocation(program, "view");
	unsigned int projectionLoc  = glGetUniformLocation(program, "projection");

	glBindVertexArray(0);
	glm::vec3 degrees(0.f, 0.f, 0.f);
	glm::vec3 trans_vec3(0.f, 0.f, 0.f);
	glm::mat4 model         = glm::mat4(1.0f);
	glm::mat4 view          = glm::mat4(1.0f);
	glm::mat4 projection    = glm::mat4(1.0f);

	// 设置观察者的位置、观察点的位置和相机的上方向
	glm::vec3 eye 		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 center	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up 		= glm::vec3(0.0f, 1.0f, 0.0f);
// glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec4 cus = glm::vec4(-40.0f, 120.0f, 0.0f, 0.0f);
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		// mat4x4 m, p, mvp;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        // 在这里添加你的 ImGui 窗口设置
        ImGui::Begin("Custom Window");

        // 设置 Text 窗口的大小
        // ImGui::SetWindowSize(ImVec2(400, 300));

		// processInput(window);
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindTexture(GL_TEXTURE_2D, texture);
		// mat4x4_identity(m);
		// float t = (float)glfwGetTime();
		// t = -90.0f;
		// t *= 90;
		glm::vec4 vec2(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		// 逆时针旋转90度。然后缩放0.5倍
		// 旋转
		// trans = glm::rotate(trans, glm::radians(t), glm::vec3(0.0, 1.0, 0.0));
		glUseProgram(program);
		// model = glm::rotate(model, glm::radians(t), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(glm::mat4(1.0f), glm::radians(degrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(degrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(degrees.z), glm::vec3(0.0f, 0.0f, 1.0f));
		// model =	glm::scale(model, degrees);

		// 创建 view 矩阵
		view = glm::lookAt(cameraPos, cameraFront, cameraUp);
		// view  = glm::translate(view, trans_vec3);//平移
		// 透视投影矩阵
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		//正交投影矩阵
		// projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
		projection = customPerspectiveMatrix((45.0f), (float)width / (float)height, cus.x, cus.y, cus.z, cus.w);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
		// glm::mat4 trans0 = glm::mat4(1.0f);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);
		// glBindVertexArray(VBO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &trans[0]);
		/*画线*/
		glLineWidth(3.0f);
		glUniform1i(glGetUniformLocation(program, "vertexInteger"), 0);
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

		/*画正方体*/
		glUniform1i(glGetUniformLocation(program, "vertexInteger"), 1); 
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,  (void*)(6 * sizeof(unsigned int)));

		/*画正方体*/
		// glm::vec3 degrees2(5.0f, 0.f, 0.f);
		model =	glm::translate(model, glm::vec3(5.0f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniform1i(glGetUniformLocation(program, "vertexInteger"), 1); 
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,  (void*)(6 * sizeof(unsigned int)));

		// glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)(sizeof(float)*8*6));
		saveImg(width, height);
		ImGui::Text("Avg fps: %.3f", ImGui::GetIO().Framerate);
		// ImGui::Text("Avg fps: %.3f", ImGui::GetIO().Framerate);

		ImGui::SliderFloat3("Degree", &degrees.x, -180.0f, 180.0f);
		ImGui::SameLine();
		if (ImGui::Button("Degree")) {
			printf("click degrees\n");
			degrees = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		ImGui::SliderFloat3("eye.xyz", &cameraPos.x, -30.0, 30.0);
		ImGui::SameLine();
		if (ImGui::Button("Reset0")) {
			printf("click reset eye\n");
			cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		}

		ImGui::SliderFloat4("cus", &cus.x, -500.0f, 500.0f);
		// cameraPos.z = sqrtf(9-powf(cameraPos.x, 2.0f));

		ImGui::SliderFloat3("center.xyz", &cameraFront.x, -100.0, 100.0);
		ImGui::SameLine();
		if (ImGui::Button("Reset1")) {
			printf("click reset centet\n");
			cameraFront = glm::vec3(0.0f);;
		}
		ImGui::SliderFloat3("up.xyz", &up.x, -10.0, 10.0);
		// ImGui::SliderFloat3("Translation.xyz", &translation.x, -1.0, 1.0);
		// ImGui::SliderFloat("Fov", &fov, 1.0f, 360.0f);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();


		struct timeval tv;
		gettimeofday(&tv, NULL);
		static long last;
		long milliseconds = (long long)(tv.tv_sec) * 1000 + (long long)(tv.tv_usec) / 1000;
		// printf("diff %d ms, \n", milliseconds-last);
		deltaTime = milliseconds-last;
		last = milliseconds;
		glm::vec4 src0 = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
		// glm::mat4 src0    = glm::mat4(1.0f);
		glm::vec4 dst0    = glm::vec4(0.5f);
		dst0 = projection*view*model*src0;

		// printf("%f, %f, %f, %f\n", dst0.x, dst0.y, dst0.z, dst0.w);

		// "	gl_Position = projection * view * model * vec4(vPos, 1.0);\n"

	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
 