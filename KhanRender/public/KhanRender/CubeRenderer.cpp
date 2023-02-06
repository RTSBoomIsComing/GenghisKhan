#include "pch.h"
#include "CubeRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::CubeRenderer::CubeRenderer(std::shared_ptr<RenderingHub> pHub)
	:
	Renderer(pHub)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice, vertices, sizeof(vertices));
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice, indices, sizeof(indices));
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice, "PS_BasicGeometry3d");

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_BasicGeometry3d");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice, pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice, pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice, 10U);
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice);
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice);

}

void KhanRender::CubeRenderer::Update(std::vector<DirectX::XMFLOAT4X4> const& transforms)
{
	using namespace DirectX;

	m_numInstance = (UINT)transforms.size();
	std::vector <XMFLOAT4X4> WVPMatrices(m_numInstance);
	float aspect_ratio = (float)m_screenWidth / m_screenHeight;
	for (UINT i{}; i < m_numInstance; ++i)
	{
		XMStoreFloat4x4(&WVPMatrices[i],
			XMMatrixTranspose(
				XMLoadFloat4x4(&transforms[i])
				// Now these are hard coded. But later LookAt and PerspectiveFov matrices will be obtained by camera entity.
				* XMMatrixLookToLH({ 0.0F, 0.0F, -10.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F, 0.0F })
				* XMMatrixPerspectiveFovLH(3.14F / 4.F, aspect_ratio, 1.0F, 100.0F)));
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pVSDynConstBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource);
	::memcpy(mappedResource.pData, WVPMatrices.data(), sizeof(XMFLOAT4X4) * m_numInstance);
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
	m_pDeviceContext->DrawIndexedInstanced(ARRAYSIZE(indices), m_numInstance, 0U, 0U, 0U);
}
