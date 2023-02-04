#include "pch.h"
#include "SelectionRectRenderer.h"
#include "KhanDx/KhanDxComponents.h"


KhanRender::SelectionRectRenderer::SelectionRectRenderer(std::shared_ptr<RenderingHub> core)
	:
	Renderer(core)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice, vertices, sizeof(vertices));
	m_pPixelShader  = KhanDx::CreatePixelShader(m_pDevice, "PixelShader");

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VertexShader");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice, pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice, pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pPSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4>(m_pDevice, 1U);
	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice, 1U);

	m_pBlendState = KhanDx::CreateBlendState_Alpha(m_pDevice);
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice);
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice);
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
	m_pDeviceContext->IASetVertexBuffers(0U, 1U, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);

	float screen_w = static_cast<float>(m_screenWidth);
	float screen_h = static_cast<float>(m_screenHeight);

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
	m_pDeviceContext->Map(m_pVSDynConstBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource_vs);
	::memcpy(mappedResource_vs.pData, &rectTransform, sizeof(rectTransform));
	m_pDeviceContext->Unmap(m_pVSDynConstBuf.Get(), 0U);
	m_pDeviceContext->VSSetConstantBuffers(0U, 1U, m_pVSDynConstBuf.GetAddressOf());

	// Update pixel shader dynamic constant buffer.
	D3D11_MAPPED_SUBRESOURCE mappedResource_ps{};
	m_pDeviceContext->Map(m_pPSDynConstBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource_ps);
	::memcpy(mappedResource_ps.pData, &rectSize, sizeof(rectSize));
	m_pDeviceContext->Unmap(m_pPSDynConstBuf.Get(), 0U);
	m_pDeviceContext->PSSetConstantBuffers(0U, 1U, m_pPSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1U);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->Draw(ARRAYSIZE(vertices), 0U);
}
