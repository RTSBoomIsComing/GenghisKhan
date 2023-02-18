#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>

// standard libraries
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>

// additional dependencies
//#include <stb_image.h>

namespace KhanRender
{


	class SkeletalMeshRenderer : public Renderer
	{
		struct MeshInfo
		{
			uint32_t NumVertices{};
			uint32_t NumIndices{};
			uint32_t BaseVertexLocation{};
			uint32_t StartIndexLocation{};
			ComPtr<ID3D11ShaderResourceView> m_pSRV;
		};
		
	public:
		SkeletalMeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath);
		void Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat, float debugScalar);
		void Render();
	private: // about rendering infomations
		UINT m_numInstance{};
		std::vector<MeshInfo> m_meshInfos;

		enum class VertexElement
		{
			POSITION, TEXCOORD, NORMAL, BLENDINDICES, BLENDWEIGHT, MAX
		};
		static constexpr int NUM_VERTEX_ELEMENTS{ static_cast<int>(VertexElement::MAX) };

		D3D11_INPUT_ELEMENT_DESC m_elementDescs[NUM_VERTEX_ELEMENTS]
		{
			{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		static_cast<int>(VertexElement::POSITION),		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT,		static_cast<int>(VertexElement::TEXCOORD),		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		static_cast<int>(VertexElement::NORMAL),		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	static_cast<int>(VertexElement::BLENDINDICES),	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	static_cast<int>(VertexElement::BLENDWEIGHT),	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		uint32_t m_VBuf_Strides[NUM_VERTEX_ELEMENTS]
		{
			12, 12, 12, 16, 16
		};
		uint32_t m_VBuf_Offsets[NUM_VERTEX_ELEMENTS]{};
		ID3D11Buffer* m_VBuf_Ptrs[NUM_VERTEX_ELEMENTS]{};

		std::vector<ID3D11Buffer*> m_CBuf_VS_Ptrs;

		std::vector<DirectX::XMFLOAT4X4>	m_bones;
		std::vector<DirectX::XMMATRIX>		m_boneOffsets;
		std::unordered_map<std::string, uint32_t> m_NodeNameToBoneIndex;

		std::vector<std::unordered_map<std::string, std::vector<DirectX::XMMATRIX>>> m_AnimNodeTransforms;

	private: // about directx 11 components
		ComPtr<ID3D11Buffer> m_pVBuf_Positions;
		ComPtr<ID3D11Buffer> m_pVBuf_TexCoords;
		ComPtr<ID3D11Buffer> m_pVBuf_Normals;
		ComPtr<ID3D11Buffer> m_pVBuf_BlendIndices;
		ComPtr<ID3D11Buffer> m_pVBuf_BlendWeight;

		ComPtr<ID3D11Buffer> m_pCBuf_VS_Worlds;
		ComPtr<ID3D11Buffer> m_pCBuf_VS_ViewProjection;
		ComPtr<ID3D11Buffer> m_pCBuf_VS_Bones;

		ComPtr<ID3D11Buffer> m_pIndexBuffer;

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
	};
}
