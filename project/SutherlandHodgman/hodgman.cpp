#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "main.h"
// using namespace glm;
using namespace std;

#include <iostream>
#include <vector>


#define Horizontal 1
#define Vertical   2
// 定义一个结构体来表示点
// struct vec2 {
//     float x, y;
//     vec2(float x = 0, float y = 0) : x(x), y(y) {}
// };

// 判断点是否在裁剪窗口内 (矩形内部)
bool isInside(const vec2& p, const vec2& min, const vec2& max,  pair<vec2, vec2> edge) {

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

	pr_debug("ret %d\n", ret);
	return ret;
}

// 判断交点是否在两点之间（即线段范围内）pair<vec2, vec2> edge
bool isOnSegment(const vec2& intersect, const vec2& p1, const vec2& p2) {
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
vec2 getIntersection(const vec2& p1, const vec2& p2, const vec2& lineStart, const vec2& lineEnd) {
	float A1 = p2.y - p1.y;
	float B1 = p1.x - p2.x;
	float C1 = A1 * p1.x + B1 * p1.y;

	float A2 = lineEnd.y - lineStart.y;
	float B2 = lineStart.x - lineEnd.x;
	float C2 = A2 * lineStart.x + B2 * lineStart.y;

    float denom = A1 * B2 - A2 * B1;

    // 如果两线段平行或重合，返回无效点
    if (denom == 0) {
        return vec2(); // 返回无效交点
    }

    float x = (B2 * C1 - B1 * C2) / denom;
    float y = (A1 * C2 - A2 * C1) / denom;

    vec2 intersect(x, y);
	pr_debug(" x %3.2f/%3.2f\n", x, y);
DBL_EPSILON;

pair<vec2, vec2> edge1 = pair<vec2, vec2>(p1,p2);
pair<vec2, vec2> edge2 = pair<vec2, vec2>(lineStart,lineEnd);



    // // 检查交点是否在线段范围内
    // if (isOnSegment(intersect, p1, p2) && isOnSegment(intersect, lineStart, lineEnd)) {
	// 	pr_debug("vaild\n");
        return intersect; // 有效交点
    // }
	pr_debug("invaild\n");
    return vec2(); // 无效交点
}
// Sutherland-Hodgman 裁剪算法
vector<vec2> sutherlandHodgmanClip(const vector<vec2>& subjectPolygon, const vec2& min, const vec2& max) {
	// 裁剪窗口的四条边
	vector<pair<vec2, vec2>> clipEdges = {
		{vec2(min.x, max.y), vec2(min.x, min.y)}, // 左边
		{vec2(max.x, max.y), vec2(min.x, max.y)}, // 顶边
		{vec2(min.x, min.y), vec2(max.x, min.y)}, // 底边
		{vec2(max.x, min.y), vec2(max.x, max.y)}, // 右边
	};

	vector<vec2> outputPolygon = subjectPolygon;

	// 对每一条裁剪窗口的边进行裁剪
	for (const pair<vec2, vec2>& clipEdge : clipEdges) {
		vector<vec2> inputPolygon = outputPolygon;
		outputPolygon.clear();
		pr_debug("\n");
		pr_debug_33("side  %3.2f/%3.2f  %3.2f/%3.2f\n", clipEdge.first.x, clipEdge.first.y, clipEdge.second.x, clipEdge.second.y);

		if (inputPolygon.empty()) {
			break; // 如果多边形已经为空，则不再处理
		}

		vec2 s = inputPolygon.back(); // 初始化为最后一个顶点

		for (const vec2& e : inputPolygon) {
			bool insideS = isInside(s, min, max, clipEdge); // 上一个顶点是否在区域内
			bool insideE = isInside(e, min, max, clipEdge); // 当前顶点是否在区域内
			pr_debug("vec2 %3.2f/%3.2f %s  %3.2f/%3.2f %s %d/%d\n", s.x, s.y,s.name.c_str(), e.x, e.y, e.name.c_str(), insideS, insideE);

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
				vec2 intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				outputPolygon.push_back(intersect);
			} else if (insideE && !insideS) { // 情况3：S外侧，E内侧 => 先选择交点I，然后选择E
				vec2 intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				outputPolygon.push_back(intersect);
				outputPolygon.push_back(e);
			} else { // 情况1：S外侧，E外侧 => 不选择顶点

			}
#if 0
			if (insideE) { // 当前顶点在裁剪区域内
				if (!insideS) { // 上一个顶点在外部
					// 计算交点并加入结果
					vec2 intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
					if (intersect.x != 0 || intersect.y != 0) {
						pr_debug("push to out 1 %s, %3.2f %3.2f\n", intersect.name.c_str(), intersect.x, intersect.y);
						outputPolygon.push_back(intersect);
					}
				}
				pr_debug("push to out 2 %s, %3.2f %3.2f\n", e.name.c_str(), e.x, e.y);
				outputPolygon.push_back(e); // 当前顶点在内部，直接添加
			} else if (insideS) { // 当前顶点在外部，上一个顶点在内部
				// 计算交点并添加
				vec2 intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				pr_debug("push to out 3 %s, %3.2f %3.2f\n", intersect.name.c_str(), intersect.x, intersect.y);
				if (intersect.x != 0 || intersect.y != 0) {
					outputPolygon.push_back(intersect);
				}
			} else { // insideS 和 insideE 都为 false 的情况
				// 虽然两个顶点都在外部，但需要检查它们的边是否与裁剪边相交
				vec2 intersect = getIntersection(s, e, clipEdge.first, clipEdge.second);
				pr_debug("false %3.2f/%3.2f \n", intersect.x, intersect.y);
				if (intersect.x != 0 || intersect.y != 0) { // 如果存在交点
					pr_debug("push to out 4 %s, %3.2f %3.2f\n", intersect.name.c_str(), intersect.x, intersect.y);
					outputPolygon.push_back(intersect);
				}
			}
#endif
			// 更新上一个顶点
			s = e;
		}
		// pr_debug("size %d\n", outputPolygon.size());
		// break;
	}
	pr_debug("--size %d\n", outputPolygon.size());
	return outputPolygon;
}

vector<vec2> Hodgmanmain(vec2 min, vec2 max, vector<vec2> subjectPolygon) {
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
	vector<vec2> clippedPolygon = sutherlandHodgmanClip(subjectPolygon, min, max);

	// 输出裁剪后的多边形顶点
	// cout << "裁剪后的多边形顶点:\n";
	// for (const auto& p : clippedPolygon) {
	// 	cout << "(" << p.x << ", " << p.y << ")\n";
	// }

	return clippedPolygon;
}
