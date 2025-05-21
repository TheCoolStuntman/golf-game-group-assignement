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
            dist += (kbd->held[Key::A] ? 1.0f : kbd->held[Key::D] ? -1.0f : 0.0f) * dt;
            if (kbd->held[Key::Space]) {
                shootPower = Math::max(shootPower + shootPowerGrowSpeed * dt, maxShootPower);
            }
            if (kbd->released[Key::Space]) {
                this->linearVelocity = shootPower * glm::vec4(0, 0, -1.0f, 0.0f) * glm::rotate(-rotation.y, glm::vec3(0, 1, 0));
                shootPower = 0.0f;
            }
        }
        else {
            rotX = -gpd->axis[Input::GamepadAxis::RIGHT_STICK_Y];
            rotY = gpd->axis[Input::GamepadAxis::RIGHT_STICK_X];
            dist += gpd->axis[Input::GamepadAxis::LEFT_STICK_Y] * dt;
            if (gpd->held[Input::GamepadButton::A]) {
                shootPower = Math::max(shootPower + shootPowerGrowSpeed * dt, maxShootPower);
            }
            if (gpd->released[Input::GamepadButton::A]) {
                this->linearVelocity = shootPower * glm::vec4(0, 0, -1.0f, 0.0f) * glm::rotate(-rotation.y, glm::vec3(0, 1, 0));
                shootPower = 0.0f;
            }
        }
        dist = Math::clamp(dist, minDist, maxDist);

        this->position += this->linearVelocity * dt;
        this->linearVelocity *= 0.98f;

        const float rotationSpeed = 1.8f;
        rotation = {
            rotation.x + rotX * rotationSpeed * dt,
            rotation.y + rotY * rotationSpeed * dt,
            0
        };

        this->transform = glm::translate(this->position) * glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0));

        offset = {
            dist * cosf(rotation.x) * sinf(rotation.y),
            dist * sinf(rotation.x),
            dist * cosf(rotation.x)* cosf(rotation.y)
        };

        this->camPos = this->position + offset;

        // update camera view transform
        cam->view = lookAt(this->camPos, this->position, glm::vec3(0, 1, 0));
    }

    bool
    SpaceShip::CheckCollisions()
    {
        bool hit = false;
        for (int i = 0; i < 8; i++)
        {
            glm::vec3 pos = position;
            glm::vec3 dir = rotation * glm::normalize(colliderEndPoints[i]);
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