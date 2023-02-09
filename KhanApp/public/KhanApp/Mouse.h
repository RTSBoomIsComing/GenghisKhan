#pragma once
#include "Delegate.h"
#include <bitset>

namespace KhanApp
{
	class Mouse
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		// Return types of funcions bound to Delegate are void
		Delegate<int, int> OnLeftButtonDown{};
		Delegate<int, int> OnLeftButtonUp{};

		Delegate<int, int> OnRightButtonDown{};
		Delegate<int, int> OnRightButtonUp{};

		Delegate<int, int> OnMouseMove{};

	private:
		enum class EventType
		{
			LEFT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_UP, MIDDLE_DOWN, MIDDLE_UP, MOVE, MAX
		};
		POINTS Positions[static_cast<int>(EventType::MAX)]{};

		enum class ButtonType
		{
			LEFT, RIGHT, MIDDLE, MAX
		};
		std::bitset<static_cast<int>(ButtonType::MAX)> ButtonStates{};



	public:
		POINTS GetLastMovePosition() const noexcept { return Positions[static_cast<int>(EventType::MOVE)]; }
		POINTS GetLastLeftDownPosition() const noexcept { return Positions[static_cast<int>(EventType::LEFT_DOWN)]; }
		POINTS GetLastLeftUpPosition() const noexcept { return Positions[static_cast<int>(EventType::LEFT_UP)]; }
		POINTS GetLastRightDownPosition() const noexcept { return Positions[static_cast<int>(EventType::RIGHT_DOWN)]; }
		POINTS GetLastRightUpPosition() const noexcept { return Positions[static_cast<int>(EventType::RIGHT_UP)]; }
		POINTS GetLastMiddleDownPosition() const noexcept { return Positions[static_cast<int>(EventType::MIDDLE_DOWN)]; }
		POINTS GetLastMiddleUpPosition() const noexcept { return Positions[static_cast<int>(EventType::MIDDLE_UP)]; }

		bool IsLeftButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::LEFT)]; }
		bool IsRightButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::RIGHT)]; }
		bool IsMiddleButtonDown() const noexcept { return ButtonStates[static_cast<int>(ButtonType::MIDDLE)]; }
	};
}
