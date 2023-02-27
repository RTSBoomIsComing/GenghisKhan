#pragma once
#include "KhanRender/Renderer.h"
#include "KhanAnim/SkeletalAnimation.h"
#include <DirectXMath.h>

// standard libraries
#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace KhanRender
{
	struct AnimationInfo
	{
		unsigned int AnimationStartOffset;
		unsigned int Durations;
		float		 TicksPerSecond;
	};

	class SkeletalMeshRenderer : public Renderer
	{
		struct MeshInfo
		{
			uint32_t  NumVertices{};
			uint32_t  NumIndices{};
			uint32_t  BaseVertexLocation{};
			uint32_t  StartIndexLocation{};
			ComPtr<ID3D11ShaderResourceView> m_pSRV;
		};

	public:
		SkeletalMeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath);
		void Update(size_t numInstances, DirectX::XMMATRIX const* worldMats, const uint32_t* AnimationId, const float* runningTime, DirectX::XMMATRIX const& viewProjMat);
		void Render();
	private: // about vertex buffer
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

		std::array<unsigned int, NUM_VERTEX_ELEMENTS>  m_VBuf_Strides
		{
			12, 12, 12, 16, 16
		};
		std::array<unsigned int, NUM_VERTEX_ELEMENTS>			m_VBuf_Offsets;
		std::array<ID3D11Buffer*, NUM_VERTEX_ELEMENTS>			m_VBuf_Ptrs;
		std::array<ComPtr<ID3D11Buffer>, NUM_VERTEX_ELEMENTS>	m_pVBufs;

	private: // about rendering infomations
		static constexpr unsigned int MAX_NUM_BONES{ 100 }; // I think maybe the number of bones are not more than 100
		static constexpr unsigned int MAX_NUM_BONE_WEIGHTS{ 4 };
		
		uint32_t m_NumInstances{};
		unsigned int  m_TotalNumBones{};
		std::vector<MeshInfo> m_MeshInfos;
		std::vector<AnimationInfo> m_AnimationInfos;
		std::vector<DirectX::XMMATRIX> m_FinalBoneTransforms;

	private: // about constant buffer
		ComPtr<ID3D11Buffer> m_pCBuf_VS_Worlds;
		ComPtr<ID3D11Buffer> m_pCBuf_VS_ViewProjection;
		ComPtr<ID3D11Buffer> m_pCBuf_VS_Blending;
		std::array<ID3D11Buffer*, 3> m_CBuf_VS_Ptrs;

	private: // about directx 11 components
		ComPtr<ID3D11Buffer>			m_pIndexBuffer;
		ComPtr<ID3D11PixelShader>	    m_pPixelShader;
		ComPtr<ID3D11VertexShader>	    m_pVertexShader;
		ComPtr<ID3D11InputLayout>	    m_pInputLayout;
		ComPtr<ID3D11BlendState>	    m_pBlendState;
		ComPtr<ID3D11RasterizerState>   m_pRasterizerState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11SamplerState>		m_pSamplerState;

		//ComPtr<ID3D11Buffer>		    m_pVSDynStructBuf;
		//ComPtr<ID3D11Buffer>		    m_pPSDynCBuf;
		ComPtr<ID3D11ShaderResourceView> m_pSRV_VS_FinalBoneTransforms;
	};
}
