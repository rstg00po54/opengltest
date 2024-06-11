#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <Eigen/eigen>
#include "Model.h"

struct Light {
	Eigen::Vector3f position;
	float intensity;
};
struct Camera {
	Eigen::Vector3f position;
	Eigen::Vector3f top;
	Eigen::Vector3f front;
	float near;
	float far;
};
class Scene {

public:
	Scene() : viewMatrix(Eigen::Matrix4f::Identity()), projectionMatrix(Eigen::Matrix4f::Identity()) {
		//setCamera({ 0,1,0 }, { 0,0,-1 }, { 0,0,0 });
	}

	int addModel(string path, string texp = "", string norp = "");
	void translateModel(int index, Eigen::Vector3f vec);
	void rotateModel(int index, float angle, Eigen::Vector3f axis);
	void scaleModel(int index, Eigen::Vector3f scale);
	void rmModel(int index) {
		models.erase(models.begin() + index);
		modelMatrices.erase(modelMatrices.begin() + index);
	}

	void addLight(const Eigen::Vector3f& position, float intensity) {
		lights.push_back({position, intensity});
		lightBuffer.emplace_back();
	}
	//void setLight(int index, const Eigen::Vector3f& position, float intensity);
	//void rmLight(int index);
	
	void setCamera(Eigen::Vector3f top, Eigen::Vector3f front, Eigen::Vector3f position);
	void setPerspective(float fov, float aspect, float near, float far);
	//void setOrthogonal(float fov, float aspect, float near, float far);

	static Eigen::Matrix4f translateMatrix(Eigen::Vector3f vec) {
		Eigen::Matrix4f translate = Eigen::Matrix4f::Identity();
		translate(0, 3) = vec[0];
		translate(1, 3) = vec[1];
		translate(2, 3) = vec[2];
		return translate;
	}
	static Eigen::Matrix4f rotateMatrix(float angle, Eigen::Vector3f axis) {
		Eigen::Matrix4f rotate = Eigen::Matrix4f::Identity();
		//Rodrigues
		Eigen::Matrix3f A;
		A << 0, -axis[2], axis[1],
			axis[2], 0, -axis[0],
			-axis[1], axis[0], 0;

		float c = cos(angle / 180 * M_PI);
		rotate.block<3, 3>(0, 0) = c * Eigen::Matrix3f::Identity() + (1 - c) * axis * axis.transpose() + sin(angle / 180 * M_PI) * A;
		return rotate;
	}

public:
	Camera mainCamera;

	Eigen::Matrix4f viewMatrix;
	Eigen::Matrix4f projectionMatrix;

	vector<Model> models;
	vector<Eigen::Matrix4f> modelMatrices;
	vector<Light> lights;

	vector<Light> lightBuffer;

};