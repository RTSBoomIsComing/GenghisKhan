#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>
#include <vector>

namespace KhanRender
{
	struct Model
	{
		UINT NumVertices{};
		UINT NumIndices{};
		UINT BaseVertexLocation{};
		UINT StartIndexLocation{};
	};

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(const Renderer& renderer);
		void Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat);
		void Render();
	private: // about rendering infomations
		UINT m_numInstance{};
		std::vector<Model> m_models;
		std::vector<ID3D11Buffer*>	m_vtxBufPtrs;
		std::vector<UINT>			m_vtxStrides;
		std::vector<UINT>			m_vtxBufOffsets;

		//UINT m_numIndices{};
		//UINT m_numVertices{};
		
	private: // about directx 11 components
		ComPtr<ID3D11Buffer> m_pVtxBufPosition;
		ComPtr<ID3D11Buffer> m_pVtxBufTexCoord;
		ComPtr<ID3D11Buffer> m_pVtxBufNormal;

		ComPtr<ID3D11Buffer>		    m_pIndexBuffer;
		ComPtr<ID3D11PixelShader>	    m_pPixelShader;
		ComPtr<ID3D11VertexShader>	    m_pVertexShader;
		ComPtr<ID3D11InputLayout>	    m_pInputLayout;
		//ComPtr<ID3D11Buffer>		    m_pPSDynCBuf;
		ComPtr<ID3D11Buffer>		    m_pVSDynConstBuf;
		//ComPtr<ID3D11Buffer>		    m_pVSDynStructBuf;
		ComPtr<ID3D11BlendState>	    m_pBlendState;
		ComPtr<ID3D11RasterizerState>   m_pRasterizerState;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		//ComPtr<ID3D11ShaderResourceView> m_pSRV;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_elementDescs;

		//struct Vertex
		//{
		//	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		//		:
		//		pos(x, y, z), tex(u, v), normal(nx, ny, nz) {}

		//	DirectX::XMFLOAT3 pos;
		//	DirectX::XMFLOAT2 tex;
		//	DirectX::XMFLOAT3 normal;
		//};
	};
}