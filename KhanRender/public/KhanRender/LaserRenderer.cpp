#include "pch.h"
#include "LaserRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::LaserRenderer::LaserRenderer(const Renderer& renderer)
	:
	Renderer(renderer)
{
	m_pVertexBuffer = KhanDx::CreateDynVertexBuffer(m_pDevice.Get(), nullptr, sizeof(Vertex) * 100/* the max number of ray*/);

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_Laser");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pBlendState = KhanDx::CreateBlendState_Alpha(m_pDevice.Get());
	m_pRasterizerState = KhanDx::CreateRasterizerState_WireFrame(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "PS_Laser");
	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(DirectX::XMFLOAT4X4), 1);
}

void KhanRender::LaserRenderer::Update(DirectX::XMMATRIX const& viewProjMat)
{
	using namespace DirectX;

	// Update pixel shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pVSDynConstBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, &viewProjMat, sizeof(viewProjMat));
	m_pDeviceContext->Unmap(m_pVSDynConstBuf.Get(), 0);

	mappedResource = {};
	m_pDeviceContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
	m_pDeviceContext->Unmap(m_pVertexBuffer.Get(), 0);
}


void KhanRender::LaserRenderer::Render()
{
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &VERTEX_STRIDE, &VERTEX_OFFSET);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pVSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->Draw(static_cast<UINT>(m_Vertices.size()) * 2, 0); // one vertex has two positions, rayStart and rayEnd
}



