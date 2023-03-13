#pragma once
#include "Renderer.h"
#include <DirectXMath.h>
namespace KhanRender
{
	class LaserRenderer : public Renderer
	{
	public:
		LaserRenderer(const Renderer& renderer);
		void Render();
		void Update(DirectX::XMMATRIX const& ViewProjMat);
		ComPtr<ID3D11Buffer>		    m_pVertexBuffer;
		ComPtr<ID3D11InputLayout>	    m_pInputLayout;
		ComPtr<ID3D11VertexShader>	    m_pVertexShader;
		ComPtr<ID3D11RasterizerState>   m_pRasterizerState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11BlendState>	    m_pBlendState;
		ComPtr<ID3D11PixelShader>		m_pPixelShader;
		ComPtr<ID3D11Buffer>			m_pVSDynConstBuf;

	private:
		struct Vertex
		{
			DirectX::XMFLOAT3 RayStart{};
			DirectX::XMFLOAT3 RayEnd{};
		};

		static constexpr D3D11_INPUT_ELEMENT_DESC elementDescs[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		static constexpr UINT VERTEX_STRIDE = sizeof(DirectX::XMFLOAT3);
		static constexpr UINT VERTEX_OFFSET{};
		
		std::vector<Vertex> m_Vertices;

	public:
		std::vector<Vertex>& GetVertices() { return m_Vertices; };
	};
}

