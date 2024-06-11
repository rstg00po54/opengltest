#pragma once
#include <Eigen/Eigen>
#include <vector>
#include "Texture.h"
#include "Model.h"
using namespace std;

struct TriVert {
	Eigen::Vector4f pos;
	Eigen::Vector3f normal;
	Eigen::Vector2f tex_coord;
	Eigen::Vector3f color;
	Eigen::Vector3f view_pos;

	const TriVert operator*(const float& f) const{
		return { f * pos, f * normal, f * tex_coord, f * color, f * view_pos };
	}
	const TriVert operator/(const float& f) const {
		return { pos / f, normal / f, tex_coord / f, color / f, view_pos / f };
	}
	const TriVert operator+(const TriVert& tv) const {
		return { pos + tv.pos, normal + tv.normal, tex_coord + tv.tex_coord, color + tv.color, view_pos + tv.view_pos };
	}
	const TriVert operator-(const TriVert& tv) const {
		return { pos - tv.pos, normal - tv.normal, tex_coord - tv.tex_coord, color - tv.color, view_pos - tv.view_pos };
	}
	void operator*=(const float& f){
		pos *= f; normal *= f; color *= f; view_pos *= f; tex_coord *= f;
	}
	void operator/=(const float& f) {
		pos /= f; normal /= f; color /= f; view_pos /= f; tex_coord /= f;
	}
};

inline const TriVert operator*(const float& f, const TriVert& tv) {
	return { f * tv.pos, f * tv.normal, f * tv.tex_coord, f * tv.color, f * tv.view_pos };
}

class Triangle {
public:
	Triangle() = default;
	Triangle(const TriVert& va, const TriVert& vb, const TriVert& vc, Model* m = nullptr) : 
		vertices{ va.pos, vb.pos, vc.pos }, 
		normals{ va.normal, vb.normal, vc.normal }, 
		tex_coords{ va.tex_coord, vb.tex_coord, vc.tex_coord },
		color{va.color, vb.color, vc.color},
		view_pos{va.view_pos, vb.view_pos, vc.view_pos},
		model(m) {}
	~Triangle() = default;

	//TriVert vertices[3];

	
	Eigen::Vector4f vertices[3]; //转换到屏幕空间的齐次坐标

	Eigen::Vector3f normals[3]; //法线
	Eigen::Vector2f tex_coords[3]; //纹理坐标uv
	Eigen::Vector3f color[3]; //顶点颜色

	Eigen::Vector3f view_pos[3]; //经过mv矩阵后的世界坐标
	
	//shared_ptr<Texture> normalTexture;
	Model* model;
};

