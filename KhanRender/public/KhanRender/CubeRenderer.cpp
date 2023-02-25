#include "pch.h"
#include "CubeRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::CubeRenderer::CubeRenderer(const Renderer& renderer)
	:
	Renderer(renderer)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice.Get(), vertices, sizeof(vertices));
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice.Get(), indices, sizeof(indices));
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "PS_BasicGeometry3d");

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_BasicGeometry3d");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice.Get(), 1000U);
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());

}

void KhanRender::CubeRenderer::Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat)
{
	using namespace DirectX;

	m_NumInstances = (UINT)worldMats.size();
	std::vector <XMFLOAT4X4> WVPMatrices(m_NumInstances);

	for (UINT i{}; i < m_NumInstances; ++i)
	{
		XMStoreFloat4x4(&WVPMatrices[i], XMMatrixTranspose(worldMats[i] * viewProjMat));
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pVSDynConstBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource);
	::memcpy(mappedResource.pData, WVPMatrices.data(), sizeof(XMFLOAT4X4) * m_NumInstances);
	m_pDeviceContext->Unmap(m_pVSDynConstBuf.Get(), 0U);
}

void KhanRender::CubeRenderer::Render()
{
	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_pDeviceContext->IASetVertexBuffers(0U, 1U, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0U);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0U);

	//m_pDeviceContext->VSSetShaderResources(0U, 1U, m_pSRV.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(0U, 1U, m_pVSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1U);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->DrawIndexedInstanced(ARRAYSIZE(indices), m_NumInstances, 0U, 0U, 0U);
}
