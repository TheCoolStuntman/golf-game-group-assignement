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
                this->linearVelocity = shootPower * glm::vec4(0, 0, -1.0f, 0.0f) * glm::rotate(-camRot.y, glm::vec3(0, 1, 0));
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
                this->linearVelocity = shootPower * glm::vec4(0, 0, -1.0f, 0.0f) * glm::rotate(-camRot.y, glm::vec3(0, 1, 0));
                shootPower = 0.0f;
            }
        }
        dist = Math::clamp(dist, minDist, maxDist);

        this->position += this->linearVelocity * dt;
        this->linearVelocity *= 0.98f;
        Debug::DrawLine(position, position + linearVelocity, 0.2f, glm::vec4(1, 1, 0, 1), glm::vec4(1, 1, 0, 1));

        const float rotationSpeed = 1.8f;
        camRot = {
            camRot.x + rotX * rotationSpeed * dt,
            camRot.y + rotY * rotationSpeed * dt,
            0
        };

        this->transform = glm::translate(this->position) * glm::rotate(camRot.x, glm::vec3(-1.0f, 0, 0)) * glm::rotate(camRot.y, glm::vec3(0, 1.0f, 0));

        offset = {
            dist * cosf(camRot.x) * sinf(camRot.y),
            dist * sinf(camRot.x),
            dist * cosf(camRot.x)* cosf(camRot.y)
        };

        this->camPos = this->position + offset;

        // update camera view transform
        cam->view = lookAt(this->camPos, this->position, glm::vec3(0, 1, 0));
    }

    bool
    SpaceShip::CheckCollisions(const Level::Level& level)
    {
        Physics::RaycastPayload closest;
        closest.hitDistance = FLT_MAX;

        for (const auto& it : colliderEndPoints)
        {
            float len = glm::length(it);  
            const glm::vec3& dir = it;
            const glm::vec4 col = { 0.0f, 1.0f, 1.0f, 1.0f };
            Debug::DrawLine(position, position + dir * len, 0.2f, col, col);

            Physics::RaycastPayload payload = Physics::Raycast(level, position, dir, len);

            if (payload.hit && payload.hitDistance < 0.04f)
            {
                //if (glm::dot(payload.hitNormal, linearVelocity) < 0.0f) payload.hitNormal *= -1.0f;
                
                Debug::DrawLine(payload.hitPoint, payload.hitPoint + payload.hitNormal, 2.0f, glm::vec4(1, 0, 1, 1), glm::vec4(1, 0, 1, 1));
                
                if (payload.hitDistance < closest.hitDistance) {
                    closest = std::move(payload);
                }
            }
        }

        if (closest.hit) {
            linearVelocity = glm::reflect(linearVelocity, closest.hitNormal) * 0.8f;
            linearVelocity.y = 0;
            position += closest.hitNormal * 0.02f;
            return true;
        }
        return false;
    }
}