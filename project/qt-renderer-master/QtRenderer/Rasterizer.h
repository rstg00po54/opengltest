#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <tuple>
#include <eigen3/Eigen/Eigen>
#include "Triangle.h"
#include <iostream>
#include "Model.h"
#include "Scene.h"

struct fragment_shader_payload
{
	Eigen::Vector3f view_pos;
	Eigen::Vector3f color;
	Eigen::Vector3f normal;
	Eigen::Vector2f tex_coord;
	//shared_ptr<Texture> texture;
	const Scene* scene;
	const Model* model;
};

class Rasterizer {
public:
	Rasterizer(int w, int h);
	~Rasterizer();

	void drawLine(int x0, int y0, int x1, int y1, Eigen::Vector3f color);
	void drawTriangle(const Triangle& t, const Scene* scene);
	void drawLineTriangle(const Triangle& t);
	
	Rasterizer operator=(Rasterizer&& right) noexcept {
		cout << this->depthBuffer << endl;
		cout << this->width << endl;
		cout << this->height << endl;
		cout << this->depthBuffer << endl;

		this->depthBuffer = right.depthBuffer;
		this->frameBuffer = right.frameBuffer;
		this->width = right.width;
		this->height = right.height;
		
		right.depthBuffer = nullptr;
		right.frameBuffer = nullptr;
	}
	unsigned char* getFrame() {
		return reinterpret_cast<unsigned char*>(frameBuffer);
	}

	//void render();
	void render(Scene* scene);
private:
	int width;
	int height;

	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	Pixel* frameBuffer;
	float* depthBuffer;
	void refreshBuffer();

	void setPixel(int x, int y, Eigen::Vector3f color);
	int getIndex(int x, int y) {
		return ((height - y - 1) * width + x);
	}

	Eigen::Vector3f(*fragment_shader)(const fragment_shader_payload& payload);

};
