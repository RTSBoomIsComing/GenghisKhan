#include "pch.h"
#include "SelectionRectRenderer.h"
#include "KhanDx/KhanDxComponents.h"


KhanRender::SelectionRectRenderer::SelectionRectRenderer(std::shared_ptr<RenderingHub> core)
	:
	Renderer(std::move(core))
{
	m_rsstate = KhanDx::GetRSState_Solid(m_core->d3d_device.Get());
	m_vertexBuffer = KhanDx::CreateVertexBuffer(m_core->d3d_device.Get(), vertices, sizeof(vertices));
	m_indexBuffer = KhanDx::CreateIndexBuffer(m_core->d3d_device.Get(), indices, sizeof(indices));
	m_blendState = KhanDx::CreateBlendState_Alpha(m_core->d3d_device.Get());
	m_pixelShader = KhanDx::CreatePixelShader(m_core->d3d_device.Get(), "PixelShader.cso");

	ComPtr<ID3DBlob> shaderBlob = KhanDx::CreateShaderBlob("VertexShader.cso");
	m_vertexShader = KhanDx::CreateVertexShader(m_core->d3d_device.Get(), shaderBlob.Get());
	m_inputLayout = KhanDx::CreateInputLayout(m_core->d3d_device.Get(), shaderBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_PSDynamicCBuffer = KhanDx::CreateDynamicCBuffer<DirectX::XMFLOAT4, 1U>(m_core->d3d_device.Get());
	m_VSDynamicCBuffer = KhanDx::CreateDynamicCBuffer<DirectX::XMFLOAT4X4, 1U>(m_core->d3d_device.Get());
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

	m_core->d3d_context->RSSetState(m_rsstate.Get());

	UINT Stride = sizeof(Vertex);
	UINT offset{};
	m_core->d3d_context->IASetVertexBuffers(0U, 1U, m_vertexBuffer.GetAddressOf(), &Stride, &offset);


	m_core->d3d_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	m_core->d3d_context->PSSetShader(m_pixelShader.Get(), nullptr, 0u);


	m_core->d3d_context->IASetInputLayout(m_inputLayout.Get());
	m_core->d3d_context->VSSetShader(m_vertexShader.Get(), nullptr, 0u);

	float rect_w = static_cast<float>(x2 - x1);
	float rect_h = static_cast<float>(y2 - y1);

	XMFLOAT4 rectSize{ rect_w, rect_h, 0.f, 0.f };

	rect_w = rect_w * 2 / m_core->rt_width;
	rect_h = rect_h * 2 / m_core->rt_height;
	float pos_x = float(x1) * 2 / m_core->rt_width - 1.0f;
	float pos_y = 1.0f - float(y1) * 2 / m_core->rt_height;

	XMFLOAT4X4 rectTransform // row major matrix transposed, because hlsl use column major matrix.
	{
		rect_w, 0.0f,   0.0f,  pos_x,
		0.0f,   rect_h, 0.0f,  pos_y,
		0.0f,   0.0f,   1.0f,  0.0f,
		0.0f,   0.0f,   0.0f,  1.0f,
	};

	//static ComPtr<ID3D11Buffer> vsDynamicCBuffer = KhanDx::CreateDynamicCBuffer<XMFLOAT4X4, 1U>(m_core->d3d_device.Get());
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	m_core->d3d_context->Map(m_VSDynamicCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &rectTransform, sizeof(rectTransform));
	m_core->d3d_context->Unmap(m_VSDynamicCBuffer.Get(), 0u);

	m_core->d3d_context->VSSetConstantBuffers(0u, 1u, m_VSDynamicCBuffer.GetAddressOf());


	//static ComPtr<ID3D11Buffer> psDynamicCBuffer = KhanDx::CreateDynamicCBuffer<XMFLOAT4, 1U>(m_core->d3d_device.Get());
	D3D11_MAPPED_SUBRESOURCE mappedResource_ps{};

	m_core->d3d_context->Map(m_PSDynamicCBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource_ps);
	::memcpy(mappedResource_ps.pData, &rectSize, sizeof(rectSize));
	m_core->d3d_context->Unmap(m_PSDynamicCBuffer.Get(), 0U);

	m_core->d3d_context->PSSetConstantBuffers(0U, 1U, m_PSDynamicCBuffer.GetAddressOf());

	m_core->d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_core->d3d_context->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);

	m_core->d3d_context->DrawIndexed(ARRAYSIZE(indices), 0u, 0u);
}
