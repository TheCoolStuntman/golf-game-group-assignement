#pragma once
//------------------------------------------------------------------------------
/**
    @file

    @copyright
    (C) 2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vector>

namespace Input {

    struct GamepadButton {
        enum Code {
            A = 0,
            B,
            X,
            Y,
            LEFT_BUMPER,
            RIGHT_BUMPER,
            BACK,
            START,
            L3,
            R3,
            DPAD_UP,
            DPAD_RIGHT,
            DPAD_DOWN,
            DPAD_LEFT,
            NUMGamepadButton,
        };
    };

    struct GamepadAxis {
        enum Code {
            LEFT_STICK_X = 0,
            LEFT_STICK_Y,
            RIGHT_STICK_X,
            RIGHT_STICK_Y,
            LEFT_TRIGGER,
            RIGHT_TRIGGER,
            NUMGamepadAxis,
        };
    };

    struct Gamepad {
        int id;
        std::vector<float> axis;
        std::vector<bool> pressed;
        std::vector<bool> held;
        std::vector<bool> released;

        Gamepad(int id);
        ~Gamepad();

        void Update();
    };

} // namespace Input
