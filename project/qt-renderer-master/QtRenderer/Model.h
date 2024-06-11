#pragma once
#include <vector>
#include <Eigen/eigen>
#include "Texture.h"

#define MAX_TEXTURES 3

using namespace std;

struct Vertex {
	Vertex(Eigen::Vector4f&& _v, Eigen::Vector3f&& _n, Eigen::Vector2f&& _t) {
		vert = _v;
		normal = _n;
		tex = _t;
	}
	Eigen::Vector4f vert;
	Eigen::Vector3f normal;
	Eigen::Vector2f tex;
};

/// <summary>
/// 一个Model只对应一个Mesh，一种材质
/// </summary>
class Model {

public:
	Model() {
	};
	~Model() = default;

	vector<Vertex> vertexBuffer;
	vector<unsigned int> indexBuffer;
	//Texture* texture[MAX_TEXTURES];
	shared_ptr<Texture> textures[MAX_TEXTURES];
};