#include "pch.h"
#include "GridFloorRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::GridFloorRenderer::GridFloorRenderer(const Renderer& renderer)
	:
	Renderer(renderer)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice.Get(), vertices, sizeof(vertices));

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "GridFloor_PS");
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("GridFloor_VS");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pPSDynConstBuf = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(DirectX::XMFLOAT4X4), 1);

	m_pBlendState = KhanDx::CreateBlendState_Alpha(m_pDevice.Get());
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
}

void KhanRender::GridFloorRenderer::Render()
{
	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pPSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->Draw(ARRAYSIZE(vertices), 0);
}

void KhanRender::GridFloorRenderer::Update(DirectX::XMMATRIX const& inverseViewProjMat)
{
	using namespace DirectX;

	XMMATRIX inverseViewProjMat_transposed = XMMatrixTranspose(inverseViewProjMat);
	// Update pixel shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pPSDynConstBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, &inverseViewProjMat_transposed, sizeof(inverseViewProjMat_transposed));
	m_pDeviceContext->Unmap(m_pPSDynConstBuf.Get(), 0);
}
