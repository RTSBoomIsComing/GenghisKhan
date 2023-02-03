#pragma once
#include "KhanRender/Renderer.h"
#include <DirectXMath.h>

namespace KhanRender
{
	class CubeRenderer : public Renderer
	{
	public:
		CubeRenderer(std::shared_ptr<RenderingHub> core);
		void Render();

	private:
		ComPtr<ID3D11Buffer>		    m_vertexBuffer;
		ComPtr<ID3D11Buffer>		    m_indexBuffer;
		ComPtr<ID3D11PixelShader>	    m_pixelShader;
		ComPtr<ID3D11VertexShader>	    m_vertexShader;
		ComPtr<ID3D11InputLayout>	    m_inputLayout;
		ComPtr<ID3D11Buffer>		    m_PSDynamicCBuffer;
		ComPtr<ID3D11Buffer>		    m_VSDynamicCBuffer;
		ComPtr<ID3D11RasterizerState>   m_rsstate;
		ComPtr<ID3D11DepthStencilState> m_dsstate;
		ComPtr<ID3D11BlendState>	    m_blendState;

		struct Vertex
		{
			Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
				:
				pos(x, y, z), tex(u, v), normal(nx, ny, nz){}

			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
			DirectX::XMFLOAT3 normal;
		};

		const Vertex vertices[24]
		{
			// Front Face
			{-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f, 1.0f, 0.0f,  1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,  1.0f, -1.0f, -1.0f},

			// Back Face										 
			{-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, -1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  1.0f,  1.0f},

			// Top Face											 
			{-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, -1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  1.0f,  1.0f, -1.0f},

			// Bottom Face										
			{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f,  1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f, -1.0f,  1.0f},

			// Left Face										 
			{-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, -1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, -1.0f,  1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f},

			// Right Face										 
			{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f,  1.0f, -1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f,  1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  1.0f, -1.0f,  1.0f},
		};

		const UINT indices[36]
		{
			// Front Face
			0,  1,  2,
			0,  2,  3,

			// Back Face
			4,  5,  6,
			4,  6,  7,

			// Top Face
			8,  9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		const D3D11_INPUT_ELEMENT_DESC elementDescs[3]
		{
			{ "POSITION", 0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, 0U,                            D3D11_INPUT_PER_VERTEX_DATA, 0U },
			{ "TEXCOORD", 0U, DXGI_FORMAT_R32G32_FLOAT,    0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },
			{ "NORMAL",   0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },
		};
	};
}