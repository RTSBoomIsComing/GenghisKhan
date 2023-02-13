#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>

// standard libraries
#include <vector>
#include <filesystem>

// additional dependencies
#include <stb_image.h>

namespace KhanRender
{
	struct MeshInfo
	{
		UINT NumVertices{};
		UINT NumIndices{};
		UINT BaseVertexLocation{};
		UINT StartIndexLocation{};
		ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath = "..\\Mixamo\\Standing Idle 01.fbx");
		void Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat);
		void Render();
	private: // about rendering infomations
		UINT m_numInstance{};
		std::vector<MeshInfo> m_meshInfos;
		
	private: // about directx 11 components
		ComPtr<ID3D11Buffer>			m_pVertexBuffer;
		ComPtr<ID3D11Buffer>		    m_pIndexBuffer;
		ComPtr<ID3D11Buffer>		    m_pVSDynConstBuf;
		ComPtr<ID3D11PixelShader>	    m_pPixelShader;
		ComPtr<ID3D11VertexShader>	    m_pVertexShader;
		ComPtr<ID3D11InputLayout>	    m_pInputLayout;
		ComPtr<ID3D11BlendState>	    m_pBlendState;
		ComPtr<ID3D11RasterizerState>   m_pRasterizerState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11SamplerState>		m_pSamplerState;

		//ComPtr<ID3D11Buffer>		    m_pVSDynStructBuf;
		//ComPtr<ID3D11Buffer>		    m_pPSDynCBuf;
		//ComPtr<ID3D11ShaderResourceView> m_pSRV;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_elementDescs
		{
			{ "POSITION", 0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, 0U, D3D11_INPUT_PER_VERTEX_DATA, 0U },
			{ "TEXCOORD", 0U, DXGI_FORMAT_R32G32_FLOAT,    0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },
			{ "NORMAL",   0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },
		};

		struct Vertex
		{
			constexpr Vertex(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 t, DirectX::XMFLOAT3 n) noexcept
				:
				pos(p), tex(t.x, t.y), normal(n) {}

			constexpr Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) noexcept
				:
				pos(x, y, z), tex(u, v), normal(nx, ny, nz) {}

			DirectX::XMFLOAT3 pos{};
			DirectX::XMFLOAT2 tex{};
			DirectX::XMFLOAT3 normal{};
		};
	};
}