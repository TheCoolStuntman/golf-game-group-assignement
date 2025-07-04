#pragma once
#include "render/model.h"
#include "core/level.h"

namespace Render
{
    struct ParticleEmitter;
}

namespace Game
{

struct SpaceShip
{
    SpaceShip();
    
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 position = glm::vec3(0);
    glm::vec3 offset = glm::vec3(0);
    glm::vec3 camPos = glm::vec3(0, 0.0f, 0.5f);
    glm::vec3 camRot = glm::vec3(0.0f);
    glm::vec3 linearVelocity = glm::vec3(0);

    const float minDist = 1.0f;
    const float maxDist = 3.0f;
    const float cameraSmoothFactor = 10.0f;
    const float maxShootPower = 2.0f;
    const float shootPowerGrowSpeed = 10.0f;
    
    float shootPower = 0.0f;
    float dist = minDist;

    int strokes = 0;

    Render::ModelId model;

    void Update(float dt);

    bool CheckCollisions(const Level::Level& level, Physics::ColliderId flagColliderId);

    void ResetPos(const Level::Level& level);
    
    const std::vector<glm::vec3> colliderEndPoints = {
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)* glm::rotate(glm::radians(-150.0f), glm::vec3(0, 1, 0)))),
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)* glm::rotate(glm::radians(-120.0f), glm::vec3(0, 1, 0)))),
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)))),   // -90
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(-60.0f), glm::vec3(0, 1, 0)))),  // -60
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(-30.0f), glm::vec3(0, 1, 0)))),   // -30
        glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)),   // 0
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(30.0f), glm::vec3(0, 1, 0)))),    // 30
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(60.0f), glm::vec3(0, 1, 0)))),    // 60
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)))),   // 90
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)* glm::rotate(glm::radians(120.0f), glm::vec3(0, 1, 0)))),
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)* glm::rotate(glm::radians(150.0f), glm::vec3(0, 1, 0)))),
        glm::normalize(glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)* glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)))),
    };
};

}