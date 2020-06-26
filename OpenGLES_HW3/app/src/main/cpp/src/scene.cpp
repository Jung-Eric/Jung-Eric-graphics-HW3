#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    //player
    //캐릭터 기본 설정
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));


    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
    Scene::lineDraw->worldMat = scale(vec3(1.0f / 3.0f));
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();

    Scene::camera->update();

    const float playTime = 5.0;
    static float time = 0.0f;
    time += deltaTime;
    if (time >= playTime) time -= playTime;

    int prevFrame = (int)floor(time / playTime * motions.size());
    int nextFrame = (int)ceil(time / playTime * motions.size());
    if(nextFrame >= motions.size()) nextFrame = 0;
    float ratio = time / playTime * motions.size() - (float)prevFrame;

    // from bone space to world space
    //연산된 값들을 넣어준다.
    vector<mat4x4> bone2world = {mat4x4(1.0f)};


    // from world space to bone space
    //연산된 값들을 넣어준다.
    vector<mat4x4> world2bone = {mat4x4(1.0f)};

    // 이 부분은 키 interpolate 부분(애니메이션 부드럽게)
    /*
    for(int jointIdx = 1; jointIdx < jNames.size(); jointIdx ++) {
        //Motion interpolation using an Euler angle or quaternion.
        // Please use prevFrame, nextFrame, ratio, R, bone2world, and world2bone.

        //float x =
        //float y = ;
        //float z = ;

        mat4x4 R = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


        // With motion
        //bone2world;
        //world2bone;
    }



    //vertex에 따라서 skinning 해주기기
    */
    //et new vertex
    vector<Vertex> newVertex;
    for(int vertexIdx = 0; vertexIdx <= playerVertices.size(); vertexIdx++){
        vec3 result;
        Vertex imp;
        for(int boneIter = 0; boneIter < 4; boneIter ++) {

        }

        imp.pos = playerVertices[vertexIdx].pos;
        imp.nor = playerVertices[vertexIdx].nor;
        imp.bone =  playerVertices[vertexIdx].bone;
        imp.tex = playerVertices[vertexIdx].tex;
        imp.weight = playerVertices[vertexIdx].weight;

        newVertex.push_back(imp);
    }

    //연산된 걸 가져와서 적용해 준다.
    Scene::player->load(newVertex, playerIndices);
    Scene::player->draw();

    //If you want to see the skeleton as a line..
    //스켈레톤 그려진 것 확인하기
    //우선 이미지를 적용한다
    //glLineWidth(20);
    //Scene::lineDraw->load(?, ?);
    //Scene::lineDraw->draw();
}