//------------------------------------------------------------------------------
//  @file inputserver.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "inputserver.h"
#include "GLFW/glfw3.h"

namespace Input
{

struct HIDState
{
	Keyboard keyboard;
	Mouse mouse;
	Gamepad* current_gamepad;
	std::vector<Gamepad*> gamepads;
};

static HIDState* hid = nullptr;

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::Create()
{
	if (hid == nullptr) {
		hid = new HIDState();
	}
}

void InputHandler::Destroy() {
	if (hid != nullptr) {
		while (hid->gamepads.size() > 0) {
			delete hid->gamepads.back();
			hid->gamepads.pop_back();
		}
		delete hid;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::BeginFrame()
{
	for (int i = 0; i < Key::Code::NumKeyCodes; i++)
	{
		if (hid->keyboard.released[i])
			hid->keyboard.held[i] = false;
		
		hid->keyboard.pressed[i] = false;
		hid->keyboard.released[i] = false;
	}

	for (int i = 0; i < Mouse::Button::NumMouseButtons; i++)
	{
		if (hid->mouse.released[i])
			hid->mouse.held[i] = false;

		hid->mouse.pressed[i] = false;
		hid->mouse.released[i] = false;
	}

	hid->mouse.delta = glm::vec2(0);
	hid->mouse.previousPosition = hid->mouse.position;
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleKeyEvent(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (action == GLFW_PRESS)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.pressed[code] = true;
		hid->keyboard.held[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.pressed[Key::Code::Control] = true;
			hid->keyboard.held[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.pressed[Key::Code::Shift] = true;
			hid->keyboard.held[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.pressed[Key::Code::Alt] = true;
			hid->keyboard.held[Key::Code::Alt] = true;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.released[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.released[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.released[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.released[Key::Code::Alt] = true;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMousePressEvent(int32 button, int32 action, int32 mods)
{
	assert(button < Mouse::Button::NumMouseButtons);
	if (action == GLFW_PRESS)
	{
		hid->mouse.pressed[button] = true;
		hid->mouse.held[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		hid->mouse.released[button] = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMouseMoveEvent(double x, double y)
{
	hid->mouse.position = glm::vec2(x, y);
	hid->mouse.delta = hid->mouse.position - hid->mouse.previousPosition;
}

void InputHandler::HandleJoystickEvent(int jid, int e) {
	if (e == GLFW_CONNECTED) {
		CreateGamepad(jid);
	}
	else if (e == GLFW_DISCONNECTED) {
		DestroyGamepad(jid);
	}
}

//------------------------------------------------------------------------------
/**
*/
Keyboard*
GetDefaultKeyboard()
{
	assert(hid != nullptr);
	return &hid->keyboard;
}

//------------------------------------------------------------------------------
/**
*/
Mouse*
GetDefaultMouse()
{
	assert(hid != nullptr);
	return &hid->mouse;
}

Gamepad* GetCurrentGamepad()
{
	assert(hid != nullptr);
	return hid->current_gamepad;
}

//------------------------------------------------------------------------------
/**
*/
Gamepad*
GetGamepad(int id)
{
	assert(hid != nullptr);
	assert(id > 0 && id < hid->gamepads.size());
	return hid->gamepads[id];
}

int CreateGamepad(int id) {
	assert(hid != nullptr);

	hid->gamepads.emplace_back(new Gamepad(id));
	hid->current_gamepad = hid->gamepads.back();
	return hid->gamepads.size();
}

void DestroyGamepad(int id) {
	assert(hid != nullptr);
	assert(id >= 0 && id < hid->gamepads.size());

	if (hid->gamepads.size() == 1) {
		delete hid->gamepads.back();
		hid->gamepads.pop_back();
		hid->current_gamepad = nullptr;
		return;
	}

	for (int i = 0; i < hid->gamepads.size(); ++i) {
		if (hid->gamepads[i]->id == id) {
			std::swap(hid->gamepads[i], hid->gamepads.back());
			delete hid->gamepads.back();
			hid->gamepads.pop_back();
			hid->current_gamepad = hid->gamepads.back();
		}
	}
}

} // namespace Input
