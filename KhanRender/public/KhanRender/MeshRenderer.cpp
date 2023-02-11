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
	using namespace DirectX;
	// Load the model using Assimp
	Assimp::Importer importer;

	//"D:\\Assets\\monkey.obj"
	const aiScene* scene = importer.ReadFile("D:/Assets/X Bot.fbx", aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	UINT accNumVertices{};
	UINT accNumIndices{};
	UINT numMeshes = scene->mNumMeshes;

	for (UINT i{}; i < numMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		Model model{};
		model.NumVertices = mesh->mNumVertices;
		model.NumIndices = mesh->mNumFaces * 3;
		model.BaseVertexLocation = accNumVertices;
		model.StartIndexLocation = accNumIndices;
		m_models.push_back(model);

		accNumVertices += model.NumVertices;
		accNumIndices += model.NumIndices;
	}

	std::vector<UINT> m_indices; // I think this would be a member variable... but not now, just let it as a local variable
	m_indices.reserve(accNumIndices);
	for (UINT i{}; i < numMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		for (UINT j{}; j < mesh->mNumFaces; ++j)
		{
			aiFace face = mesh->mFaces[j];
			m_indices.insert(m_indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}
	}

	std::vector<XMFLOAT3> m_vertexPositions; // I think this would be a member variable... but not now, just let it as a local variable
	m_vertexPositions.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		m_vertexPositions.insert(m_vertexPositions.end(), reinterpret_cast<XMFLOAT3*>(mesh->mVertices), reinterpret_cast<XMFLOAT3*>(mesh->mVertices + mesh->mNumVertices));
	}

	m_elementDescs.emplace_back("POSITION", 0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, 0U, D3D11_INPUT_PER_VERTEX_DATA, 0U);
	//{ "TEXCOORD", 0U, DXGI_FORMAT_R32G32_FLOAT,    0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },
	//{ "NORMAL",   0U, DXGI_FORMAT_R32G32B32_FLOAT, 0U, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0U },


	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice, m_vertexPositions.data(), sizeof(m_vertexPositions[0]) * (UINT)m_vertexPositions.size());
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice, m_indices.data(), sizeof(m_indices[0]) * (UINT)m_indices.size());

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice, "PS_3dModelFromAssimp");
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_3dModelFromAssimp");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice, pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice, pBlob.Get(), m_elementDescs.data(), (UINT)m_elementDescs.size());

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
	for (auto& model : m_models)
	{
		m_pDeviceContext->DrawIndexedInstanced(model.NumIndices, m_numInstance, model.StartIndexLocation, model.BaseVertexLocation, 0U);
	}
	
}
