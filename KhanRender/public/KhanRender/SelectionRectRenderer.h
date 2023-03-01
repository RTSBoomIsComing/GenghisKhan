#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>

namespace KhanRender
{
	class SelectionRectRenderer : public Renderer
	{
	public:
		SelectionRectRenderer(const Renderer& renderer);
		void Render();
		void Update(const RECT& rect, UINT m_screenWidth, UINT m_screenHeight);

	private:
		ComPtr<ID3D11Buffer>		    m_pVertexBuffer;
		ComPtr<ID3D11PixelShader>	    m_pPixelShader;
		ComPtr<ID3D11VertexShader>	    m_pVertexShader;
		ComPtr<ID3D11InputLayout>	    m_pInputLayout;
		ComPtr<ID3D11Buffer>		    m_pVSDynConstBuf;
		ComPtr<ID3D11RasterizerState>   m_pRasterizerState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11BlendState>	    m_pBlendState;
	private:
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};

		const Vertex vertices[4]
		{
			DirectX::XMFLOAT3{  0.0f,  0.0f,  0.0f },	DirectX::XMFLOAT2{ 0.0f, 0.0f },
			DirectX::XMFLOAT3{  1.0f,  0.0f,  0.0f },	DirectX::XMFLOAT2{ 1.0f, 0.0f },
			DirectX::XMFLOAT3{  0.0f, -1.0f,  0.0f },	DirectX::XMFLOAT2{ 0.0f, 1.0f },
			DirectX::XMFLOAT3{  1.0f, -1.0f,  0.0f },	DirectX::XMFLOAT2{ 1.0f, 1.0f },
		};

		const D3D11_INPUT_ELEMENT_DESC elementDescs[2]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};
}


