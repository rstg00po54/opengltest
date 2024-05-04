/usr/bin/c++    -rdynamic CMakeFiles/test.dir/glad.c.o CMakeFiles/test.dir/main.cpp.o  -o test  /usr/local/lib/libglfw3.a -lpthread -lrt -lm -ldl
/usr/bin/cc  -o CMakeFiles/test.dir/glad.c.o   -c /home/t/code/k610/opengl/glad.c
/usr/bin/c++ -o CMakeFiles/test.dir/main.cpp.o -c /home/t/code/k610/opengl/main.cpp


VAO（Vertex Array Object）和VBO（Vertex Buffer Object）是OpenGL中用于管理和存储顶点数据的两种重要的概念。

1. **VBO（Vertex Buffer Object）:**
   
   VBO是一块在GPU内存中分配的缓冲区，用于存储顶点数据，例如顶点坐标、法线、纹理坐标等。使用VBO，你可以将顶点数据传递到GPU，以提高性能。通过将顶点数据存储在GPU内存中，可以减少CPU到GPU之间的数据传输，从而提高渲染效率。

   创建和使用VBO的基本步骤包括：
   - 生成一个VBO标识符：`glGenBuffers`
   - 绑定VBO：`glBindBuffer`
   - 将顶点数据传递到VBO：`glBufferData`
   - 解绑VBO：`glBindBuffer`

2. **VAO（Vertex Array Object）:**
   
   VAO是OpenGL中用于存储顶点属性状态的对象。它可以包含多个VBO，以定义顶点数组的格式和布局。VAO主要用于封装和保存顶点属性设置的状态，包括启用和禁用顶点属性、设置顶点属性指针等。使用VAO，你可以轻松地切换和管理不同的顶点属性状态。

   创建和使用VAO的基本步骤包括：
   - 生成一个VAO标识符：`glGenVertexArrays`
   - 绑定VAO：`glBindVertexArray`
   - 启用和配置顶点属性：`glEnableVertexAttribArray` 和 `glVertexAttribPointer`
   - 绑定相关的VBO：`glBindBuffer`
   - 解绑VAO：`glBindVertexArray`

通过结合使用VAO和VBO，你可以更有效地组织和管理顶点数据，提高OpenGL应用程序的性能。这两个概念通常与渲染管线中的顶点着色器一起使用，以绘制图形和场景。

RenderDoc



透视矩阵（Perspective Matrix）在计算机图形学中用于实现透视投影，将三维空间中的物体投影到二维平面上。透视矩阵通常与视图矩阵（View Matrix）和模型矩阵（Model Matrix）一起使用，构成了图形渲染管线的一部分。

在透视矩阵中，一点透视矩阵和二点透视矩阵分别表示针对一点和两点的透视投影。这两种透视矩阵的构建方式有一些不同。

1. **一点透视矩阵（One-Point Perspective Matrix）：**

   一点透视是指观察者或相机位于场景的一个点，投影平面位于相机前方。一点透视矩阵的构建可以通过以下步骤：

   - 将相机位于原点，观察方向为负Z轴方向。
   - 将场景中的点投影到视平面上。
   - 透视投影的公式为：
$$
   P_{perspective} = 

	\begin{bmatrix} 
	\frac{n}{r} & 0 & 0 & 0 \\
	0 & \frac{n}{t} & 0 & 0 \\
	0 & 0 & \frac{n+f}{n-f} & \frac{2nf}{n-f} \\
	0 & 0 & -1 & 0 
	\end{bmatrix} 
$$
   其中，\(n\) 是近裁剪平面的距离，\(f\) 是远裁剪平面的距离，\(r\) 和 \(t\) 分别是近裁剪平面上的右和上的边界。

2. **二点透视矩阵（Two-Point Perspective Matrix）：**

   二点透视是指观察者或相机位于场景的一侧，投影平面位于相机前方。二点透视矩阵的构建可以通过以下步骤：

   - 将相机位于原点，观察方向为负Z轴方向。
   - 将场景中的点投影到视平面上。
   - 透视投影的公式为：
$$
P_{perspective} = 
\begin{bmatrix} 
\frac{n}{r} & 0 & \frac{u}{r} & 0 \\
0 & \frac{n}{t} & \frac{v}{t} & 0 \\
0 & 0 & \frac{n+f}{n-f} & \frac{2nf}{n-f} \\
0 & 0 & -1 & 0 
\end{bmatrix} 
$$
   其中，\(n\) 是近裁剪平面的距离，\(f\) 是远裁剪平面的距离，\(r\) 和 \(t\) 分别是近裁剪平面上的右和上的边界，\(u\) 和 \(v\) 是观察点相对于视平面的水平和垂直偏移。

这些矩阵的具体参数可能会根据具体的图形引擎或应用而有所不同，但上述的基本思想是透视投影的基础。


$$
\begin{aligned}
\begin{cases}
y'(x)=z(x)\\
z'(x)=g(x,y,z)
\end{cases}\\
\end{aligned}
$$
$$|\lambda E-A|=0$$

$$
A=
\left[ \begin{matrix}
    1&2&-1\\
    3&1&0\\
    -1&-1&-2
\end{matrix}
\right]
$$



三点透视矩阵通常是通过将透视投影矩阵和视图矩阵结合而得到的。透视投影矩阵通常由 `glm::perspective` 函数生成，而视图矩阵通常由 `glm::lookAt` 函数生成。下面是一个示例三点透视矩阵的计算：

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 computeThreePointPerspectiveMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up,
                                             float fovY, float aspectRatio, float nearClip, float farClip) {
    // 1. 创建视图矩阵
    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);

    // 2. 创建透视投影矩阵
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fovY), aspectRatio, nearClip, farClip);

    // 3. 计算三点透视矩阵
    glm::mat4 threePointPerspectiveMatrix = projectionMatrix * viewMatrix;

    return threePointPerspectiveMatrix;
}

int main() {
    // 设置相机位置、观察点和相机的上方向
    glm::vec3 eye = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // 设置透视参数
    float fovY = 45.0f;      // 垂直视场角
    float aspectRatio = 1.0f; // 宽高比
    float nearClip = 0.1f;    // 近裁剪平面
    float farClip = 100.0f;   // 远裁剪平面

    // 计算三点透视矩阵
    glm::mat4 threePointPerspectiveMatrix = computeThreePointPerspectiveMatrix(eye, center, up, fovY, aspectRatio, nearClip, farClip);

    // 使用 threePointPerspectiveMatrix 进行渲染或传递给着色器
    // ...

    return 0;
}
```

在这个示例中，`computeThreePointPerspectiveMatrix` 函数接受相机的位置 `eye`、观察点 `center`、相机的上方向 `up`，以及透视投影的参数，返回计算得到的三点透视矩阵 `threePointPerspectiveMatrix`。这个矩阵可以用于渲染或传递给着色器。

# 透视投影矩阵  
透视投影矩阵的形式通常为：
$$

\begin{bmatrix}
\frac{1}{\tan(\frac{\text{fovy}}{2}) \times \text{aspect}} & 0 & 0 & 0 \\
0 & \frac{1}{\tan(\frac{\text{fovy}}{2})} & 0 & 0 \\
0 & 0 & -\frac{\text{far} + \text{near}}{\text{far} - \text{near}} & -\frac{2 \times \text{far} \times \text{near}}{\text{far} - \text{near}} \\
0 & 0 & -1 & 0 \\
\end{bmatrix}

$$
其中:

- \(\text{fovy}\) 是视野角度，以弧度为单位。
- \(\text{aspect}\) 是宽高比。
- \(\text{near}\) 是近平面距离。
- \(\text{far}\) 是远平面距离。

这个矩阵执行了透视变换，将三维坐标映射到二维屏幕上。在GLM库中，`glm::perspective` 函数会生成类似上述形式的透视投影矩阵。你不需要手动写出矩阵的所有元素，而是使用这个函数来方便地创建透视投影矩阵。

注意：上述矩阵是右手坐标系中的透视投影矩阵，如果你使用的是左手坐标系，一些元素的符号可能会变化。 GLM库默认使用右手坐标系。






# 正交透视矩阵
正交透视矩阵（Orthographic Projection Matrix）是计算机图形学中用于将三维场景投影到二维平面的一种方法。与透视投影不同，正交投影不会考虑观察者和投影平面之间的距离，因此导致投影结果没有透视效果，所有的线都是平行的。这种投影方法常用于制作技术图、CAD（计算机辅助设计）图和游戏中的2D效果等场景。

正交透视矩阵的数学表示如下：

$$
\begin{bmatrix}
2 / (right - left) & 0 & 0 & -(right + left) / (right - left) \\
0 & 2 / (top - bottom) & 0 & -(top + bottom) / (top - bottom) \\
0 & 0 & -2 / (far - near) & -(far + near) / (far - near) \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

其中，left、right、bottom、top表示投影平面的左、右、下、上边界，near和far表示投影平面到观察者的距离范围。




# 透视矩阵
透视矩阵（Perspective Matrix）是计算机图形学中用于进行透视投影的矩阵。透视投影是一种在三维场景中创建逼真透视效果的投影方法，它会考虑观察者和投影平面之间的距离，导致在投影结果中出现透视效果，即远处的对象会显得较小，近处的对象会显得较大。

透视矩阵的数学表示通常如下：

$$
\begin{bmatrix}
\frac{2n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\
0 & \frac{2n}{t-b} & \frac{t+b}{t-b} & 0 \\
0 & 0 & -\frac{f+n}{f-n} & -\frac{2fn}{f-n} \\
0 & 0 & -1 & 0
\end{bmatrix}
$$

其中，\(l\)、\(r\)、\(b\)、\(t\)分别表示投影平面的左、右、下、上边界，\(n\)和\(f\)表示观察者到投影平面的近、远裁剪面的距离。

透视投影矩阵会将场景中的三维坐标变换到透视投影平面上，并且考虑了距离视点的远近关系，从而实现真实的透视效果。


[automount]
 enabled = true
[boot]
systemd=true


[automount]
options = "metadata"