#pragma once
#include <vector>
#include <eigen3/Eigen/eigen>
#include "Texture.h"
#include <memory>
// #include <boost/make_shared.hpp>

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
/// һ��Modelֻ��Ӧһ��Mesh��һ�ֲ���
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
