#include "pch.h"
#include "SelectionRectRenderer.h"
#include "KhanDx/KhanDxComponents.h"


KhanRender::SelectionRectRenderer::SelectionRectRenderer(std::shared_ptr<RenderingHub> core)
	:
	Renderer(core)
{
	m_vertexBuffer = KhanDx::CreateVertexBuffer(m_core->GetDevice(), vertices, sizeof(vertices));
	//m_indexBuffer  = KhanDx::CreateIndexBuffer(m_core->GetDevice(), indices, sizeof(indices));
	m_pixelShader  = KhanDx::CreatePixelShader(m_core->GetDevice(), "PixelShader.cso");

	std::tie(m_vertexShader, m_inputLayout) = KhanDx::CreateVertexShaderAndInputLayout(
		m_core->GetDevice(), "VertexShader.cso", elementDescs, ARRAYSIZE(elementDescs));

	m_PSDynamicCBuffer = KhanDx::CreateDynamicCBuffer<DirectX::XMFLOAT4, 1U>(m_core->GetDevice());
	m_VSDynamicCBuffer = KhanDx::CreateDynamicCBuffer<DirectX::XMFLOAT4X4, 1U>(m_core->GetDevice());

	m_rsstate	   = KhanDx::CreateRSState_Solid(m_core->GetDevice());
	m_dsstate	   = KhanDx::CreateDSState_Default(m_core->GetDevice());
	m_blendState   = KhanDx::CreateBlendState_Alpha(m_core->GetDevice());
}

void KhanRender::SelectionRectRenderer::Render(int x1, int y1, int x2, int y2)
{
	using namespace DirectX;

	if (x1 == x2 || y1 == y2)
	{
		return;
	}

	// normalize the rectangle (x1 < x2, y1 < y2) 
	if (x1 > x2)
	{
		std::swap(x1, x2);
	}

	if (y1 > y2)
	{
		std::swap(y1, y2);
	}


	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_core->GetContext()->IASetVertexBuffers(0U, 1U, m_vertexBuffer.GetAddressOf(), &Stride, &offset);
	m_core->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_core->GetContext()->IASetInputLayout(m_inputLayout.Get());

	m_core->GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
	m_core->GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0u);

	float screen_w = static_cast<float>(m_core->GetScreenWidth());
	float screen_h = static_cast<float>(m_core->GetScreenHeight());

	// max value of (x2 - x1) is (screen width - 1), so adjust to have max value of screen width
	float rect_w = static_cast<float>(x2 - x1) * screen_w / (screen_w - 1);

	// max value of (y2 - y1) is (screen height - 1), so adjust to have max value of screen height
	float rect_h = static_cast<float>(y2 - y1) * screen_h / (screen_h - 1);

	XMFLOAT4 rectSize{ rect_w, rect_h, 0.f, 0.f };	// data to pixel shader constant buffer.

	rect_w = rect_w * 2 / screen_w;
	rect_h = rect_h * 2 / screen_h;
	float pos_x = static_cast<float>(x1) * 2 / screen_w - 1.0F;
	float pos_y = 1.0f - static_cast<float>(y1) * 2 / screen_h;

	XMFLOAT4X4 rectTransform	// row major matrix transposed, because hlsl use column major matrix.
	{							// data to vertex shader constant buffer.
		rect_w, 0.0f,   0.0f,  pos_x,
		0.0f,   rect_h, 0.0f,  pos_y,
		0.0f,   0.0f,   1.0f,  0.0f,
		0.0f,   0.0f,   0.0f,  1.0f,
	};

	// Update vertex shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource_vs{};
	m_core->GetContext()->Map(m_VSDynamicCBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource_vs);
	::memcpy(mappedResource_vs.pData, &rectTransform, sizeof(rectTransform));
	m_core->GetContext()->Unmap(m_VSDynamicCBuffer.Get(), 0U);
	m_core->GetContext()->VSSetConstantBuffers(0U, 1U, m_VSDynamicCBuffer.GetAddressOf());

	// Update pixel shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource_ps{};
	m_core->GetContext()->Map(m_PSDynamicCBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource_ps);
	::memcpy(mappedResource_ps.pData, &rectSize, sizeof(rectSize));
	m_core->GetContext()->Unmap(m_PSDynamicCBuffer.Get(), 0U);
	m_core->GetContext()->PSSetConstantBuffers(0U, 1U, m_PSDynamicCBuffer.GetAddressOf());

	m_core->GetContext()->RSSetState(m_rsstate.Get());
	m_core->GetContext()->OMSetDepthStencilState(m_dsstate.Get(), 1U);
	m_core->GetContext()->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
	m_core->GetContext()->Draw(ARRAYSIZE(vertices), 0U);
}
