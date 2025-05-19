#include "config.h"
#include "spaceship.h"
#include "render/input/inputserver.h"
#include "render/cameramanager.h"
#include "render/physics.h"
#include "render/debugrender.h"
#include "core/math.h"

using namespace Input;
using namespace glm;
using namespace Render;

namespace Game
{
    SpaceShip::SpaceShip()
    {
    }

    void
    SpaceShip::Update(float dt)
    {
        Mouse* mouse = Input::GetDefaultMouse();
        Keyboard* kbd = Input::GetDefaultKeyboard();
        Gamepad* gpd = Input::GetCurrentGamepad();
        Camera* cam = CameraManager::GetCamera(CAMERA_MAIN);

        float rotX{ 0.0f }, rotY{ 0.0f }, rotZ{ 0.0f };
        if (auto cgp = Input::GetCurrentGamepad(); cgp == nullptr) {
            rotX = kbd->held[Key::Left] ? 1.0f : kbd->held[Key::Right] ? -1.0f : 0.0f;
            rotY = kbd->held[Key::Up] ? -1.0f : kbd->held[Key::Down] ? 1.0f : 0.0f;
            rotZ = kbd->held[Key::A] ? -1.0f : kbd->held[Key::D] ? 1.0f : 0.0f;
        }
        else {
            rotX = -gpd->axis[Input::GamepadAxis::RIGHT_STICK_X];
            rotY = -gpd->axis[Input::GamepadAxis::RIGHT_STICK_Y];
            rotZ = gpd->axis[Input::GamepadAxis::LEFT_STICK_X];
        }

        this->position += this->linearVelocity * dt * 10.0f;

        const float rotationSpeed = 1.8f * dt;
        rotXSmooth = mix(rotXSmooth, rotX * rotationSpeed, dt * cameraSmoothFactor);
        rotYSmooth = mix(rotYSmooth, rotY * rotationSpeed, dt * cameraSmoothFactor);
        rotZSmooth = mix(rotZSmooth, rotZ * rotationSpeed, dt * cameraSmoothFactor);
        quat localOrientation = quat(vec3(-rotYSmooth, rotXSmooth, rotZSmooth));
        this->orientation = this->orientation * localOrientation;
        this->rotationZ -= rotXSmooth;
        this->rotationZ = clamp(this->rotationZ, -45.0f, 45.0f);
        mat4 T = translate(this->position) * (mat4)this->orientation;
        this->transform = T * (mat4)quat(vec3(0, 0, rotationZ));
        this->rotationZ = mix(this->rotationZ, 0.0f, dt * cameraSmoothFactor);

        // update camera view transform
        vec3 desiredCamPos = this->position + vec3(this->transform * vec4(0, camOffsetY, -4.0f, 0));
        this->camPos = mix(this->camPos, desiredCamPos, dt * cameraSmoothFactor);
        cam->view = lookAt(this->camPos, this->camPos + vec3(this->transform[2]), vec3(this->transform[1]));
    }

    bool
    SpaceShip::CheckCollisions()
    {
        glm::mat4 rotation = (glm::mat4)orientation;
        bool hit = false;
        for (int i = 0; i < 8; i++)
        {
            glm::vec3 pos = position;
            glm::vec3 dir = rotation * glm::vec4(glm::normalize(colliderEndPoints[i]), 0.0f);
            float len = glm::length(colliderEndPoints[i]);
            Physics::RaycastPayload payload = Physics::Raycast(position, dir, len);

            // debug draw collision rays
            // Debug::DrawLine(pos, pos + dir * len, 1.0f, glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 1), Debug::RenderMode::AlwaysOnTop);

            if (payload.hit)
            {
                Debug::DrawDebugText("HIT", payload.hitPoint, glm::vec4(1, 1, 1, 1));
                hit = true;
            }
        }
        return hit;
    }
}