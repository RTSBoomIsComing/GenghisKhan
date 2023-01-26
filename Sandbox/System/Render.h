#pragma once


class Render
{
	Render() = delete;
	~Render() = delete;
public:
	static void Clear() noexcept;
	static void Triangle(entt::registry& reg) noexcept;

};