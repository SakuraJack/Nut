#pragma once

#include "KeyCode.h"

namespace Nut {

	struct KeyData
	{
		KeyCode Key;
		KeyState State = KeyState::None;
		KeyState LastState = KeyState::None;
	};

	struct ButtomData
	{
		MouseButton Button;
		KeyState State = KeyState::None;
		KeyState LastState = KeyState::None;
	};

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyHeld(KeyCode key);
		static bool IsKeyReleased(KeyCode key);

		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonHeld(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

		static void TransitionPressedKeys();
		static void TransitionPressedButtons();
		static void UpdateKeyState(KeyCode key, KeyState newState);
		static void UpdateButtonState(MouseButton button, KeyState newState);
		static void ClearReleasedKeys();

	private:
		inline static std::unordered_map<KeyCode, KeyData> s_KeyData;
		inline static std::unordered_map<MouseButton, ButtomData> s_MouseData;
	};
}
