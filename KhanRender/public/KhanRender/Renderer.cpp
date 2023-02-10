#include "pch.h"
#include "Renderer.h"
#include "KhanDx/KhanDxUtils.h"
#include "KhanDx/KhanDxFactories.h"

KhanRender::Renderer::Renderer() noexcept
{
	KhanDx::CreateDeviceAndDeviceContext(m_pDevice, m_pDeviceContext);
}
