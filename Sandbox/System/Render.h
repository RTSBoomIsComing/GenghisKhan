#pragma once


class Render
{
	Render() = delete;
	~Render() = delete;
public:
	static void Clear() noexcept;
	static void Triangle(entt::registry& reg) noexcept;
	static void SelectionRect(int screen_width, int screen_height, int x1, int y1, int x2, int y2) noexcept;
};