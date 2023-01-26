#pragma once

#include <d3d11.h>
#include <memory>
namespace Khan
{
	class RSState_WireFrame
	{
		RSState_WireFrame() noexcept;
	public:
		void Bind() noexcept;

	private:
		ComPtr<ID3D11RasterizerState> rsstate;
	public:
		static std::shared_ptr<RSState_WireFrame> Get()
		{
			static std::shared_ptr<RSState_WireFrame> instance(new RSState_WireFrame{});
			return instance;
		}
	};

	#define rsstate_wireframe RSState_WireFrame::Get()




}

