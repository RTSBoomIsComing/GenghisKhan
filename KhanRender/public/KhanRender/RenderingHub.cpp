#include "pch.h"
#include "RenderingHub.h"
#include "KhanDx/KhanDxUtils.h"
#include "KhanDx/KhanDxFactories.h"

KhanRender::RenderingHub::RenderingHub(HWND hWnd, UINT width, UINT height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	
	KhanDx::CreateDeviceAndDeviceContext(m_pDevice, m_pDeviceContext);

	m_pSwapChain = KhanDx::CreateSwapChain(m_pDevice.Get(), hWnd);
	m_pRTV    = KhanDx::CreateRTV(m_pDevice.Get(), m_pSwapChain.Get());
	m_pDSV    = KhanDx::CreateDSV(m_pDevice.Get(), width, height);
	m_viewport  = CD3D11_VIEWPORT(0.0F, 0.0F, float(width), float(height));
}

void KhanRender::RenderingHub::ResizeRenderTarget(UINT width, UINT height) noexcept
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_pDeviceContext->OMSetRenderTargets(0U, nullptr, nullptr);
	m_pDeviceContext->Flush();
	m_pRTV = nullptr;
	m_pDSV = nullptr;

	m_pSwapChain->ResizeBuffers(0U, 0U, 0U, DXGI_FORMAT_UNKNOWN, 0U);

	m_pRTV = KhanDx::CreateRTV(m_pDevice.Get(), m_pSwapChain.Get());
	m_pDSV = KhanDx::CreateDSV(m_pDevice.Get(), width, height);
	m_viewport = CD3D11_VIEWPORT(0.0F, 0.0F, float(width), float(height));

	//Reset camera's aspect ratio based on backBufferWidth/backBufferHeight
}

void KhanRender::RenderingHub::RenderBegin() noexcept
{
	m_pDeviceContext->RSSetViewports(1U, &m_viewport);
	m_pDeviceContext->OMSetRenderTargets(1u, m_pRTV.GetAddressOf(), m_pDSV.Get());

	float bg_color[]{ 0.0F, 0.0F, 0.0F, 0.0F };
	m_pDeviceContext->ClearRenderTargetView(m_pRTV.Get(), bg_color);
	m_pDeviceContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0F, 0U);
}

void KhanRender::RenderingHub::RenderEnd() noexcept
{
	m_pSwapChain->Present(1U, 0U);
}
