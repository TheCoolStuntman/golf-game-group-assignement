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
            rotX = kbd->held[Key::Up] ? 1.0f : kbd->held[Key::Down] ? -1.0f : 0.0f;
            rotY = kbd->held[Key::Left] ? 1.0f : kbd->held[Key::Right] ? -1.0f : 0.0f;
            offsetZ += (kbd->held[Key::A] ? 1.0f : kbd->held[Key::D] ? -1.0f : 0.0f) * dt;
        }
        else {
            rotX = -gpd->axis[Input::GamepadAxis::RIGHT_STICK_Y];
            rotY = gpd->axis[Input::GamepadAxis::RIGHT_STICK_X];
            offsetZ += gpd->axis[Input::GamepadAxis::LEFT_STICK_Y] * dt;
        }
        offsetZ = Math::clamp(offsetZ, minOffsetZ, maxOffsetZ);

        this->position += this->linearVelocity * dt * 10.0f;

        const float rotationSpeed = 1.8f;
        rot.x = rotX * rotationSpeed * dt;
        rot.y = rotY * rotationSpeed * dt;

        this->transform = this->transform * glm::translate(this->position) * glm::rotate(rot.x, glm::vec3(1, 0, 0)) * glm::rotate(rot.y, glm::vec3(0, 1, 0));

        // update camera view transform
        vec3 desiredCamPos = this->position + vec3(this->transform * vec4(0, 0.0f, -offsetZ, 0));
        this->camPos = mix(this->camPos, desiredCamPos, dt * cameraSmoothFactor);
        cam->view = lookAt(this->camPos, this->camPos + vec3(this->transform[2]), vec3(this->transform[1]));
    }

    bool
    SpaceShip::CheckCollisions()
    {
        bool hit = false;
        for (int i = 0; i < 8; i++)
        {
            glm::vec3 pos = position;
            glm::vec3 dir = rot * glm::normalize(colliderEndPoints[i]);
            float len = glm::length(colliderEndPoints[i]);
            Physics::RaycastPayload payload = Physics::Raycast(position, dir, len);

            if (payload.hit)
            {
                hit = true;
            }
        }
        return hit;
    }
}