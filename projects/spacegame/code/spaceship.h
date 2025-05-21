#pragma once
#include "render/model.h"

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
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 offset = glm::vec3(0);
    glm::vec3 camPos = glm::vec3(0, 0.0f, 0.5f);
    glm::vec3 linearVelocity = glm::vec3(0);

    const float maxSpeed = 2.0f;
    const float minDist = 1.0f;
    const float maxDist = 3.0f;
    const float cameraSmoothFactor = 10.0f;

    float currentSpeed = 0.0f;

    float dist = minDist;

    Render::ModelId model;

    void Update(float dt);

    bool CheckCollisions();
    
    const glm::vec3 colliderEndPoints[8] = {
        glm::vec3(-1.10657, -0.480347, -0.346542),  // right wing
        glm::vec3(1.10657, -0.480347, -0.346542),  // left wing
        glm::vec3(-0.342382, 0.25109, -0.010299),   // right top
        glm::vec3(0.342382, 0.25109, -0.010299),   // left top
        glm::vec3(-0.285614, -0.10917, 0.869609), // right front
        glm::vec3(0.285614, -0.10917, 0.869609), // left front
        glm::vec3(-0.279064, -0.10917, -0.98846),   // right back
        glm::vec3(0.279064, -0.10917, -0.98846)   // right back
    };
};

}