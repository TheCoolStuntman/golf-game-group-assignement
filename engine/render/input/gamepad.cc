//------------------------------------------------------------------------------
//  @file gamepad.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "gamepad.h"

#include "core/math.h"

#include <GLFW/glfw3.h>

namespace Input {
    
    Gamepad::Gamepad(int id) : id(id) {
        int axis_c;
        const float* tmp_axis = glfwGetJoystickAxes(id, &axis_c);
        axis.resize(axis_c);

        int buttons_c;
        const unsigned char* tmp_btns = glfwGetJoystickButtons(id, &buttons_c);
        pressed.resize(buttons_c);
        held.resize(buttons_c);
        released.resize(buttons_c);
    }

    Gamepad::~Gamepad() {
    }

    void Gamepad::Update() {
        int axis_c;
        const float* tmp_axis = glfwGetJoystickAxes(id, &axis_c);
        for (int i = 0; i < axis_c && i < axis.size(); ++i) {
            axis[i] = Math::outside(tmp_axis[i], DeadZone);
        }

        for (int i = 0; i < pressed.size(); ++i) {
            if (released[i]) {
                held[i] = false;
            }
            pressed[i] = false;
            released[i] = false;
        }

        int buttons_c;
        const unsigned char* tmp_btns = glfwGetJoystickButtons(id, &buttons_c);
        for (int i = 0; i < buttons_c && i < pressed.size(); ++i) {
            if (tmp_btns[i]) {
                pressed[i] = held[i] ? false : true;
                held[i] = true;
            }
            else {
                released[i] = held[i] ? true : false;
            }
        }
    }

} // namespace Input
