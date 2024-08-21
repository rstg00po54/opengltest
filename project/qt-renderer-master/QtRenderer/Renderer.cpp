#include "Renderer.h"
// #include <QDebug>
// #include <QDateTime>

Renderer::Renderer()
	: rasterizer(WIDTH, HEIGHT), index(-1)
{
    // setFixedSize(WIDTH, HEIGHT);
    // setFocusPolicy(Qt::StrongFocus);

    scene.setCamera(Eigen::Vector3f{ 0,1,0.3 }.normalized(), Eigen::Vector3f{ 0,-0.3,1 }.normalized(), { 0,5,-5 });
    scene.setPerspective(60, float(HEIGHT) / WIDTH, 0.1f, 50.f);

    scene.addLight(Eigen::Vector3f(20, 20, 20), 500);
    scene.addLight(Eigen::Vector3f(-20, 20, 0), 500);

    rendering = true;
    rt = std::thread(&Renderer::renderThread, this);
}

Renderer::~Renderer()
{
    rendering = false;
    rt.join();
}

// void Renderer::keyPressEvent(QKeyEvent* event) {
//     if (index < 0) return;
//     switch (event->key()) {
//     case Qt::Key_W:
//         scene.translateModel(index, { 0,0,0.1 });
//         break;
//     case Qt::Key_S:
//         scene.translateModel(index, { 0,0,-0.1 });
//         break;
//     case Qt::Key_A:
//         scene.translateModel(index, { -0.1,0,0 });
//         break;
//     case Qt::Key_D:
//         scene.translateModel(index, { 0.1,0,0 });
//         break;
//     case Qt::Key_Q:
//         scene.rotateModel(index, -1, { 0,1,0 });
//         break;
//     case Qt::Key_E:
//         scene.rotateModel(index, 1, { 0,1,0 });
//         break;
//     case Qt::Key_Z:
//         scene.scaleModel(index, { 1.2,1.2,1.2 });
//         break;
//     case Qt::Key_X:
//         scene.scaleModel(index, { 0.8,0.8,0.8 });
//         break;
//     default:
//         break;
//     }
//     /*
//     rasterizer.render(&scene);
//     QImage img(rasterizer.getFrame(), WIDTH, HEIGHT, QImage::Format::Format_RGB888);
//     this->setPixmap(QPixmap::fromImage(img));
//     */
// }

void Renderer::renderThread() {
    pause = false; resumed = false;
    int seg = 1000 / FPSLimit;
    // qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while (rendering) {

        // qint64 t = QDateTime::currentDateTime().toMSecsSinceEpoch();

        // if (t - start > seg) {
            rasterizer.render(&scene);
            // QImage img(rasterizer.getFrame(), WIDTH, HEIGHT, QImage::Format::Format_RGB888);
            // this->setPixmap(QPixmap::fromImage(img));
            fps++;

            // start = t;
        // }

        {
            std::unique_lock<std::mutex> lock(pause_mtx);
            if (pause) {
                resumed = true;
                modifier_cv.notify_one();
                render_cv.wait(lock, [&]() { return !pause; });
                resumed = false;
            }
        }

    }
}

int Renderer::addModel(string& m, string& t, string& n) {
    {
        std::unique_lock<std::mutex> lock(pause_mtx);
        pause = true;
        modifier_cv.wait(lock, [&] { return resumed; });
    }

    // int result = scene.addModel(m.toStdString(), t.toStdString(), n.toStdString());

    {
        std::unique_lock<std::mutex> lock(pause_mtx);
        pause = false;
        render_cv.notify_one();
    }

    return 0;
}

void Renderer::rmModel(int index) {
    {
        std::unique_lock<std::mutex> lock(pause_mtx);
        pause = true;
        modifier_cv.wait(lock, [&] { return resumed; });
    }

    scene.rmModel(index);
    this->index = -1;
    {
        std::unique_lock<std::mutex> lock(pause_mtx);
        pause = false;
        render_cv.notify_one();
    }

}

/*
void Renderer::loadModel() {

	objl::Loader loader;
	loader.LoadFile("spot.obj");
	//loader.
	triangleList.clear();
	for (auto& mesh : loader.LoadedMeshes) {
		for (int i = 0; i < mesh.Vertices.size(); i += 3)
		{
			Triangle t;
			for (int j = 0; j < 3; j++)
			{
				t.vertices[j][0] = mesh.Vertices[i + j].Position.X;
				t.vertices[j][1] = mesh.Vertices[i + j].Position.Y;
				t.vertices[j][2] = mesh.Vertices[i + j].Position.Z;
				t.vertices[j][3] = 1;

				memcpy(&t.normals[j], &mesh.Vertices[i + j].Normal, 3 * sizeof(float));
				memcpy(&t.tex_coords[j], &mesh.Vertices[i + j].TextureCoordinate, 2 * sizeof(float));
			}
			triangleList.push_back(t);
		}
	}
}
*/