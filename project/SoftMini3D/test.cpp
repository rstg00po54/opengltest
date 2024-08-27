struct Vertex {
    float x_w, y_w, z_w;  // 实际三维坐标
    float x_s, y_s;       // 屏幕坐标
    float rhw;            // 透视齐次坐标
};

// 已知顶点
Vertex v1 = {0.0f, 0.0f, 0.0f, 100.0f, 200.0f, 1.0f};  // (x_w, y_w, z_w, x_s, y_s, rhw)
Vertex v2 = {10.0f, 10.0f, 10.0f, 300.0f, 400.0f, 0.5f}; // (x_w, y_w, z_w, x_s, y_s, rhw)

// 目标屏幕坐标
float x_target = 200.0f;
float y_target = 300.0f;

// 计算插值参数
float p_x = (x_target - v1.x_s) / (v2.x_s - v1.x_s);
float p_y = (y_target - v1.y_s) / (v2.y_s - v1.y_s);

// 线性插值透视齐次坐标
float rhw_target = v1.rhw + p_x * (v2.rhw - v1.rhw);

// 线性插值实际三维坐标
float x_target_w = v1.x_w + p_x * (v2.x_w - v1.x_w);
float y_target_w = v1.y_w + p_y * (v2.y_w - v1.y_w);
float z_target_w = v1.z_w + p_x * (v2.z_w - v1.z_w);

// 应用透视校正
float x_real = x_target_w * rhw_target;
float y_real = y_target_w * rhw_target;
float z_real = z_target_w * rhw_target;


