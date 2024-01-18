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