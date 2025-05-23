#pragma once
//------------------------------------------------------------------------------
/**
	Manages the opening and closing of a window.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <nanovg.h>

namespace Display
{
class Window
{
public:
	/// constructor
	Window();
	/// destructor
	~Window();

	/// set size of window
	void SetSize(int32 width, int32 height);
	/// get size of windows
	void GetSize(int32 & width, int32 & height);
	/// set title of window
	void SetTitle(const std::string& title);

	/// open window
	bool Open();
	/// close window
	void Close();
	/// returns true if window is open
	const bool IsOpen() const;

	/// make this window current, meaning all draws will direct to this window context
	void MakeCurrent();

	/// update a tick
	void Update();
	/// swap buffers at end of frame
	void SwapBuffers();

	/// set key press function callback
	void SetKeyPressFunction(const std::function<void(int32, int32, int32, int32)>& func);
	/// set mouse press function callback
	void SetMousePressFunction(const std::function<void(int32, int32, int32)>& func);
	/// set mouse move function callback
	void SetMouseMoveFunction(const std::function<void(double, double)>& func);
	/// set mouse enter leave function callback
	void SetMouseEnterLeaveFunction(const std::function<void(bool)>& func);
	/// set mouse scroll function callback
	void SetMouseScrollFunction(const std::function<void(double, double)>& func);
	void SetJoystickFunction(const std::function<void(int, int)>& func);
    /// set window resize function callback
    void SetWindowResizeFunction(const std::function<void(int32, int32)>& func);

	/// set optional UI render function
	void SetUiRender(const std::function<void()>& func);
	void SetNanoFunc(const std::function<void(NVGcontext*)>& func);
	
	/// get pos of mouse
	void GetMousePos(float64& x, float64& y);

private:

	/// static key press callback
	static void StaticKeyPressCallback(GLFWwindow* win, int32 key, int32 scancode, int32 action, int32 mods);
	/// static mouse press callback
	static void StaticMousePressCallback(GLFWwindow* win, int32 button, int32 action, int32 mods);
	/// static mouse move callback
	static void StaticMouseMoveCallback(GLFWwindow* win, float64 x, float64 y);
	/// static mouse enter/leave callback
	static void StaticMouseEnterLeaveCallback(GLFWwindow* win, int32 mode);
	/// static mouse scroll callback
	static void StaticMouseScrollCallback(GLFWwindow* win, float64 x, float64 y);
    /// static resize window callback
    static void StaticWindowResizeCallback(GLFWwindow* win, int32 x, int32 y);
	
	static void StaticCloseCallback(GLFWwindow* window);
	static void StaticFocusCallback(GLFWwindow* window, int focus);
	static void StaticCharCallback(GLFWwindow* window, unsigned int key);
	static void StaticDropCallback(GLFWwindow* window, int files, const char** args);

	/// resize update
	void Resize();
	/// title rename update
	void Retitle(); 

	static int32 WindowCount;

	/// function for key press callbacks
	std::function<void(int32, int32, int32, int32)> keyPressCallback;
	/// function for mouse press callbacks
	std::function<void(int32, int32, int32)> mousePressCallback;
	/// function for mouse move callbacks
	std::function<void(double, double)> mouseMoveCallback;
	/// function for mouse enter/leave callbacks
	std::function<void(bool)> mouseLeaveEnterCallback;
	/// function for mouse scroll callbacks
	std::function<void(double, double)> mouseScrollCallback;
	std::function<void(int, int)> joystickCallback;
    /// function for window resize callbacks
    std::function<void(int32, int32)> windowResizeCallback;
	/// callback for ui rendering callback
	std::function<void()> uiFunc;
	/// callback for nano vg rendering
	std::function<void(NVGcontext*)> nanoFunc;
	
	int32 width;
	int32 height;
	std::string title;
	GLFWwindow* window;
	NVGcontext* vg;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetSize(int32 width, int32 height)
{
	this->width = width;
	this->height = height;
	if (nullptr != this->window) this->Resize();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::GetSize(int32 & width, int32 & height)
{
	width = this->width;
	height = this->height;

}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetTitle(const std::string& title)
{
	this->title = title;
	if (nullptr != this->window) this->Retitle();
}

//------------------------------------------------------------------------------
/**
*/
inline const bool
Window::IsOpen() const
{
	return nullptr != this->window;
}

//------------------------------------------------------------------------------
/**
	parameters:
		keycode
		scancode
		action
		mods
*/
inline void
Window::SetKeyPressFunction(const std::function<void(int32, int32, int32, int32)>& func)
{
	this->keyPressCallback = func;
}

//------------------------------------------------------------------------------
/**
	parameters:
		button code
		pressed (bool)
		scancode?
*/
inline void
Window::SetMousePressFunction(const std::function<void(int32, int32, int32)>& func)
{
	this->mousePressCallback = func;
}

//------------------------------------------------------------------------------
/**
	parameters: x, y position
*/
inline void
Window::SetMouseMoveFunction(const std::function<void(double, double)>& func)
{
	this->mouseMoveCallback = func;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetMouseEnterLeaveFunction(const std::function<void(bool)>& func)
{
	this->mouseLeaveEnterCallback = func;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetMouseScrollFunction(const std::function<void(double, double)>& func)
{
	this->mouseScrollCallback = func;
}

inline void Window::SetJoystickFunction(const std::function<void(int, int)>& func) {
	this->joystickCallback = func;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetWindowResizeFunction(const std::function<void(int32, int32)>& func)
{
    this->windowResizeCallback = func;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetUiRender(const std::function<void()>& func)
{
	this->uiFunc = func;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Window::SetNanoFunc(const std::function<void(NVGcontext*)>& func)
{
	this->nanoFunc = func;
}

} // namespace Display