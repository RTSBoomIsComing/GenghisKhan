#include "pch.h"
#include "Render.h"

#include <Khan/DxCore.h>
#include <Khan/Dx/RSState_WireFrame.h>
#include <Khan/Dx/DxFunction.h>
#include <Khan/Dx/DxUtility.h>

void Render::Clear() noexcept
{
	float bg_color[]{ 0.f, 1.f, 1.f, 1.f };

	dx_context->OMSetRenderTargets(1u, dx_rtv.GetAddressOf(), dx_dsv.Get());
	dx_context->ClearRenderTargetView(dx_rtv.Get(), bg_color);
	dx_context->ClearDepthStencilView(dx_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Render::Triangle(entt::registry& reg) noexcept
{
	using namespace DirectX;
	Khan::rsstate_wireframe->Bind();


	struct Vertex
	{
		XMFLOAT3 pos;
	};
	static Vertex vertices[]
	{
		XMFLOAT3{ -1.f, -1.f,  0.f },
		XMFLOAT3{  1.f,  1.f,  0.f },
		XMFLOAT3{  1.f, -1.f,  0.f },
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

	static ComPtr<ID3D11PixelShader> pixelShader = Khan::CreatePixelShader("PixelShader.cso");
	dx_context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	ComPtr<ID3DBlob> vertexShader_blob;
	Khan::ThrowIfFailed(::D3DReadFileToBlob(
		L"..\\bin\\Debug-x64\\Sandbox\\VertexShader.cso", &vertexShader_blob),
		"failed to read file, VertexShader.cso");

	ComPtr<ID3D11VertexShader> vertexShader;
	Khan::ThrowIfFailed(dx_device->CreateVertexShader(
		vertexShader_blob->GetBufferPointer(), vertexShader_blob->GetBufferSize(),
		nullptr, &vertexShader),
		"failed to create vertex shader");

	dx_context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	ComPtr<ID3D11InputLayout> inputLayout;

	const D3D11_INPUT_ELEMENT_DESC elementDescs[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Khan::ThrowIfFailed(
		dx_device->CreateInputLayout(
			elementDescs, ARRAYSIZE(elementDescs),
			vertexShader_blob->GetBufferPointer(), vertexShader_blob->GetBufferSize(), &inputLayout),
		"failed to create input layout");

	dx_context->IASetInputLayout(inputLayout.Get());
	dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dx_context->DrawIndexed(ARRAYSIZE(indices), 0u, 0u);
}
