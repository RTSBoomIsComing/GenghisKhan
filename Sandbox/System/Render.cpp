#include "pch.h"
#include "Render.h"

#include <Khan/DxCore.h>
#include <Khan/Dx/RSState_WireFrame.h>
#include <Khan/Dx/DxFunction.h>
#include <Khan/Dx/DxUtility.h>

void Render::Clear() noexcept
{
	float bg_color[]{ 0.f, 0.f, 0.f, 1.f };

	dx_context->OMSetRenderTargets(1u, dx_rtv.GetAddressOf(), dx_dsv.Get());
	dx_context->ClearRenderTargetView(dx_rtv.Get(), bg_color);
	dx_context->ClearDepthStencilView(dx_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Render::Triangle(entt::registry& reg) noexcept
{
	using namespace DirectX;
	dx_context->RSSetState(dx_rsstate.Get());
	//Khan::rsstate_wireframe->Bind();

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
	static constexpr Vertex vertices[]
	{
		XMFLOAT3{ -1.f, -1.f,  0.1f }, XMFLOAT2{0.0f, 1.0f},
		XMFLOAT3{  0.f,  1.f,  0.1f }, XMFLOAT2{0.5f, 0.0f},
		XMFLOAT3{  1.f, -1.f,  0.1f }, XMFLOAT2{1.0f, 1.0f},
	};
	UINT Stride = sizeof(Vertex);
	UINT offset{};

	static ComPtr<ID3D11Buffer> vertexBuffer = Khan::CreateVertexBuffer(vertices, sizeof(vertices));
	dx_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &Stride, &offset);

	static constexpr UINT indices[]
	{
		0, 1, 2,
	};

	static ComPtr<ID3D11Buffer> indexBuffer = Khan::CreateIndexBuffer(indices, sizeof(indices));
	dx_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static ComPtr<ID3D11PixelShader> pixelShader = Khan::CreatePixelShader("PS_IndexedInstanced.cso");
	dx_context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	static const D3D11_INPUT_ELEMENT_DESC elementDescs[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static ComPtr<ID3DBlob> shaderBlob = Khan::CreateShaderBlob("VS_IndexedInstanced.cso");
	static ComPtr<ID3D11VertexShader> vertexShader = Khan::CreateVertexShader(shaderBlob.Get());
	static ComPtr<ID3D11InputLayout> inputLayout = Khan::CreateInputLayout(shaderBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));
	dx_context->IASetInputLayout(inputLayout.Get());
	dx_context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	dx_context->DrawIndexed(ARRAYSIZE(indices), 0u, 0u);
}

void Render::SelectionRect(int screen_width, int screen_height, int x1, int y1, int x2, int y2) noexcept
{
	if (x1 == x2 || y1 == y2)
	{
		return;
	}

	using namespace DirectX;
	dx_context->RSSetState(dx_rsstate.Get());
	//Khan::rsstate_wireframe->Bind();

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
	static constexpr Vertex vertices[]
	{
		XMFLOAT3{  0.0f, -1.0f,  0.0f },	XMFLOAT2{ 0.0f, 1.0f },
		XMFLOAT3{  0.0f,  0.0f,  0.0f },	XMFLOAT2{ 0.0f, 0.0f },
		XMFLOAT3{  1.0f,  0.0f,  0.0f },	XMFLOAT2{ 1.0f, 0.0f },
		XMFLOAT3{  1.0f, -1.0f,  0.0f },	XMFLOAT2{ 1.0f, 1.0f },
	};
	UINT Stride = sizeof(Vertex);
	UINT offset{};

	static ComPtr<ID3D11Buffer> vertexBuffer = Khan::CreateVertexBuffer(vertices, sizeof(vertices));
	dx_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &Stride, &offset);

	static constexpr UINT indices[]
	{
		0, 1, 2,
		0, 2, 3,
	};

	static ComPtr<ID3D11Buffer> indexBuffer = Khan::CreateIndexBuffer(indices, sizeof(indices));
	dx_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	static ComPtr<ID3D11PixelShader> pixelShader = Khan::CreatePixelShader("PixelShader.cso");
	dx_context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	static const D3D11_INPUT_ELEMENT_DESC elementDescs[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static ComPtr<ID3DBlob> shaderBlob = Khan::CreateShaderBlob("VertexShader.cso");
	static ComPtr<ID3D11VertexShader> vertexShader = Khan::CreateVertexShader(shaderBlob.Get());
	static ComPtr<ID3D11InputLayout> inputLayout = Khan::CreateInputLayout(shaderBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	dx_context->IASetInputLayout(inputLayout.Get());
	dx_context->VSSetShader(vertexShader.Get(), nullptr, 0u);


	if (x1 > x2)
	{
		std::swap(x1, x2);
	}

	if (y1 > y2)
	{
		std::swap(y1, y2);
	}

	float rect_w = static_cast<float>(x2 - x1);
	float rect_h = static_cast<float>(y2 - y1);

	XMFLOAT4 rectSize{ rect_w, rect_h, 0.f, 0.f };

	rect_w = rect_w * 2 / screen_width;
	rect_h = rect_h * 2 / screen_height;
	float pos_x = float(x1) * 2 / screen_width - 1.0f;
	float pos_y = 1.0f - float(y1) * 2 / screen_height;

	XMFLOAT4X4 rectTransform // lefthand -> transpose -> righthand
	{
		rect_w, 0.0f,   0.0f,  pos_x,
		0.0f,   rect_h, 0.0f,  pos_y,
		0.0f,   0.0f,   1.0f,  0.0f,
		0.0f,   0.0f,   0.0f,  1.0f,
	};

	static ComPtr<ID3D11Buffer> vsDynamicCBuffer = Khan::CreateDynamicCBuffer<XMFLOAT4X4, 1u>();
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	dx_context->Map(vsDynamicCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &rectTransform, sizeof(rectTransform));
	dx_context->Unmap(vsDynamicCBuffer.Get(), 0u);

	dx_context->VSSetConstantBuffers(0u, 1u, vsDynamicCBuffer.GetAddressOf());


	static ComPtr<ID3D11Buffer> psDynamicCBuffer = Khan::CreateDynamicCBuffer<XMFLOAT4, 1u>();
	D3D11_MAPPED_SUBRESOURCE mappedResource_ps{};

	dx_context->Map(psDynamicCBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource_ps);
	::memcpy(mappedResource_ps.pData, &rectSize, sizeof(rectSize));
	dx_context->Unmap(psDynamicCBuffer.Get(), 0u);

	dx_context->PSSetConstantBuffers(0u, 1u, psDynamicCBuffer.GetAddressOf());

	dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	ComPtr<ID3D11BlendState> blendState_alpha = Khan::CreateBlendState_Alpha_Static();
	dx_context->OMSetBlendState(blendState_alpha.Get(), nullptr, 0xffffffff);

	dx_context->DrawIndexed(ARRAYSIZE(indices), 0u, 0u);
}

void Render::Cube(entt::registry& reg) noexcept
{
	using namespace DirectX;
	dx_context->RSSetState(dx_rsstate.Get());
	Khan::rsstate_wireframe->Bind();
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 normal;
	};
	static constexpr Vertex vertices[]
	{
		// Front Face
		XMFLOAT3{-1.0f, -1.0f, -1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{-1.0f, -1.0f, -1.0f},
		XMFLOAT3{-1.0f,  1.0f, -1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{-1.0f,  1.0f, -1.0f},
		XMFLOAT3{ 1.0f,  1.0f, -1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 1.0f,  1.0f, -1.0f},
		XMFLOAT3{ 1.0f, -1.0f, -1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 1.0f, -1.0f, -1.0f},

		// Back Face										 
		XMFLOAT3{-1.0f, -1.0f,  1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{-1.0f, -1.0f,  1.0f},
		XMFLOAT3{ 1.0f, -1.0f,  1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 1.0f, -1.0f,  1.0f},
		XMFLOAT3{ 1.0f,  1.0f,  1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 1.0f,  1.0f,  1.0f},
		XMFLOAT3{-1.0f,  1.0f,  1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{-1.0f,  1.0f,  1.0f},

		// Top Face											 
		XMFLOAT3{-1.0f,  1.0f, -1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{-1.0f, 1.0f, -1.0f},
		XMFLOAT3{-1.0f,  1.0f,  1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{-1.0f, 1.0f,  1.0f},
		XMFLOAT3{ 1.0f,  1.0f,  1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 1.0f, 1.0f,  1.0f},
		XMFLOAT3{ 1.0f,  1.0f, -1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 1.0f, 1.0f, -1.0f},

		// Bottom Face										
		XMFLOAT3{-1.0f, -1.0f, -1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{-1.0f, -1.0f, -1.0f},
		XMFLOAT3{ 1.0f, -1.0f, -1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 1.0f, -1.0f, -1.0f},
		XMFLOAT3{ 1.0f, -1.0f,  1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 1.0f, -1.0f,  1.0f},
		XMFLOAT3{-1.0f, -1.0f,  1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{-1.0f, -1.0f,  1.0f},

		// Left Face										 
		XMFLOAT3{-1.0f, -1.0f,  1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{-1.0f, -1.0f,  1.0f},
		XMFLOAT3{-1.0f,  1.0f,  1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{-1.0f,  1.0f,  1.0f},
		XMFLOAT3{-1.0f,  1.0f, -1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{-1.0f,  1.0f, -1.0f},
		XMFLOAT3{-1.0f, -1.0f, -1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{-1.0f, -1.0f, -1.0f},

		// Right Face										 
		XMFLOAT3{1.0f, -1.0f, -1.0f},  XMFLOAT2{0.0f,  1.0f},XMFLOAT3{ 1.0f, -1.0f, -1.0f},
		XMFLOAT3{1.0f,  1.0f, -1.0f},  XMFLOAT2{0.0f,  0.0f},XMFLOAT3{ 1.0f,  1.0f, -1.0f},
		XMFLOAT3{1.0f,  1.0f,  1.0f},  XMFLOAT2{1.0f,  0.0f},XMFLOAT3{ 1.0f,  1.0f,  1.0f},
		XMFLOAT3{1.0f, -1.0f,  1.0f},  XMFLOAT2{1.0f,  1.0f},XMFLOAT3{ 1.0f, -1.0f,  1.0f},
	};
	UINT Stride = sizeof(Vertex);
	UINT offset{};

	static ComPtr<ID3D11Buffer> vertexBuffer = Khan::CreateVertexBuffer(vertices, sizeof(vertices));
	dx_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &Stride, &offset);
	dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static constexpr UINT indices[]
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

	static ComPtr<ID3D11Buffer> indexBuffer = Khan::CreateIndexBuffer(indices, sizeof(indices));
	dx_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);


	static ComPtr<ID3D11PixelShader> pixelShader = Khan::CreatePixelShader("PS_BasicGeometry3d.cso");
	dx_context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	static const D3D11_INPUT_ELEMENT_DESC elementDescs[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static ComPtr<ID3DBlob> shaderBlob = Khan::CreateShaderBlob("VS_BasicGeometry3d.cso");
	static ComPtr<ID3D11VertexShader> vertexShader = Khan::CreateVertexShader(shaderBlob.Get());
	static ComPtr<ID3D11InputLayout> inputLayout = Khan::CreateInputLayout(shaderBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	dx_context->IASetInputLayout(inputLayout.Get());
	dx_context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	XMFLOAT4X4 WorldViewProjMatrix{};
	//XMMatrixRotationZ
	//XMMatrixRotationX
	//XMMatrixRotationY
	//XMMatrixTranslation
	static float angle_temp{};
	angle_temp += 0.02f;
	if (angle_temp > 10.f) angle_temp = 0.0f;
	XMStoreFloat4x4(&WorldViewProjMatrix,
		XMMatrixTranspose(
			XMMatrixRotationY(angle_temp)
			* XMMatrixTranslation(angle_temp, 0.0f, 0.0f)
			* XMMatrixLookAtLH({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })
			* XMMatrixPerspectiveFovLH(3.14f / 4.f, 4.0f / 3.0f, 1.0f, 100.0f)));

	static ComPtr<ID3D11Buffer> vsDynamicCBuffer = Khan::CreateDynamicCBuffer<XMFLOAT4X4, 1u>();
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	dx_context->Map(vsDynamicCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &WorldViewProjMatrix, sizeof(WorldViewProjMatrix));
	dx_context->Unmap(vsDynamicCBuffer.Get(), 0u);

	dx_context->VSSetConstantBuffers(0u, 1u, vsDynamicCBuffer.GetAddressOf());

	ComPtr<ID3D11BlendState> blendState_alpha = Khan::CreateBlendState_Alpha_Static();
	dx_context->OMSetBlendState(blendState_alpha.Get(), nullptr, 0xffffffff);

	dx_context->DrawIndexed(ARRAYSIZE(indices), 0u, 0u);
}
