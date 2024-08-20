#include "Rasterizer.h"
#include <climits>
#include <chrono>
#include "main.h"
// #include <QImage>
int64_t getTimestamp() ;
static Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    Eigen::Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}


static Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd;
    if (payload.model->textures[0]) {
        kd = payload.model->textures[0]->getPixel(payload.tex_coord.x(), payload.tex_coord.y());
        kd /= 255.f;
    }
    else kd = payload.color/255.f;

    Eigen::Vector3f normal;
    if (payload.model->textures[1]) {
        normal = payload.model->textures[1]->getPixel(payload.tex_coord.x(), payload.tex_coord.y());
        normal.normalize();
    }
    else normal = payload.normal;

    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    float amb_light_intensity = 10;

    float p = 150;

    Eigen::Vector3f point = payload.view_pos;
    

    Eigen::Vector3f result_color = { 0, 0, 0 };

    Eigen::Vector3f ambient = ka.cwiseProduct(kd) * amb_light_intensity;
    for (auto& light : payload.scene->lightBuffer)
    {
        Eigen::Vector3f lvec = (light.position - point);
        float intensity = light.intensity / lvec.dot(lvec);
        lvec.normalize();
        Eigen::Vector3f viewvec = (payload.scene->mainCamera.position - point).normalized();
        Eigen::Vector3f halfvec = (lvec + viewvec).normalized();
        //lambert
        Eigen::Vector3f diffuse = kd * (intensity * std::max(.0f, normal.dot(lvec)));
        //half-lambert
        //Eigen::Vector3f diffuse = kd * (intensity * (std::max(.0f, normal.dot(lvec)) * 0.5 + 0.5));
        Eigen::Vector3f specular = ks * (intensity * pow(std::max(.0f, normal.dot(halfvec)), p));

        result_color += ambient + diffuse + specular;
    }

    return result_color * 255.f;
}

Rasterizer::Rasterizer(int w, int h) : width(w), height(h){
	frameBuffer = new Pixel[w * h];
	depthBuffer = new float[w * h];
    refreshBuffer();

    fragment_shader = phong_fragment_shader;
}

Rasterizer::~Rasterizer() {
	delete[] frameBuffer;
	delete[] depthBuffer;
}

/// <summary>
/// Bresenham’s algorithm
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void Rasterizer::drawLine(int x0, int y0, int x1, int y1, Eigen::Vector3f color = {255, 255, 255}) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            setPixel(y, x, color);
        }
        else {
            setPixel(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }

}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Eigen::Vector4f* v) {
    //float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
    //float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
    
    float c1 = ((y - v[1].y()) * (v[2].x() - v[1].x()) - (x - v[1].x()) * (v[2].y() - v[1].y()))
             / ((v[0].y() - v[1].y()) * (v[2].x() - v[1].x()) - (v[0].x() - v[1].x()) * (v[2].y() - v[1].y()));
    float c2 = ((y - v[2].y()) * (v[0].x() - v[2].x()) - (x - v[2].x()) * (v[0].y() - v[2].y()))
             / ((v[1].y() - v[2].y()) * (v[0].x() - v[2].x()) - (v[1].x() - v[2].x()) * (v[0].y() - v[2].y()));
    
    //float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
    return { c1,c2,1 - c1 - c2 };
}

template<typename T>
static const T interpolate(float alpha, float beta, float gamma, float Z, const T* i, const Eigen::Vector4f* tv) {
    T it = alpha * i[0] / tv[0].w() + beta * i[1] / tv[1].w() + gamma * i[2] / tv[2].w();
    it *= Z;
    return it;
}

static bool backTest(const Eigen::Vector4f* v) {
    Eigen::Vector3f a = (v[1] - v[0]).head<3>();
    a[2] = 0;
    Eigen::Vector3f b = (v[2] - v[0]).head<3>();
    b[2] = 0;
    return a.cross(b).z() > 0;
}

void Rasterizer::drawTriangle(const Triangle& t, const Scene* scene) {
    if (backTest(t.vertices)) return;
    // pr_debug("%s +++", __func__);

    /*
    cout << t.tex_coords[0] << endl;
    cout << t.tex_coords[1] << endl;
    cout << t.tex_coords[2] << endl;
    */

    int left = min(min(t.vertices[0].x(), t.vertices[1].x()), t.vertices[2].x());
    int right = max(max(t.vertices[0].x(), t.vertices[1].x()), t.vertices[2].x());
    int bottom = min(min(t.vertices[0].y(), t.vertices[1].y()), t.vertices[2].y());
    int top = max(max(t.vertices[0].y(), t.vertices[1].y()), t.vertices[2].y());
    // pr_debug("%3.2f/%3.2f/%3.2f", t.vertices[0].x(), t.vertices[0].y(), t.vertices[0].z());
    // pr_debug("%3.2f/%3.2f/%3.2f", t.vertices[1].x(), t.vertices[1].y(), t.vertices[1].z());
    // pr_debug("%3.2f/%3.2f/%3.2f", t.vertices[2].x(), t.vertices[2].y(), t.vertices[2].z());
    // printf("w %d, h %d \n", right-left, top-bottom);
    // int64_t begin =  getTimestamp();

    for (int y = bottom; y <= top; y++) {
		int64_t begin =  getTimestamp();
		int64_t end;
        for (int x = left; x <= right; x++) {
            auto [alpha, beta, gamma] = computeBarycentric2D(x + 0.5, y + 0.5, t.vertices);
            if (alpha > 0 && beta > 0 && gamma > 0) {
                float Z = 1.0 / (alpha / t.vertices[0].w() + beta / t.vertices[1].w() + gamma / t.vertices[2].w());
                /*
                float zp = alpha * t.vertices[0].z() / t.vertices[0].w() + beta * t.vertices[1].z() / t.vertices[1].w() + gamma * t.vertices[2].z() / t.vertices[2].w();
                zp *= Z;
                */
                float zp = alpha * t.view_pos[0].z() + beta * t.view_pos[1].z() + gamma * t.view_pos[2].z();
                int index = getIndex(x, y);
                if (zp < depthBuffer[index]) {
                    fragment_shader_payload payload;
                    Eigen::Vector3f color = {0,200,0};

                    payload.normal = interpolate(alpha, beta, gamma, Z, t.normals, t.vertices).normalized();
                    payload.color = interpolate(alpha, beta, gamma, Z, t.color, t.vertices);
                    payload.view_pos = interpolate(alpha, beta, gamma, Z, t.view_pos, t.vertices);
                    payload.tex_coord = interpolate(alpha, beta, gamma, Z, t.tex_coords, t.vertices);
                    payload.model = t.model;
                    payload.scene = scene;

                    color = fragment_shader(payload);

                    setPixel(x, y, color);
                    depthBuffer[index] = zp;
                }
            }
        }
		end = getTimestamp();
		// pr_debug("time %d ms", end-begin);
    }
    // printf("%s ---\n", __func__);
}

void Rasterizer::setPixel(int x, int y, Eigen::Vector3f color) {
    int index = getIndex(x, y);
    frameBuffer[index].r = clamp(color.x(), 0.f, 255.f);
    frameBuffer[index].g = clamp(color.y(), 0.f, 255.f);
    frameBuffer[index].b = clamp(color.z(), 0.f, 255.f);
}

void Rasterizer::refreshBuffer() {
    memset(frameBuffer, 0, width * height * sizeof(Pixel));

    for (int i = 0; i < width * height; i++) depthBuffer[i] = INFINITY;
}

void Rasterizer::drawLineTriangle(const Triangle& t) {
    drawLine(t.vertices[0].x(), t.vertices[0].y(), t.vertices[1].x(), t.vertices[1].y());
    drawLine(t.vertices[1].x(), t.vertices[1].y(), t.vertices[2].x(), t.vertices[2].y());
    drawLine(t.vertices[2].x(), t.vertices[2].y(), t.vertices[0].x(), t.vertices[0].y());
}


/*
方向的判断原理：

 b /
 /
a
正值：如果 a×b>0，则 b 在 a 的逆时针方向。 

负值：如果 a×b<0，则 b 在 a 的顺时针方向。
零值：如果 a×b=0，则 a 和 b 共线，即它们在同一条直线上。
*/
//sutherland-hodgman
const static Eigen::Vector4f clipPlanes[6]{ 
        Eigen::Vector4f(0,0,1,1),
        Eigen::Vector4f(0,0,-1,1),
        Eigen::Vector4f(1,0,0,1),
        Eigen::Vector4f(-1,0,0,1),
        Eigen::Vector4f(0,-1,0,1),
        Eigen::Vector4f(0,1,0,1) 
        };
/*
如果结果大于0：点在半空间内部。
等于零在平面上
如果结果小于零：点在半空间外部。
*/
static inline bool insideClip(const Eigen::Vector4f& plane, const Eigen::Vector4f& point) {
    return plane.dot(point) >= 0;
}

static inline TriVert intersect(const TriVert& v1, const TriVert& v2, const Eigen::Vector4f& plane) {
    float da = v1.pos.dot(plane);
    float db = v2.pos.dot(plane);

    float weight = da / (da - db);
    return v1 + (v2 - v1) * weight;
}
static int flag;
static inline vector<Triangle> clip(const TriVert& va, const TriVert& vb, const TriVert& vc) {
    vector<TriVert> output{ va, vb, vc };
    output.reserve(7);// 对vector预留内存空间，使用前并不申请空间
    vector<TriVert> input;
    input.reserve(7);
    pr_debug("+++ %d", input.size());
    for (int i = 0; i < 6; i++) {
        //vector<TriVert> input;
        swap(input, output);
        output.clear();
		pr_debug("+++ %d", input.size());
        for (int j = 0; j < input.size(); j++) {
            TriVert& cur = input[j];
            TriVert& prev = input[(j + input.size() - 1) % input.size()];
			// pr_debug("cur %d prev %d", j,(j + input.size() - 1) % input.size());
			// 在平面上或内部
            if (insideClip(clipPlanes[i], cur.pos)) { // 
				//外部
                if (!insideClip(clipPlanes[i], prev.pos)) { //
                    TriVert intersecting = intersect(prev, cur, clipPlanes[i]);
					intersecting.number = flag++;
                    output.push_back(intersecting);
					pr_debug("push 1 %d", intersecting.number);
                }else{
					
					// pr_debug("is in plane");
				}
				cur.number = flag++;
				pr_debug("push 2 %d", cur.number);
                output.push_back(cur);
            }
			// 在平面上或内部
            else if (insideClip(clipPlanes[i], prev.pos)) {
                TriVert intersecting = intersect(prev, cur, clipPlanes[i]);
                output.push_back(intersecting);
				intersecting.number = flag++;
				pr_debug("push 3 %d", intersecting.number);
            }
        }
		pr_debug("output in loop %d", output.size());
    }
	pr_debug("number %d", flag);
    vector<Triangle> ans;
    for (int i = 1; i < output.size(); i++) {
        ans.emplace_back(output[0], output[i], output[(i + 1) % output.size()]);
		pr_debug("push %d: %d/%d/%d", i, output[0].number, output[i].number, output[(i + 1) % output.size()].number);
    }
	pr_debug("output %d", output.size());
	pr_debug("ans %d", ans.size());
    return ans;

}
/*
static inline bool allInside(const TriVert& va, const TriVert& vb, const TriVert& vc) {
    return insideClip(clipPlanes[0], va.pos) && insideClip(clipPlanes[0], vb.pos) && insideClip(clipPlanes[0], vc.pos) &&
        insideClip(clipPlanes[1], va.pos) && insideClip(clipPlanes[1], vb.pos) && insideClip(clipPlanes[1], vc.pos) &&
        insideClip(clipPlanes[2], va.pos) && insideClip(clipPlanes[2], vb.pos) && insideClip(clipPlanes[2], vc.pos) &&
        insideClip(clipPlanes[3], va.pos) && insideClip(clipPlanes[3], vb.pos) && insideClip(clipPlanes[3], vc.pos) &&
        insideClip(clipPlanes[4], va.pos) && insideClip(clipPlanes[4], vb.pos) && insideClip(clipPlanes[4], vc.pos) &&
        insideClip(clipPlanes[5], va.pos) && insideClip(clipPlanes[5], vb.pos) && insideClip(clipPlanes[5], vc.pos);
}
*/


void Rasterizer::render(Scene* scene) {

    refreshBuffer();

    for (int i = 0; i < scene->lights.size(); i++) {
        scene->lightBuffer[i] = scene->lights[i];
        scene->lightBuffer[i].position = (scene->viewMatrix * Eigen::Vector4f{ scene->lightBuffer[i].position[0], scene->lightBuffer[i].position[1], scene->lightBuffer[i].position[2], 1 }).head<3>();
        
    }

    float f1 = (scene->mainCamera.far - scene->mainCamera.near) / 2.0;
    float f2 = (scene->mainCamera.far + scene->mainCamera.near) / 2.0;
#if 0
// C++ : compute the matrix
glm::mat4 MVPmatrix = projection * view * model; // Remember : inverted !
// GLSL : apply it
transformed_vertex = MVP * in_vertex

#endif
     //组装，变换，裁剪，绘制
    for (int index = 0; index < scene->models.size(); index++) {
        Eigen::Matrix4f mv = scene->viewMatrix * scene->modelMatrices[index];
        Eigen::Matrix3f normalMatrix = mv.block<3, 3>(0, 0).inverse().transpose();

        Model& model = scene->models[index];
        // pr_debug("index buffer size %d\n", model.indexBuffer.size());

        for (int i = 0; i < model.indexBuffer.size(); i += 3) {
            // pr_debug("draw %d +++", i);

            int ia = model.indexBuffer[i];
            int ib = model.indexBuffer[i + 1];
            int ic = model.indexBuffer[i + 2];

            TriVert va, vb, vc;

            va.pos = mv * model.vertexBuffer[ia].vert;
            vb.pos = mv * model.vertexBuffer[ib].vert;
            vc.pos = mv * model.vertexBuffer[ic].vert;

            // pr_debug("vertex0 %3.2f/%3.2f/%3.2f",model.vertexBuffer[ia].vert.x(),model.vertexBuffer[ia].vert.y(),model.vertexBuffer[ia].vert.z());
            // pr_debug("vertex1 %3.2f/%3.2f/%3.2f",model.vertexBuffer[ib].vert.x(),model.vertexBuffer[ib].vert.y(),model.vertexBuffer[ib].vert.z());
            // pr_debug("vertex2 %3.2f/%3.2f/%3.2f",model.vertexBuffer[ic].vert.x(),model.vertexBuffer[ic].vert.y(),model.vertexBuffer[ic].vert.z());

            va.view_pos = va.pos.head<3>();
            vb.view_pos = vb.pos.head<3>();
            vc.view_pos = vc.pos.head<3>();

            // pr_debug("view.pos %3.2f/%3.2f/%3.2f", va.view_pos.x(), va.view_pos.y(), va.view_pos.z());

            va.normal = (normalMatrix * model.vertexBuffer[ia].normal).normalized();
            vb.normal = (normalMatrix * model.vertexBuffer[ib].normal).normalized();
            vc.normal = (normalMatrix * model.vertexBuffer[ic].normal).normalized();

            va.tex_coord = model.vertexBuffer[ia].tex;
            vb.tex_coord = model.vertexBuffer[ib].tex;
            vc.tex_coord = model.vertexBuffer[ic].tex;

            va.pos = scene->projectionMatrix * va.pos;
            vb.pos = scene->projectionMatrix * vb.pos;
            vc.pos = scene->projectionMatrix * vc.pos;

            // pr_debug("v.pos %3.2f/%3.2f/%3.2f", va.pos.x(), va.pos.y(), va.pos.z());

            // vector<Triangle> triangles = clip(va, vb, vc);
            vector<Triangle> triangles;
			triangles.emplace_back(va, vb, vc);

            for (auto& t : triangles) {
                //Perspective Divide
                for (auto& vec : t.vertices) {
                    vec[0] /= vec[3];
                    vec[1] /= vec[3];
                    vec[2] /= vec[3];
                }

                //Viewport Trans
                for (auto& vert : t.vertices)
                {
                    vert.x() = 0.5 * width * (vert.x() + 1.0);
                    vert.y() = 0.5 * height * (vert.y() + 1.0);
                    vert.z() = vert.z() * f1 + f2;
                }
                //if (model.textures[0]) t.texture = model.textures[0];
                t.model = &model;

                t.color[0] = { 0, 255, 255 };
                t.color[1] = { 255, 0, 0 };
                t.color[2] = { 0, 255, 0 };
				static int i = 0;
                int64_t t0 = getTimestamp();
				// if(i == 2){
                	drawTriangle(t, scene);
				// }
					i++;
                int64_t t1 = getTimestamp();
                // pr_debug("time %ld", t1 - t0);
                // break;
            }
        }

    }
}
int64_t getTimestamp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
