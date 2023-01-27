#pragma once


class Render
{
	Render() = delete;
	~Render() = delete;
public:
	static void Clear() noexcept;
	static void Triangle(entt::registry& reg) noexcept;
	static void SelectionRect(float pos_x = -0.75f, float pos_y = 0.75f, float width = 1.5f, float height = 1.5f) noexcept;
};