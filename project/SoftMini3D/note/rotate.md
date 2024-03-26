四元数（Quaternion）可以表示三维空间中的旋转，而旋转矩阵（Rotation Matrix）则是另一种常用的旋转表示方法之一。四元数可以通过旋转矩阵来进行转换。下面我们将介绍四元数如何转换为旋转矩阵。

一个四元数可以表示为 \( q = (w, x, y, z) \)，其中 \( w \) 是实部，\( x, y, z \) 是虚部。四元数 \( q \) 的单位化形式为：
$$
| q | = \sqrt{w^2 + x^2 + y^2 + z^2} = 1 
$$
如果我们将四元数 \( q \) 转换为旋转矩阵 \( R \)，转换过程如下：

1. 首先，将四元数单位化： \( q' = \frac{q}{\| q \|} \)
2. 然后，计算旋转矩阵的元素：

$$
R = 
\begin{pmatrix}
1 - 2y'^2 - 2z'^2 & 2x'y' - 2w'z' & 2x'z' + 2w'y' \\
2x'y' + 2w'z' & 1 - 2x'^2 - 2z'^2 & 2y'z' - 2w'x' \\
2x'z' - 2w'y' & 2y'z' + 2w'x' & 1 - 2x'^2 - 2y'^2 \\
\end{pmatrix}
$$

其中 \( x', y', z', w' \) 是四元数 \( q' \) 的分量。

这样得到的旋转矩阵 \( R \) 可以用来对三维向量进行旋转变换。

举个例子，假设我们有一个单位四元数 $q = (\frac{1}{\sqrt{2}}, 0, 0, \frac{1}{\sqrt{2}})$ 表示绕 y 轴逆时针旋转 90 度的旋转。我们可以将这个四元数转换为旋转矩阵。

首先，单位化四元数 $q$：

$$
q' = \frac{q}{\| q \|} = (\frac{1}{\sqrt{2}}, 0, 0, \frac{1}{\sqrt{2}})
$$
然后，计算旋转矩阵的元素：
$$
R = 
\begin{pmatrix}
1 - 2 \cdot 0^2 - 2 \cdot (\frac{1}{\sqrt{2}})^2 & 2 \cdot 0 \cdot (\frac{1}{\sqrt{2}}) - 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 & 2 \cdot 0 \cdot \frac{1}{\sqrt{2}} + 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 \\
2 \cdot 0 \cdot (\frac{1}{\sqrt{2}}) + 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 & 1 - 2 \cdot 0^2 - 2 \cdot (\frac{1}{\sqrt{2}})^2 & 2 \cdot 0 \cdot \frac{1}{\sqrt{2}} - 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 \\
2 \cdot 0 \cdot \frac{1}{\sqrt{2}} - 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 & 2 \cdot 0 \cdot \frac{1}{\sqrt{2}} + 2 \cdot \frac{1}{\sqrt{2}} \cdot 0 & 1 - 2 \cdot 0^2 - 2 \cdot 0^2 \\
\end{pmatrix}
$$

化简得到旋转矩阵 $R$：

$$
R = 
\begin{pmatrix}
0 & 0 & 1 \\
0 & 1 & 0 \\
-1 & 0 & 0 \\
\end{pmatrix}
$$

这样得到的旋转矩阵 \( R \) 可以用来对三维向量进行绕 y 轴逆时针旋转 90 度的变换。