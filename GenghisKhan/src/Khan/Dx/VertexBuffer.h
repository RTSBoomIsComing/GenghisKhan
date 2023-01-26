#pragma once
#include <d3d11.h>
#include <vector>
namespace Khan
{
	template<typename T>
	class VertexBuffer
	{
	public:
		VertexBuffer(std::vector<T> vertices) noexcept;

	};

	template<typename T>
	VertexBuffer<T>::VertexBuffer(std::vector<T> vertices) noexcept
	{

	}
}


