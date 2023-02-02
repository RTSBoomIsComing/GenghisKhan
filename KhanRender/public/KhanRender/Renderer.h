#pragma once

#include "RenderingHub.h"

#include <memory>
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class Renderer
	{
	public:
		Renderer(std::shared_ptr<RenderingHub> core)
			:
			m_core(std::move(core)) {}
		//virtual void Render() abstract;

	protected:
		std::shared_ptr<RenderingHub> m_core;
	};
}

