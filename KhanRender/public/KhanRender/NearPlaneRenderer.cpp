#include "pch.h"
#include "NearPlaneRenderer.h"
#include "KhanDx/KhanDxComponents.h"

KhanRender::NearPlaneRenderer::NearPlaneRenderer(const Renderer& renderer)
	:
	Renderer(renderer)
{
	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice.Get(), vertices, sizeof(vertices));

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_NearPlane");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), elementDescs, ARRAYSIZE(elementDescs));

	m_pBlendState = KhanDx::CreateBlendState_Alpha(m_pDevice.Get());
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
}