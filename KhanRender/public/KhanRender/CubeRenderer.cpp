#include "pch.h"
#include "CubeRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::CubeRenderer::CubeRenderer(std::shared_ptr<RenderingHub> core)
	:
	Renderer(core)
{
	m_vertexBuffer = KhanDx::CreateVertexBuffer(m_core->GetDevice(), vertices, sizeof(vertices));
	m_indexBuffer  = KhanDx::CreateIndexBuffer(m_core->GetDevice(), indices, sizeof(indices));
	m_pixelShader = KhanDx::CreatePixelShader(m_core->GetDevice(), "PS_BasicGeometry3d.cso");

	std::tie(m_vertexShader, m_inputLayout) = KhanDx::CreateVertexShaderAndInputLayout(
		m_core->GetDevice(), "VS_BasicGeometry3d.cso", elementDescs, ARRAYSIZE(elementDescs));

	m_VSDynamicCBuffer = KhanDx::CreateDynamicCBuffer<DirectX::XMFLOAT4X4, 2u>(m_core->GetDevice());
	//m_rsstate = KhanDx::CreateRSState_WireFrame(m_core->GetDevice());
	m_rsstate = KhanDx::CreateRSState_Solid(m_core->GetDevice());
	m_dsstate = KhanDx::CreateDSState_Default(m_core->GetDevice());
	m_blendState = nullptr; // blend off
}

void KhanRender::CubeRenderer::Render(UINT instanceCount)
{
	using namespace DirectX;


	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_core->GetContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &Stride, &offset);
	m_core->GetContext()->IASetInputLayout(m_inputLayout.Get());
	m_core->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_core->GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	m_core->GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0u);

	m_core->GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0u);

	XMFLOAT4X4 WorldViewProjMatrix[2]{};
	//XMMatrixRotationZ
	//XMMatrixRotationX
	//XMMatrixRotationY
	//XMMatrixTranslation
	static float angle_temp{-5.0F};
	angle_temp += 0.02F;
	if (angle_temp > 5.0F) angle_temp = -5.0F;

	float aspect_ratio = (float)m_core->GetScreenWidth() / m_core->GetScreenHeight();
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

	m_core->GetContext()->Map(m_VSDynamicCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &WorldViewProjMatrix, sizeof(WorldViewProjMatrix));
	m_core->GetContext()->Unmap(m_VSDynamicCBuffer.Get(), 0u);

	m_core->GetContext()->VSSetConstantBuffers(0u, 1u, m_VSDynamicCBuffer.GetAddressOf());

	m_core->GetContext()->RSSetState(m_rsstate.Get());
	m_core->GetContext()->OMSetDepthStencilState(m_dsstate.Get(), 1U);
	m_core->GetContext()->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
	m_core->GetContext()->DrawIndexedInstanced(ARRAYSIZE(indices), 2U, 0U, 0U, 0U);
}
