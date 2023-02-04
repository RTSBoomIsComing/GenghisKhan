#include "pch.h"
#include "CubeRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::CubeRenderer::CubeRenderer(std::shared_ptr<RenderingHub> core)
	:
	Renderer(core)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice, vertices, sizeof(vertices));
	m_pIndexBuffer  = KhanDx::CreateIndexBuffer(m_pDevice, indices, sizeof(indices));
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice, "PS_BasicGeometry3d");

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_BasicGeometry3d");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice, pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice, pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, 2U);
	KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, 2U);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice);
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice);

}

void KhanRender::CubeRenderer::Render(UINT instanceCount)
{
	using namespace DirectX;


	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0U);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0U);

	XMFLOAT4X4 WorldViewProjMatrix[2]{};
	//XMMatrixRotationZ
	//XMMatrixRotationX
	//XMMatrixRotationY
	//XMMatrixTranslation
	static float angle_temp{-5.0F};
	angle_temp += 0.02F;
	if (angle_temp > 5.0F) angle_temp = -5.0F;

	float aspect_ratio = (float)m_screenWidth / m_screenHeight;
	XMStoreFloat4x4(&WorldViewProjMatrix[0],
		XMMatrixTranspose(
			XMMatrixRotationY(angle_temp)
			* XMMatrixTranslation(angle_temp, 0.0f, 0.0f)
			* XMMatrixLookAtLH({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })
			* XMMatrixPerspectiveFovLH(3.14f / 4.f, aspect_ratio, 1.0f, 100.0f)));

	XMStoreFloat4x4(&WorldViewProjMatrix[1],
		XMMatrixTranspose(
			XMMatrixRotationY(-angle_temp)
			* XMMatrixTranslation(-angle_temp, 0.0f, 0.0f)
			* XMMatrixLookAtLH({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })
			* XMMatrixPerspectiveFovLH(3.14f / 4.f, aspect_ratio, 1.0f, 100.0f)));

	
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	m_pDeviceContext->Map(m_pVSDynStructBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource);
	::memcpy(mappedResource.pData, &WorldViewProjMatrix, sizeof(WorldViewProjMatrix));
	m_pDeviceContext->Unmap(m_pVSDynStructBuf.Get(), 0U);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 2;
	
	
	m_pDevice->CreateShaderResourceView(m_pVSDynStructBuf.Get(), &srvDesc, &m_pSRV);

	m_pDeviceContext->VSSetShaderResources(0U, 1U, m_pSRV.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1U);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xffffffff);
	m_pDeviceContext->DrawIndexedInstanced(ARRAYSIZE(indices), 2U, 0U, 0U, 0U);
}
