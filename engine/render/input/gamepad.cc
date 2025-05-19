//------------------------------------------------------------------------------
//  @file gamepad.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "gamepad.h"

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
            axis[i] = tmp_axis[i];
        }

        int buttons_c;
        const unsigned char* tmp_btns = glfwGetJoystickButtons(id, &buttons_c);
        for (int i = 0; i < buttons_c && i < pressed.size(); ++i) {
            if (pressed[i] && tmp_btns[i]) {
                held[i] = true;
            }
            else {
                held[i] = false;
            }

            if (tmp_btns[i]) {
                pressed[i] = true;
                released[i] = false;
            }
            else {
                pressed[i] = false;
                released[i] = true;
            }
        }
    }

} // namespace Input
