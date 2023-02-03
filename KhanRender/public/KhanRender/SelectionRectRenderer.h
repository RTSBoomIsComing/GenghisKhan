#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

namespace KhanRender
{
	class SelectionRectRenderer : public Renderer
	{		
	public:
		SelectionRectRenderer(std::shared_ptr<RenderingHub> core);;
		void Render(int x1, int y1, int x2, int y2);

	private:
		ComPtr<ID3D11RasterizerState> m_rsstate;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11PixelShader> m_pixelShader;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_PSDynamicCBuffer;
		ComPtr<ID3D11Buffer> m_VSDynamicCBuffer;
	private:
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};

		const Vertex vertices[4]
		{
			DirectX::XMFLOAT3{  0.0f, -1.0f,  0.0f },	DirectX::XMFLOAT2{ 0.0f, 1.0f },
			DirectX::XMFLOAT3{  0.0f,  0.0f,  0.0f },	DirectX::XMFLOAT2{ 0.0f, 0.0f },
			DirectX::XMFLOAT3{  1.0f,  0.0f,  0.0f },	DirectX::XMFLOAT2{ 1.0f, 0.0f },
			DirectX::XMFLOAT3{  1.0f, -1.0f,  0.0f },	DirectX::XMFLOAT2{ 1.0f, 1.0f },
		};

		const UINT indices[6]
		{
			0, 1, 2,
			0, 2, 3,
		};

		const D3D11_INPUT_ELEMENT_DESC elementDescs[2]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};
}


