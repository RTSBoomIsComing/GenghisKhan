#include "pch.h"
#include "LaserRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::LaserRenderer::LaserRenderer(const NearPlaneRenderer& renderer)
	:
	NearPlaneRenderer(renderer) 
{
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "PS_Laser");
	m_pPSDynConstBuf = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(DirectX::XMFLOAT4X4), 1);
}

void KhanRender::LaserRenderer::Render()
{
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &VERTEX_STRIDE, &VERTEX_OFFSET);
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

void KhanRender::LaserRenderer::Update(DirectX::XMMATRIX const& inverseViewProjMat)
{
	using namespace DirectX;

	XMMATRIX inverseViewProjMat_transposed = XMMatrixTranspose(inverseViewProjMat);
	// Update pixel shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pPSDynConstBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, &inverseViewProjMat_transposed, sizeof(inverseViewProjMat_transposed));
	m_pDeviceContext->Unmap(m_pPSDynConstBuf.Get(), 0);
}


