#pragma once
#include <d3d11.h>
#include <vector>
namespace Khan
{
	class VertexShader
	{
	public:
		VertexShader(std::string_view fileName, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept;
		
		// IASetInputLayout() then VSSetShader()
		void Bind() noexcept;
	private:
		ComPtr<ID3D11VertexShader> m_shader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
	};


}


