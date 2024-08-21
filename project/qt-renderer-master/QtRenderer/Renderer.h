#pragma once
#include "Rasterizer.h"
// #include <QImage>
// #include <QLabel>
// #include <QKeyEvent>
#include "Scene.h"
#include <thread>
#include <mutex>
// #include <QDebug>
#include <condition_variable>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

using namespace std;

class Renderer 
{
public:
	Renderer();
	~Renderer();

	int getFPS() {
		int temp = fps;
		fps = 0;
		return temp;
	}

	int addModel(string& m, string& t, string& n);
	void rmModel(int index);

	bool selectModel(int index) {
		if (index >= scene.models.size() || index < 0) return false;
		this->index = index;
		return true;
	}
private:
	Rasterizer rasterizer;
	Scene scene;

	void renderThread();
	std::thread rt;

	int index;

	int fps = 0;
	int FPSLimit = 45;
	bool rendering = false;

	bool pause = false, resumed = false;
	std::mutex pause_mtx;
	std::condition_variable render_cv;
	std::condition_variable modifier_cv;

	// QPoint dragPosition;

	Camera originCamera;
	float cameraR = 15;
protected:
	// void keyPressEvent(QKeyEvent* event) override;

	// void mousePressEvent(QMouseEvent* event) override {
	// 	// ????????????¦Ë??
	// 	if (event->button() == Qt::MouseButton::LeftButton || event->button() == Qt::MouseButton::RightButton) {
	// 		dragPosition = event->globalPos();
	// 		originCamera = scene.mainCamera;
	// 	}
	// }

	// void mouseMoveEvent(QMouseEvent* event) override {
	// 	// ???????????????????????
	// 	if (event->buttons() & Qt::LeftButton) {
	// 		QPoint p = event->globalPos() - dragPosition;

	// 		//???????
	// 		Eigen::Vector4f pos = Scene::translateMatrix(-originCamera.position - originCamera.front * cameraR) * Eigen::Vector4f{ originCamera.position[0], originCamera.position[1], originCamera.position[2] , 1 };
	// 		//???
	// 		Eigen::Matrix4f rotate = Scene::rotateMatrix(float(p.x()) / width() * 180, { 0, 1, 0 }) * Scene::rotateMatrix(float(p.y()) / height() * 180, originCamera.top.cross(originCamera.front).normalized()) ;
	// 		pos = rotate * pos;
	// 		//????¦Ë??
	// 		pos = Scene::translateMatrix(originCamera.position + originCamera.front * cameraR) * pos;
			
	// 		Eigen::Vector3f front = (originCamera.position + originCamera.front * cameraR - pos.head<3>()).normalized();
	// 		Eigen::Vector3f top = (rotate * Eigen::Vector4f{ originCamera.top[0], originCamera.top[1], originCamera.top[2], 0 }).head<3>();

	// 		scene.setCamera(top.normalized(), front, pos.head<3>());

	// 	}
	// 	else if (event->buttons() & Qt::RightButton) {
	// 		QPoint p = event->globalPos() - dragPosition;

	// 		Eigen::Vector3f left = originCamera.top.cross(originCamera.front);
	// 		Eigen::Vector4f pos = Scene::translateMatrix(float(-p.x()) / width() * 10 * left + float(p.y()) / height() * 10 * originCamera.top) * Eigen::Vector4f{ originCamera.position[0], originCamera.position[1], originCamera.position[2] , 1 };
	// 		scene.setCamera(originCamera.top, originCamera.front, pos.head<3>());
	// 	}
	// }

	// void wheelEvent(QWheelEvent* event) override {
	// 	const float scaleFactor = 0.01;
	// 	scene.setCamera(scene.mainCamera.top, scene.mainCamera.front, event->delta() * scaleFactor * scene.mainCamera.front + scene.mainCamera.position);
	// }
};

