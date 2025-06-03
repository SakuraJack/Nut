#include "ntpch.h"
#include "Input.h"

#include "Window.h"
#include "Application.h"

bool Nut::Input::IsKeyPressed(KeyCode key)
{
	return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Pressed;
}

bool Nut::Input::IsKeyHeld(KeyCode key)
{
	return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Held;
}

bool Nut::Input::IsKeyReleased(KeyCode key)
{
	return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Released;
}

bool Nut::Input::IsMouseButtonPressed(MouseButton button)
{
	return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Pressed;
}

bool Nut::Input::IsMouseButtonHeld(MouseButton button)
{
	return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Held;
}

bool Nut::Input::IsMouseButtonReleased(MouseButton button)
{
	return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Released;
}

float Nut::Input::GetMouseX()
{
	auto [x, y] = GetMousePosition();
	return x;
}

float Nut::Input::GetMouseY()
{
	auto [x, y] = GetMousePosition();
	return y;
}

std::pair<float, float> Nut::Input::GetMousePosition()
{
	auto window = Application::Get().GetWindow();
	double x, y;
	glfwGetCursorPos(static_cast<GLFWwindow*>(window->GetNativeWindow()), &x, &y);
	return { static_cast<float>(x), static_cast<float>(y) };
}

void Nut::Input::TransitionPressedKeys()
{
	for (const auto& [key, data] : s_KeyData)
	{
		if (data.State == KeyState::Pressed)
			UpdateKeyState(key, KeyState::Held);
	}
}

void Nut::Input::TransitionPressedButtons()
{
	for (const auto& [button, data] : s_MouseData)
	{
		if (data.State == KeyState::Pressed)
			UpdateButtonState(button, KeyState::Held);
	}
}

void Nut::Input::UpdateKeyState(KeyCode key, KeyState newState)
{
	auto& keyData = s_KeyData[key];
	keyData.Key = key;
	keyData.LastState = keyData.State;
	keyData.State = newState;
}

void Nut::Input::UpdateButtonState(MouseButton button, KeyState newState)
{
	auto& buttonData = s_MouseData[button];
	buttonData.Button = button;
	buttonData.LastState = buttonData.State;
	buttonData.State = newState;
}

void Nut::Input::ClearReleasedKeys()
{
	for (const auto& [key, data] : s_KeyData) {
		if (data.State == KeyState::Released) {
			UpdateKeyState(key, KeyState::None);
		}
	}

	for (const auto& [button, data] : s_MouseData) {
		if (data.State == KeyState::Released) {
			UpdateButtonState(button, KeyState::None);
		}
	}
}
