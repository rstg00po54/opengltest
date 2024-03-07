图形学：观察矩阵/LookUp矩阵的推导
====================

1\. 矩阵公式背景
----------

观察矩阵 View Matrix，就是将物体在世界空间（World Space）的坐标变换到观察空间（View Space）的矩阵。观察空间需要一个观察者作为参考对象，一般来说都是摄像机。摄像机在世界空间内某个坐标观察场景中的物体，观察空间就是摄像机看到的视图空间。

如图在一个世界空间内，有一个摄像机（即观察者）摆放在某个位置，我们可以通过将世界空间的坐标乘上观察矩阵来得世界空间内的物体相对于摄像机的位置，简而言之，就是转换得到摄像机看到的世界。

![](https://pic3.zhimg.com/v2-2ef4047b590ede6097a02fb7db70b44e_b.jpg)

![](https://pic3.zhimg.com/80/v2-2ef4047b590ede6097a02fb7db70b44e_720w.webp)

以OpenGL为例，观察空间都有一个特征，就是观察者始终处于坐标系原点，且面向-Z轴，这是为了后续方便计算，以便可以将观察者看到的任何物体变换看成是在观察空间内相对于坐标原点的变换。即在观察空间内，观察者就是原点。

如下图，在世界空间内有一个摄像机，其观察到的物体很简单，仅仅是一个盒体。由于此时摄像机的坐标与旋转在世界空间内是极其复杂的，对后续各种变换的计算十分不利，因此要将世界空间过渡到观察空间。

![](https://pic1.zhimg.com/v2-66e6f01361309ade3d0273e73f774ea0_b.jpg)

![](https://pic1.zhimg.com/80/v2-66e6f01361309ade3d0273e73f774ea0_720w.webp)

世界空间

也就是说，摄像机从世界空间的某个坐标变换到观察空间的原点，世界空间内的所有物体也要跟着一起变换，才能保证仍然具有在世界空间中的相对位置。也就是说，保证让摄像机在空间过渡过程中看到的东西保持不变。

由上下两图可以看出，摄像机要从世界空间过渡到观察空间，需要进行平移和旋转。而平移和旋转两个变换，就是观察矩阵 View Matrix / LookUp矩阵要做的事情。

![](https://pic1.zhimg.com/v2-2d751ab646b46a6d288c7584be574ecc_b.jpg)

![](https://pic1.zhimg.com/80/v2-2d751ab646b46a6d288c7584be574ecc_720w.webp)

观察空间

  

2\. 公式推导
--------

### 1\. 平移矩阵

摄像机从世界空间中某个坐标 \\left( Xw, Yw,Zw \\right)\\left( Xw, Yw,Zw \\right) 移动到原点（观察空间） \\left( 0,0,0 \\right)\\left( 0,0,0 \\right) 进行的平移操作很简单，直接用平移矩阵就可以解决。要注意的是，为了保持相对性，世界空间中的所有物体也要进行相同的平移变换。

M\_{T} = \\left( \\begin{array}{1} 1 & 0 & 0 & -Xw\\\\ 0 & 1 & 0 & -Yw\\\\ 0&0&1& -Zw\\\\ 0&0&0& 1 \\end{array} \\right)M\_{T} = \\left( \\begin{array}{1} 1 & 0 & 0 & -Xw\\\\ 0 & 1 & 0 & -Yw\\\\ 0&0&1& -Zw\\\\ 0&0&0& 1 \\end{array} \\right)

经过平移，如下图所示，摄像机的坐标系原点和观察空间的原点重合，接下来就是要考虑旋转的问题了。

  

### 2\. 旋转矩阵

![](https://pic3.zhimg.com/v2-c3b3d166f34b435d23c99703d6af38b2_b.jpg)

![](https://pic3.zhimg.com/80/v2-c3b3d166f34b435d23c99703d6af38b2_720w.webp)

图一（左） 图二（右）

平移之后变成图二，当摄像机坐标系abc进一步旋转变成标准坐标系XYZ时，就能达到我们想要的旋转结果。因此这时候的旋转矩阵的目标就变成了如何将“原点与标准坐标系XYZ重合的坐标系abc”内的一个点映射到标准坐标系XYZ中。要注意的是，摄像机面对的方向c轴，是要映射到标准坐标系的-Z轴，因为OpenGL是右手坐标系，而另外两个轴a、b则分别映射标准坐标系中的X和Y轴。

即寻找一个 M\_{R}M\_{R} ，使得下列等式成立：

\\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)=M\_{R} \* \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)\\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)=M\_{R} \* \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)

等式左边矩阵行是要映射的标准坐标系的XYZ轴，用向量表示为 X（1,0,0）X（1,0,0） 、 Y(0,1,0 )Y(0,1,0 ) 、 \-Z（0,0,1）\-Z（0,0,1） ，右边矩阵行表示的是摄像机坐标系abc轴，用向量表示为 a（a\_{x},a\_{y},a\_{z}）a（a\_{x},a\_{y},a\_{z}） 、 b（b\_{x},b\_{y},b\_{z}）b（b\_{x},b\_{y},b\_{z}） 、 c（c\_{x},c\_{y},c\_{z}）c（c\_{x},c\_{y},c\_{z}） 。

从这个等式可以看出，此时的 M\_{R}M\_{R} 是很难计算的。因此我们可以将 M\_{R}M\_{R} 移到等号左边，如下：

M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)

再消去等式左边矩阵的-1：

M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)\* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)\*\\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)\* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&c\_{x}\\\\ a\_{y}&b\_{y}&c\_{y}\\\\ a\_{z}&b\_{z}&c\_{z} \\end{array} \\right)\*\\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&-1 \\end{array} \\right)

得：

M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&-c\_{x}\\\\ a\_{y}&b\_{y}&-c\_{y}\\\\ a\_{z}&b\_{z}&-c\_{z} \\end{array} \\right)M\_{R}^{-1} \* \\left( \\begin{array}{1} 1&0&0\\\\ 0&1&0\\\\ 0&0&1 \\end{array} \\right)= \\left( \\begin{array}{1} a\_{x}&b\_{x}&-c\_{x}\\\\ a\_{y}&b\_{y}&-c\_{y}\\\\ a\_{z}&b\_{z}&-c\_{z} \\end{array} \\right)

由单位矩阵的性质，等式左边可以变为 M\_{R}^{-1} M\_{R}^{-1} ，而且这些变换都是在直角坐标系中进行的，所以 M\_{R}M\_{R} 是正交矩阵，其矩阵的逆等于矩阵的转置，则可得：

M\_{R}^{T}= \\left( \\begin{array}{1} a\_{x}&b\_{x}&-c\_{x}\\\\ a\_{y}&b\_{y}&-c\_{y}\\\\ a\_{z}&b\_{z}&-c\_{z} \\end{array} \\right)M\_{R}^{T}= \\left( \\begin{array}{1} a\_{x}&b\_{x}&-c\_{x}\\\\ a\_{y}&b\_{y}&-c\_{y}\\\\ a\_{z}&b\_{z}&-c\_{z} \\end{array} \\right)

最后就可以得到旋转矩阵 M\_{R}M\_{R} ，加上齐次坐标后得：

M\_{R} = \\left( \\begin{array}{1} a\_{x}&a\_{y}&a\_{z}&0\\\\ b\_{x}&b\_{y}&b\_{z}&0\\\\ -c\_{x}&-c\_{y}&-c\_{z}&0\\\\ 0&0&0&1 \\end{array} \\right) M\_{R} = \\left( \\begin{array}{1} a\_{x}&a\_{y}&a\_{z}&0\\\\ b\_{x}&b\_{y}&b\_{z}&0\\\\ -c\_{x}&-c\_{y}&-c\_{z}&0\\\\ 0&0&0&1 \\end{array} \\right)

  

### 3\. 合并

观察矩阵就是由平移矩阵和旋转矩阵相乘组合而来，则可得到观察矩阵：（知乎上写矩阵太累了）

M\_{View} = M\_{R} \*M\_{T} = \\left( \\begin{array}{1} a\_{x}&a\_{y}&a\_{z}&-a\_{x}Xw-a\_{y}Yw-a\_{z}Zw\\\\ b\_{x}&b\_{y}&b\_{z}&-b{x}Xw-b\_{y}Yw-b\_{z}Zw\\\\ -c\_{x}&-c\_{y}&-c\_{z}&c\_{x}Xw+c\_{y}Yw+c\_{z}Zw\\\\ 0&0&0&1 \\end{array} \\right)M\_{View} = M\_{R} \*M\_{T} = \\left( \\begin{array}{1} a\_{x}&a\_{y}&a\_{z}&-a\_{x}Xw-a\_{y}Yw-a\_{z}Zw\\\\ b\_{x}&b\_{y}&b\_{z}&-b{x}Xw-b\_{y}Yw-b\_{z}Zw\\\\ -c\_{x}&-c\_{y}&-c\_{z}&c\_{x}Xw+c\_{y}Yw+c\_{z}Zw\\\\ 0&0&0&1 \\end{array} \\right)

至此，观察矩阵的推导完毕；这里可以注意到c轴的方向是相反的。也就是目标指向摄像机坐标系原点的方向。

  

3\. LookUp矩阵
------------

LookUp()是OpenGL中提供的一个函数，其作用就是通过传入摄像机位置、目标位置和摄像机的向上向量即可得到观察矩阵。通过上面对观察矩阵的推导便可知道，摄像机的位置和目标位置即可得出摄像机坐标系中的c轴，摄像机向上向量可得到摄像机坐标系的b轴，而a轴可通过c叉乘b可得。[\[1\]](#ref_1)

![](https://pic1.zhimg.com/v2-c273a36afbb6da1f3f03cf5b7f2085bc_b.jpg)

![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='460' height='330'></svg>)

所以我们往LookUp函数里传入的数据就是在构建一个摄像机坐标系，以便进一步计算观察矩阵。[\[2\]](#ref_2)

![](https://pic3.zhimg.com/v2-591a3e841dd9a63c5cac07442e59146a_b.jpg)

![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='547' height='149'></svg>)

LookUp矩阵

这里的D向量是方向向量，其方向是从目标物体指向摄像机的方向。也就是上面所说的c轴反方向-c。除此之外，其余都和观察矩阵一样的写法。

其内部实现可以为：

    mat4 lookAt(const vec3& camera, const vec3& target, const vec3& up)
    {
        // 计算从目标点到摄像机位置的方向向量
        vec3 forward = camera- target;               
        // 叉乘得到左向量
        vec3 left = cross(up, forward); 
    
        // 标准化为单位向量
        normalize(left);
        normalize(forward); 
    
        // 初始化观察矩阵为单位矩阵
        mat4 viewMatrix(1.0f);
    
        // 观察矩阵逐分量赋值
        viewMatrix[0] = left.x;
        viewMatrix[4] = left.y;
        viewMatrix[8] = left.z;
        viewMatrix[1] = up.x;
        viewMatrix[5] = up.y;
        viewMatrix[9] = up.z;
        viewMatrix[2] = forward.x;
        viewMatrix[6] = forward.y;
        viewMatrix[10]= forward.z;
    
        viewMatrix[12]= -left.x * camera.x - left.y * camera.y - left.z * camera.z;
        viewMatrix[13]= -up.x * camera.x - up.y * camera.y - up.z * camera.z;
        viewMatrix[14]= -forward.x * camera.x - forward.y * camera.y - forward.z * camera.z;
    
        return viewMatrix;
    }
    

参考
--

1.  [^](#ref_1_0)songho OpenGL [http://www.songho.ca/opengl/gl\_camera.html](http://www.songho.ca/opengl/gl_camera.html)
2.  [^](#ref_2_0)LearnOpenGL\_CN [https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#look-at](https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#look-at)