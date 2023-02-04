#pragma once
#include <d3d11.h>
#include <tuple>
#include <wrl.h>
using Microsoft::WRL::ComPtr;


namespace KhanDx
{
	void CreateDeviceAndDeviceContext(ComPtr<ID3D11Device>& pDevice, ComPtr<ID3D11DeviceContext>& pDeviceContext);
	ComPtr<IDXGISwapChain> CreateSwapChain(ComPtr<ID3D11Device> pDevice, HWND hWnd);
	ComPtr<ID3D11RenderTargetView> CreateRTV(ComPtr<ID3D11Device> pDevice, ComPtr<IDXGISwapChain> pSwapChain);
	ComPtr< ID3D11DepthStencilView> CreateDSV(ComPtr<ID3D11Device> pDevice, UINT width, UINT height);
}