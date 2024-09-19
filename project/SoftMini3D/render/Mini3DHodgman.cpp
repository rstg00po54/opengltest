#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Mini3D.h"
#include "Mini3DRender.h"

// using namespace glm;
using namespace std;


#define Horizontal 1
#define Vertical   2

// 判断点是否在裁剪窗口内 (矩形内部)
bool isInside(const point_t& p, const point_t& min, const point_t& max,  pair<point_t, point_t> edge) {

	int flag = 0;
	bool ret = false;
	if(edge.first.x == edge.second.x) { // 竖直的线
		if(edge.first.x == min.x) {	// 四边形左边
			if(p.x > min.x){	// 点在右侧
				ret = true;
			}
		}else if (edge.first.x == max.x) {// 四边形右边
			if(p.x < max.x){	// 点在左侧
				ret = true;
			}	
		}else{
			pr_debug("error!!!\n");
		}
	}else if(edge.first.y == edge.second.y) { // 水平的线
		if(edge.first.y == min.y) {	// 四边形下边
			if(p.y > min.y){	// 点在上侧
				ret = true;
			}
		}else if (edge.first.y == max.y) {// 四边形上边
			if(p.y < max.y){	// 点在下侧
				ret = true;
			}	
		}else{
			pr_debug("error!!!\n");
		}
	}else{
		pr_debug("error !!!\n");
	}

	// pr_debug("ret %d\n", ret);
	return ret;
}

// 判断交点是否在两点之间（即线段范围内）pair<vec2, vec2> edge
bool isOnSegment(const point_t& intersect, const point_t& p1, const point_t& p2) {
	bool ret[4],r;
    ret[0] = (std::min(p1.x, p2.x) - intersect.x) <= 0.001f;
	ret[1] = (intersect.x - std::max(p1.x, p2.x)) <= 0.001f;
	ret[2] = (std::min(p1.y, p2.y) - intersect.y) <= 0.001f;
	ret[3] = (intersect.y - std::max(p1.y, p2.y)) <= 0.001f;
	r = ret[0]&&ret[1]&&ret[2]&&ret[3];
	// if(!r){
	// 	float dif = std::min(p1.x, p2.x) - intersect.x ;
	// 	pr_debug("err?\n");
	// }
	return r;
}
bool floatEqual(double a, double b, double epsilon = 1e-10) {
	return std::fabs(a - b) < epsilon;
}
// 计算两条边的交点
point_t getIntersection(const point_t& p1, const point_t& p2, const point_t& lineStart, const point_t& lineEnd) {
	float A1 = p2.y - p1.y;
	float B1 = p1.x - p2.x;
	float C1 = A1 * p1.x + B1 * p1.y;

	float A2 = lineEnd.y - lineStart.y;
	float B2 = lineStart.x - lineEnd.x;
	float C2 = A2 * lineStart.x + B2 * lineStart.y;

    float denom = A1 * B2 - A2 * B1;

    // 如果两线段平行或重合，返回无效点
    if (denom == 0) {
        return point_t(); // 返回无效交点
    }

    float x = (B2 * C1 - B1 * C2) / denom;
    float y = (A1 * C2 - A2 * C1) / denom;

    point_t intersect;
	intersect.x = x;
	intersect.y = y;
	// pr_debug(" x %3.2f/%3.2f\n", x, y);
DBL_EPSILON;

// pair<point_t, point_t> edge1 = pair<point_t, point_t>(p1,p2);
// pair<point_t, point_t> edge2 = pair<point_t, point_t>(lineStart,lineEnd);



    // // 检查交点是否在线段范围内
    // if (isOnSegment(intersect, p1, p2) && isOnSegment(intersect, lineStart, lineEnd)) {
	// 	pr_debug("vaild\n");
        return intersect; // 有效交点
    // }
	pr_debug("invaild\n");
    return point_t(); // 无效交点
}
// Sutherland-Hodgman 裁剪算法
vector<point_t> sutherlandHodgmanClip(const vector<point_t>& subjectPolygon, const point_t& min, const point_t& max) {
	// 裁剪窗口的四条边
	vector<pair<point_t, point_t>> clipEdges = {
		{point_t{min.x, max.y}, point_t{min.x, min.y}}, // 左边
		{point_t{max.x, max.y}, point_t{min.x, max.y}}, // 顶边
		{point_t{min.x, min.y}, point_t{max.x, min.y}}, // 底边
		{point_t{max.x, min.y}, point_t{max.x, max.y}}, // 右边
	};

	vector<point_t> outputPolygon = subjectPolygon;

	// 对每一条裁剪窗口的边进行裁剪
	for (const pair<point_t, point_t>& clipEdge : clipEdges) {
		vector<point_t> inputPolygon = outputPolygon;
		outputPolygon.clear();
		// pr_debug("\n");
		// pr_debug("side  %3.2f/%3.2f  %3.2f/%3.2f\n", clipEdge.first.x, clipEdge.first.y, clipEdge.second.x, clipEdge.second.y);

		if (inputPolygon.empty()) {
			break; // 如果多边形已经为空，则不再处理
		}

		point_t s = inputPolygon.back(); // 初始化为最后一个顶点

		for (const point_t& e : inputPolygon) {
			bool insideS = isInside(s, min, max, clipEdge); // 上一个顶点是否在区域内
			bool insideE = isInside(e, min, max, clipEdge); // 当前顶点是否在区域内
			// pr_debug("vec2 %3.2f/%3.2f %s  %3.2f/%3.2f %s %d/%d\n", s.x, s.y,s.name.c_str(), e.x, e.y, e.name.c_str(), insideS, insideE);

/*
s->p
s->e
情况1：S外侧，E外侧 => 不选择顶点
情况2：S内侧，E外侧 => 选择交点I
情况3：S外侧，E内侧 => 先选择交点I，然后选择E
情况4：S内侧，E内侧 => 选择顶点E
*/
			if (insideE && insideS) { // 情况4：S内侧，E内侧 => 选择顶点E
				outputPolygon.push_back(e);
			} else if (!insideE && insideS) { // 情况2：S内侧，E外侧 => 选择交点I
				point_t intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				outputPolygon.push_back(intersect);
			} else if (insideE && !insideS) { // 情况3：S外侧，E内侧 => 先选择交点I，然后选择E
				point_t intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				outputPolygon.push_back(intersect);
				outputPolygon.push_back(e);
			} else { // 情况1：S外侧，E外侧 => 不选择顶点

			}
			// 更新上一个顶点
			s = e;
		}
		// pr_debug("size %d\n", outputPolygon.size());
		// break;
	}
	// pr_debug("--size %d\n", outputPolygon.size());
	return outputPolygon;
}

vector<point_t> Hodgmanmain(point_t min, point_t max, vector<point_t> subjectPolygon) {
	// // 定义裁剪窗口
	// vec2 min(0, 0);  // 裁剪窗口的左下角
	// vec2 max(5, 5);  // 裁剪窗口的右上角

	// // 定义一个三角形作为要裁剪的多边形
	// vector<vec2> subjectPolygon = {
	//     vec2(1, 1),
	//     vec2(6, 1),
	//     vec2(3, 6)
	// };

	// 裁剪三角形
	vector<point_t> clippedPolygon = sutherlandHodgmanClip(subjectPolygon, min, max);

	// 输出裁剪后的多边形顶点
	// cout << "裁剪后的多边形顶点:\n";
	// for (const auto& p : clippedPolygon) {
	// 	cout << "(" << p.x << ", " << p.y << ")\n";
	// }

	return clippedPolygon;
}


static vertex_t vertex_interp(const vertex_t *x1, const vertex_t *x2, float t) {
	vertex_t y;
	vector_interp(&y.pos, &x1->pos, &x2->pos, t);
	y.tc.u = interp(x1->tc.u, x2->tc.u, t);
	y.tc.v = interp(x1->tc.v, x2->tc.v, t);
	y.color.r = interp(x1->color.r, x2->color.r, t);
	y.color.g = interp(x1->color.g, x2->color.g, t);
	y.color.b = interp(x1->color.b, x2->color.b, t);
	y.rhw = interp(x1->rhw, x2->rhw, t);
	return y;
}


static void clip(device_t *device, VertDraw vt, vertex_t v1, vertex_t v2, vector<vertex_t> *r) {
	vertex_t tmp1, tmp2;
	bool insideS = !(vt.t[0] > 0); // 上一个顶点是否在区域内
	bool insideE = !(vt.t[1] < 1); // 当前顶点是否在区域内

	ImGui::Checkbox("S", &insideS);
	ImGui::SameLine();
	ImGui::Checkbox("E", &insideE);

	// v1.pos = vt.screenPosition[0];
	// v2.pos = vt.screenPosition[1];

		tmp1 = v1;
		tmp2 = v2;
	if(vt.ret < 0)
		return;
/*
	情况1：S内侧，E内侧 => 选择顶点E
	情况2：S内侧，E外侧 => 选择交点I
	情况3：S外侧，E内侧 => 先选择交点I，然后选择E
	情况4：S外侧，E外侧 => 不选择顶点
*/
	if (insideE && insideS) { 
		// 情况1：S内侧，E内侧 => 选择顶点E
		tmp2.pos = vt.screenPosition[1];
		r->push_back(tmp2);
		drawCharAt(device, tmp2.pos, "t12");
		ImGui::Text("case 1 E");
	} else if (!insideE && insideS) {
		// 情况2：S内侧，E外侧 => 选择交点I
		tmp1 = vertex_interp(&v1, &v2, vt.t[0]);
		tmp1.pos = vt.screenPosition[1];
		r->push_back(tmp1);
		drawCharAt(device, tmp1.pos, "t21");
		ImGui::Text("case 2 I");
	} else if (insideE && !insideS) {
		// 情况3：S外侧，E内侧 => 先选择交点I，然后选择E

		tmp1 = vertex_interp(&v1, &v2, vt.t[0]);
		tmp1.pos = vt.screenPosition[0];

		tmp2 = vertex_interp(&v1, &v2, vt.t[1]);
		tmp2.pos = vt.screenPosition[1];

		r->push_back(tmp1);
		r->push_back(tmp2);

		drawCharAt(device, tmp1.pos, "t31");
		drawCharAt(device, tmp2.pos, "t32");


		ImGui::Text("case 3 I/E");

	} else { // 情况4：S外侧，E外侧 => 选择交点S/E
		tmp1 = vertex_interp(&v1, &v2, vt.t[0]);
		tmp2 = vertex_interp(&v1, &v2, vt.t[1]);
		tmp1.pos = vt.screenPosition[0];
		tmp2.pos = vt.screenPosition[1];
		r->push_back(tmp1);
		r->push_back(tmp2);
		drawCharAt(device, tmp1.pos, "t41");
		drawCharAt(device, tmp2.pos, "t42");
		ImGui::Text("case 4 S/E");
	}

}

vector<vertex_t> Hodgmanmain(device_t *device, VertDraw vt1, VertDraw vt2, VertDraw vt3, vertex_t v1, vertex_t v2, vertex_t v3){
	vector<vertex_t> ret;
	clip(device, vt1, v1, v2, &ret);
	clip(device, vt2, v2, v3, &ret);
	clip(device, vt3, v3, v1, &ret);
	// ImGui::Text("push --");
	// if(vt3.t[0] > 0) {
	// 	ImGui::Text("push 0");
	// }
	// if(vt3.t[1] < 1.f) {
	// 	ImGui::Text("push 1");
	// }
	// if(vt3.t[0] == 0.f && vt3.t[1] == 1.f){
	// 	ImGui::Text("no push");
	// }


	return ret;
}