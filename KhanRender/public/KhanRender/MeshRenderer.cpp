#include "pch.h"
#include "MeshRenderer.h"
#include "KhanDx/KhanDxComponents.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



KhanRender::MeshRenderer::MeshRenderer(const Renderer& renderer)
	:
	Renderer(renderer)
{
	// Load the model using Assimp
	Assimp::Importer importer;
	//"D:/Assets/zelda/zeldaPosed001.fbx"
	//"D:\\Assets\\monkey.obj"
	const aiScene* scene = importer.ReadFile("D:\\Assets\\monkey.obj", aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	auto mesh = scene->mMeshes[0];
	m_numVertices = mesh->mNumVertices;
	m_numIndices = mesh->mNumFaces * 3;

	std::vector<UINT> m_indices;
	m_indices.reserve(m_numIndices);
	for (UINT i{}; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (UINT j{}; j < face.mNumIndices; ++j)
			m_indices.push_back(face.mIndices[j]);
	}

	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice, scene->mMeshes[0]->mVertices, 12 * m_numVertices);
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice, m_indices.data(), sizeof(UINT) * m_numIndices);

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice, "PS_BasicGeometry3d");

	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_BasicGeometry3d");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice, pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice, pBlob.Get(), elementDescs.data(), elementDescs.size());

	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice, 1000U);
	
	// if need structured buffer
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice);
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice);

}

void KhanRender::MeshRenderer::Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat)
{
	using namespace DirectX;

	m_numInstance = (UINT)worldMats.size();
	std::vector <XMFLOAT4X4> WVPMatrices(m_numInstance);

	for (UINT i{}; i < m_numInstance; ++i)
	{
		XMStoreFloat4x4(&WVPMatrices[i], XMMatrixTranspose(worldMats[i] * viewProjMat));
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pVSDynConstBuf.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedResource);
	::memcpy(mappedResource.pData, WVPMatrices.data(), sizeof(XMFLOAT4X4) * m_numInstance);
	m_pDeviceContext->Unmap(m_pVSDynConstBuf.Get(), 0U);
}

void KhanRender::MeshRenderer::Render()
{
	UINT Stride = 12;
	UINT offset{};
	m_pDeviceContext->IASetVertexBuffers(0U, 1U, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0U);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0U);

	//m_pDeviceContext->VSSetShaderResources(0U, 1U, m_pSRV.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(0U, 1U, m_pVSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1U);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->DrawIndexedInstanced(m_numIndices, m_numInstance, 0U, 0U, 0U);
}
