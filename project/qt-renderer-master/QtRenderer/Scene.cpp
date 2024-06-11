#include "Scene.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


int Scene::addModel(string path, string texp, string norp) {
    //Model tempm;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
    if (!scene) return 0;

    shared_ptr<Texture> sp;
    if (texp != "") sp = make_shared<Texture>(texp.c_str());
    shared_ptr<Texture> np;
    if (norp != "") np = make_shared<Texture>(norp.c_str());

    cout << scene->mNumMeshes;

    for (int i = 0; i < scene->mNumMeshes; i++) {

        //Model m;
        models.emplace_back();

        aiVector3D* v = scene->mMeshes[i]->mVertices;
        int numVertices = scene->mMeshes[i]->mNumVertices;

        aiVector3D* n = scene->mMeshes[i]->mNormals;

        aiVector3D* t = scene->mMeshes[i]->mTextureCoords[0];

        models.back().vertexBuffer.reserve(numVertices);
        if (t) {
            for (int j = 0; j < numVertices; j++) {
                models.back().vertexBuffer.emplace_back(
                    Eigen::Vector4f(v[j].x, v[j].y, v[j].z, 1.f),
                    Eigen::Vector3f(n[j].x, n[j].y, n[j].z),
                    Eigen::Vector2f(t[j].x, t[j].y)
                );
            }
        }
        else {
            for (int j = 0; j < numVertices; j++) {
                models.back().vertexBuffer.emplace_back(
                    Eigen::Vector4f(v[j].x, v[j].y, v[j].z, 1.f),
                    Eigen::Vector3f(n[j].x, n[j].y, n[j].z),
                    Eigen::Vector2f(0, 0)
                );
            }
        }


        models.back().indexBuffer.reserve(scene->mMeshes[i]->mNumFaces * 3);
        for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
            for (int k = 0; k < 3; k++) {
                unsigned int index = scene->mMeshes[i]->mFaces[j].mIndices[k];
                models.back().indexBuffer.push_back(index);
            }
        }
        if (sp && !sp->isEmpty()) {
            models.back().textures[0] = sp;
        }
        if (np && !np->isEmpty()) {
            models.back().textures[1] = np;
        }
        modelMatrices.push_back(Eigen::Matrix4f::Identity());
    }

    return scene->mNumMeshes;
}


/// <summary>
/// Axis should be a normalized vector.
/// </summary>
/// <param name="angle"></param>
/// <param name="axis"></param>
void Scene::rotateModel(int index, float angle, Eigen::Vector3f axis) {

    Eigen::Matrix4f rotate = Eigen::Matrix4f::Identity();
    //Rodrigues
    Eigen::Matrix3f A;
    A << 0, -axis[2], axis[1],
        axis[2], 0, -axis[0],
        -axis[1], axis[0], 0;

    float c = cos(angle / 180 * M_PI);
    rotate.block<3, 3>(0, 0) = c * Eigen::Matrix3f::Identity() + (1 - c) * axis * axis.transpose() + sin(angle / 180 * M_PI) * A;

    //modelMatrix = rotate * modelMatrix;
    //modelMatrix *= rotate;
    modelMatrices[index] *= rotate;
}
void Scene::translateModel(int index, Eigen::Vector3f vec) {
    Eigen::Matrix4f translate = Eigen::Matrix4f::Identity();
    translate(0, 3) = vec[0];
    translate(1, 3) = vec[1];
    translate(2, 3) = vec[2];
    //modelMatrix = translate * modelMatrix;
    //modelMatrix *= translate;
    modelMatrices[index] = translate * modelMatrices[index];
}
void Scene::scaleModel(int index, Eigen::Vector3f scale) {
    Eigen::Matrix4f translate = Eigen::Matrix4f::Identity();
    translate(0, 0) = scale[0];
    translate(1, 1) = scale[1];
    translate(2, 2) = scale[2];
    //modelMatrix = translate * modelMatrix;
    //modelMatrix *= translate;
    modelMatrices[index] *= translate;
}

/// <summary>
/// Set Camera with unit vectors of top and front and a vector of position.
/// </summary>
/// <param name="top">A normalized vector of top direction</param>
/// <param name="front">A normalized vector of front direction</param>
/// <param name="position">A vector of camera position</param>
void Scene::setCamera(Eigen::Vector3f top, Eigen::Vector3f front, Eigen::Vector3f position) {
    Eigen::Matrix4f rotate;
    Eigen::Matrix4f translate = Eigen::Matrix4f::Identity();

    translate(0, 3) = -position[0];
    translate(1, 3) = -position[1];
    translate(2, 3) = -position[2];

    Eigen::Vector3f temp = top.cross(front);
    rotate << temp[0], top[0], front[0], 0,
        temp[1], top[1], front[1], 0,
        temp[2], top[2], front[2], 0,
        0, 0, 0, 1;

    viewMatrix = rotate.transpose() * translate;
    mainCamera.position = position;
    mainCamera.top = top;
    mainCamera.front = front;
}

/// <summary>
/// </summary>
/// <param name="fov"></param>
/// <param name="aspect">height/width(because normal width/height is infinite decimal while height/width not)</param>
/// <param name="near">Positive value</param>
/// <param name="far">Positive value</param>
void Scene::setPerspective(float fov, float aspect, float near, float far) {
    float t = tan(fov / 360 * M_PI);
    projectionMatrix <<
        1 / t, 0, 0, 0,
        0, 1 / (t * aspect), 0, 0,
        0, 0, (near + far) / (far - near), 2 * near * far / (near - far),
        0, 0, 1, 0;
    mainCamera.far = far;
    mainCamera.near = near;
}
