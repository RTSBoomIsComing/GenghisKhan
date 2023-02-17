#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>

// standard libraries
#include <vector>
#include <filesystem>

// additional dependencies
//#include <stb_image.h>

namespace KhanRender
{


	class SkeletalMeshRenderer : public Renderer
	{
		struct MeshInfo
		{
			UINT NumVertices{};
			UINT NumIndices{};
			UINT BaseVertexLocation{};
			UINT StartIndexLocation{};
			ComPtr<ID3D11ShaderResourceView> m_pSRV;
		};
	public:
		SkeletalMeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath);
		void Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat, float debugScalar);
		void Render();
	private: // about rendering infomations
		UINT m_numInstance{};
		std::vector<MeshInfo> m_meshInfos;

		std::vector<ID3D11Buffer*> m_VBuf_Ptrs;
		std::vector<uint32_t> m_VBuf_Strides;
		std::vector<uint32_t> m_VBuf_Offsets;

		std::vector<ID3D11Buffer*> m_CBuf_VS_Ptrs;


		std::vector<DirectX::XMFLOAT4X4> m_bones;
		std::vector<DirectX::XMMATRIX> m_boneOffsets;
		//std::vector<DirectX::XMFLOAT4X4> m_nodeTransforms;

	private: // about directx 11 components
		ComPtr<ID3D11Buffer>			m_pVBuf_Positions;
		ComPtr<ID3D11Buffer>			m_pVBuf_TexCoords;
		ComPtr<ID3D11Buffer>			m_pVBuf_Normals;
		ComPtr<ID3D11Buffer>			m_pVBuf_BlendIndices;
		ComPtr<ID3D11Buffer>			m_pVBuf_BlendWeight;

		ComPtr<ID3D11Buffer>		    m_pCBuf_VS_Worlds;
		ComPtr<ID3D11Buffer>		    m_pCBuf_VS_ViewProjection;
		ComPtr<ID3D11Buffer>		    m_pCBuf_VS_Bones;

		ComPtr<ID3D11Buffer>		    m_pIndexBuffer;
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
			{ "POSITION",     0U, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0U, DXGI_FORMAT_R32G32B32_FLOAT,    1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",       0U, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0U, DXGI_FORMAT_R32G32B32A32_UINT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",  0U, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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
