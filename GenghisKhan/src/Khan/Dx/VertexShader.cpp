#include "pch.h"
#include "VertexShader.h"
#include "DxCore.h"
#include "DxFunction.h"

Khan::VertexShader::VertexShader(std::string_view fileName, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept
{
	ComPtr<ID3DBlob> vertexShader_blob = CreateShaderBlob(fileName);

	Khan::ThrowIfFailed(dx_device->CreateVertexShader(
		vertexShader_blob->GetBufferPointer(), vertexShader_blob->GetBufferSize(),
		nullptr, &m_shader),
		"failed to create vertex shader");

	Khan::ThrowIfFailed(
		dx_device->CreateInputLayout(
			elementDescs, numElements,
			vertexShader_blob->GetBufferPointer(), vertexShader_blob->GetBufferSize(), &m_inputLayout),
		"failed to create input layout");
}

void Khan::VertexShader::Bind() noexcept
{
	dx_context->IASetInputLayout(m_inputLayout.Get());
	dx_context->VSSetShader(m_shader.Get(), nullptr, 0u);
}
