#pragma once
#include "Delegate.h"
#include <bitset>

enum class MouseEvent
{
	LEFT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_UP, MIDDLE_DOWN, MIDDLE_UP, MOVE, MAX
};

namespace KhanApp
{
	class Mouse
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		friend class InputManager;
	public:
		// Return types of funcions bound to Delegate are void
		Delegate<int, int> OnLeftButtonDown{};
		Delegate<int, int> OnLeftButtonUp{};

		Delegate<int, int> OnRightButtonDown{};
		Delegate<int, int> OnRightButtonUp{};

		Delegate<int, int> OnMouseMove{};
		Delegate<int, int> OnMouseRawInput{};

		Delegate<int, int, int> OnMouseWheel{};

	private:
		POINTS Positions[static_cast<int>(MouseEvent::MAX)]{};

		enum class ButtonType
		{
			LEFT, RIGHT, MIDDLE, MAX
		};
		std::bitset<static_cast<int>(ButtonType::MAX)> ButtonStates{};

	public:
		template<MouseEvent T>
		POINT GetPosition() noexcept 
		{
			POINTS pt = Positions[static_cast<int>(T)];
			return { pt.x, pt.y };
		}

		bool IsLeftButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::LEFT)]; }
		bool IsRightButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::RIGHT)]; }
		bool IsMiddleButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::MIDDLE)]; }
	};
}
