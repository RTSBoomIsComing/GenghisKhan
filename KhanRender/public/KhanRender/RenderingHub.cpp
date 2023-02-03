#include "pch.h"
#include "RenderingHub.h"
#include "KhanDx/KhanDxUtils.h"
#include "KhanDx/KhanDxFactories.h"

KhanRender::RenderingHub::RenderingHub(HWND hwnd, UINT width, UINT height)
{
	screen_width = width;
	screen_height = height;
	
	std::tie(m_device, m_context) = KhanDx::CreateDeviceAndContext();
	m_swapchain = KhanDx::CreateSwapChain(m_device.Get(), hwnd);
	m_rtview    = KhanDx::CreateRenderTargetView(m_device.Get(), m_swapchain.Get(), width, height);
	m_viewport  = KhanDx::CreateDefaultViewport(static_cast<float>(width), static_cast<float>(height));
	m_dsview    = KhanDx::CreateDepthStencilView(m_device.Get(), width, height);
}

void KhanRender::RenderingHub::ResizeRenderTarget(UINT width, UINT height) noexcept
{
	screen_width = width;
	screen_height = height;
	m_context->OMSetRenderTargets(0U, nullptr, nullptr);
	m_context->Flush();
	m_rtview = nullptr;
	m_dsview = nullptr;

	m_swapchain->ResizeBuffers(0U, 0U, 0U, DXGI_FORMAT_UNKNOWN, 0U);

	m_rtview = KhanDx::CreateRenderTargetView(m_device.Get(), m_swapchain.Get(), width, height);
	m_dsview = KhanDx::CreateDepthStencilView(m_device.Get(), width, height);
	m_viewport = KhanDx::CreateDefaultViewport(static_cast<float>(width), static_cast<float>(height));

	//Reset camera's aspect ratio based on backBufferWidth/backBufferHeight
}

void KhanRender::RenderingHub::RenderBegin() noexcept
{
	m_context->RSSetViewports(1u, &m_viewport);
	m_context->OMSetRenderTargets(1u, m_rtview.GetAddressOf(), m_dsview.Get());

	float bg_color[]{ 0.0F, 0.0F, 0.0F, 0.0F };
	m_context->ClearRenderTargetView(m_rtview.Get(), bg_color);
	m_context->ClearDepthStencilView(m_dsview.Get(), D3D11_CLEAR_DEPTH, 1.0F, 0U);
}

void KhanRender::RenderingHub::RenderEnd() noexcept
{
	m_swapchain->Present(1U, 0U);
}
