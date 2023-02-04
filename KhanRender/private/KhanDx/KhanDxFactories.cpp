#include "pch.h"
#include "KhanDxFactories.h"
#include <KhanDx/KhanDxUtils.h>


void KhanDx::CreateDeviceAndDeviceContext(ComPtr<ID3D11Device>& pDevice, ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	static ComPtr<ID3D11Device> s_pDevice;
	static ComPtr<ID3D11DeviceContext> s_pDeviceContext;
	if (s_pDevice && s_pDeviceContext)
	{
		pDevice = s_pDevice;
		pDeviceContext = s_pDeviceContext;
		return;
	}

	UINT CreateDeviceFlags{};
#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, CreateDeviceFlags, nullptr, 0U,
		D3D11_SDK_VERSION, &s_pDevice, nullptr,
		&s_pDeviceContext);
	ThrowIfFailed(hr, "Failed to Create device and context");

	pDevice = s_pDevice;
	pDeviceContext = s_pDeviceContext;
}

ComPtr<IDXGISwapChain> KhanDx::CreateSwapChain(ComPtr<ID3D11Device> pDevice, HWND hWnd)
{
	ComPtr<IDXGISwapChain> pSwapChain;

	ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = ::CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
	ThrowIfFailed(hr, "Failed to Create DXGI Factory");

	DXGI_MODE_DESC bufDesc{};
	bufDesc.Width = 0U;  // set as window client area 
	bufDesc.Height = 0U; // set as window client area 
	bufDesc.RefreshRate.Numerator = 0U;
	bufDesc.RefreshRate.Denominator = 0U;
	bufDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapchainDesc{};
	swapchainDesc.BufferDesc = bufDesc;
	swapchainDesc.SampleDesc.Count = 1U;
	swapchainDesc.SampleDesc.Quality = 0U;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 2U;
	swapchainDesc.OutputWindow = hWnd;
	swapchainDesc.Windowed = true;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = 0U;

	hr = pFactory->CreateSwapChain(pDevice.Get(), &swapchainDesc, &pSwapChain);
	ThrowIfFailed(hr, "Failed to create swap chain");

	return pSwapChain;
}

ComPtr<ID3D11RenderTargetView> KhanDx::CreateRTV(ComPtr<ID3D11Device> pDevice, ComPtr<IDXGISwapChain> pSwapChain)
{
	ComPtr<ID3D11Resource> pBackBuffer;
	HRESULT hr = pSwapChain->GetBuffer(0u, IID_PPV_ARGS(&pBackBuffer));
	ThrowIfFailed(hr, "Failed to get the back buffer from swap chain");

	ComPtr<ID3D11RenderTargetView> pRTV;
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRTV);
	ThrowIfFailed(hr, "Failed to create render target view");

	return pRTV;
}

ComPtr<ID3D11DepthStencilView> KhanDx::CreateDSV(ComPtr<ID3D11Device> pDevice, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1U;
	texDesc.ArraySize = 1U;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1U;
	texDesc.SampleDesc.Quality = 0U;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0U;
	texDesc.MiscFlags = 0U;

	ComPtr<ID3D11Texture2D> pTex;
	HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &pTex);
	ThrowIfFailed(hr, "Failed to create texture of depth stencil");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ComPtr<ID3D11DepthStencilView> pDSV;
	hr = pDevice->CreateDepthStencilView(pTex.Get(), &dsvDesc, &pDSV);
	ThrowIfFailed(hr, "failed to create depth stencil view");

	return pDSV;
}