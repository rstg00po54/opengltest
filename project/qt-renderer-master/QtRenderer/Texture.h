#pragma once
#define DEBUG
#include <eigen3/Eigen/eigen>
#include <string>
#include <iostream>

using namespace std;

static float clamp(float u, float min_, float max_){
	float x = min(u, max_);

	return max(x, min_);
}
class Texture {

public:
	Texture(const char* path);

	~Texture();

	int width, height;

	
	Eigen::Vector3f getPixel(const float& u, const float& v) {
		return (this->*pixFunc)(u, v);
	}

	bool isEmpty() {
		
		if (img_data == nullptr) return true;
		else return false;
		
		//return img_data;
	}
private:
	Eigen::Vector3f pixel_without_interpolate(const float& u, const float& v) {
		/*
		if (u < 0 || u > 1 || v < 0 || v>1) {
			//cout << u << ' ' << v << endl;
			return { 255,255,255 };
		}
		*/
		int uu = clamp(u, 0.f, 1.f) * (width - 1);
		int vv = clamp(v, 0.f, 1.f) * (height - 1);

		int index = (uu + vv * (width)) * 3;
		return Eigen::Vector3f(img_data[index], img_data[index + 1], img_data[index + 2]);
	}

	Eigen::Vector3f pixel_with_interpolate(const float& u, const float& v) {
		/*
		if (u < 0 || u > 1 || v < 0 || v>1) {
			cout << u << ' ' << v << endl;
			return { 255,255,255 };
		}
		*/
		float uu = clamp(u, 0.f, 1.f) * (width - 1);
		float vv = clamp(v, 0.f, 1.f) * (height - 1);

		int x = uu;
		int y = vv;

		if (x + 1 >= width) x--;
		if (y + 1 >= height) y--;

		int index = (x + y * (width)) * 3;
		Eigen::Vector3f c1 = Eigen::Vector3f(img_data[index], img_data[index + 1], img_data[index + 2]);
		Eigen::Vector3f c2 = Eigen::Vector3f(img_data[index + 3], img_data[index + 4], img_data[index + 5]);
		index += width * 3;

		Eigen::Vector3f c3 = Eigen::Vector3f(img_data[index], img_data[index + 1], img_data[index + 2]);
		Eigen::Vector3f c4 = Eigen::Vector3f(img_data[index + 3], img_data[index + 4], img_data[index + 5]);

		return ((uu - x) * c2 + (x + 1 - uu) * c1) * (y + 1 - vv) + ((uu - x) * c4 + (x + 1 - uu) * c3) * (vv - y);
	}

	Eigen::Vector3f(Texture::* pixFunc)(const float& u, const float& v);

	unsigned char* img_data;
};