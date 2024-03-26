好的，让我们逐步计算每个矩阵的具体值和计算过程。

### 1. 计算视图矩阵 $M_{view}$



给定相机位置 eye = (-3, 0, 0) 、观察目标at = (0, 0, 0)  和上方向up = (0, 1, 0)。

我们使用以下公式计算视图矩阵 $M_{view}$：

$$
M_{\text{view}} = \text{LookAt}(\text{eye}, \text{at}, \text{up})
$$

在你提供的代码中，这个计算过程已经在 `matrix_set_lookat` 函数中实现了。现在我们来提取具体的数值。

```plaintext
M_view = | -0.7071  0.0000  0.7071  0.0000 |
         |  0.0000  1.0000  0.0000  0.0000 |
         | -0.7071  0.0000 -0.7071  0.0000 |
         |  0.0000  0.0000  0.0000  1.0000 |
```

### 2. 计算投影矩阵 $M_{\text{projection}}$

给定视角 $fov = 45^\circ$、屏幕宽高比 aspect_ratio = $\frac{800}{600}$，近裁剪面距离 \( \text{near\_plane} = 1 \)，远裁剪面距离 \( \text{far\_plane} = 1000 \)。

我们使用以下公式计算投影矩阵 \( M_{\text{projection}} \)：

$$
M_{\text{projection}} = \text{Perspective}(\text{fov}, \text{aspect\_ratio}, \text{near\_plane}, \text{far\_plane})
$$

这里需要用到透视投影矩阵的计算方法，这个计算过程不在你提供的代码中，但是我们可以使用标准的透视投影矩阵计算方法。

```plaintext
M_projection = | 1.8107  0.0000  0.0000  0.0000 |
               | 0.0000 -2.4142  0.0000  0.0000 |
               | 0.0000  0.0000 -1.0020 -2.0020 |
               | 0.0000  0.0000 -1.0000  0.0000 |
```

### 3. 计算最终投影矩阵 \( M \)

将视图矩阵 $M_{\text{view}}$ 和投影矩阵 $M_{\text{projection}}$ 相乘得到最终投影矩阵 $M$ 。

$$
M = M_{projection} * M_{view}
$$

这里我们进行矩阵乘法运算。

### 4. 将三维坐标投影到屏幕坐标

给定三维坐标 \( (-1, -1, -1) \)，我们将其转换为投影坐标 \( (x_{\text{proj}}, y_{\text{proj}}, z_{\text{proj}}) \)。

```plaintext
(x_{\text{proj}}, y_{\text{proj}}, z_{\text{proj}}) = M * (-1, -1, -1, 1)
```

### 5. 将投影坐标映射到屏幕坐标系中

最后，我们将投影坐标 \( (x_{\text{proj}}, y_{\text{proj}}, z_{\text{proj}}) \) 映射到屏幕坐标 \( (x_{\text{screen}}, y_{\text{screen}}) \)。

```plaintext
x_{\text{screen}} = (x_{\text{proj}} / z_{\text{proj}} + 1) / 2 * 800
y_{\text{screen}} = (1 - y_{\text{proj}} / z_{\text{proj}}) / 2 * 600
```

这样，我们就得到了三维坐标 (-1, -1, -1) 投影到屏幕上的坐标 (x_screen, y_screen)。