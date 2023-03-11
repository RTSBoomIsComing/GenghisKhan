#pragma once
#include "NearPlaneRenderer.h"

namespace KhanRender
{
	class LaserRenderer : public NearPlaneRenderer
	{
	public:
		LaserRenderer(const NearPlaneRenderer& renderer);
		void Render();
		void Update(DirectX::XMMATRIX const& inverseViewProjMat);
	private:
		ComPtr<ID3D11PixelShader>	m_pPixelShader;
		ComPtr<ID3D11Buffer>		m_pPSDynConstBuf;
	};
}

