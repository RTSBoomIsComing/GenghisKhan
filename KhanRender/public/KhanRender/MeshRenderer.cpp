#include "pch.h"
#include "MeshRenderer.h"
#include "KhanDx/KhanDxComponents.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <KhanTools/Log.h>

KhanRender::MeshRenderer::MeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath)
	:
	Renderer(renderer)
{
	using namespace DirectX;
	//"D:\\Assets\\zelda\\zeldaPosed001.fbx"
	// Load the model using Assimp
	Assimp::Importer importer;
	//std::string SceneFilePath{ "..\\Mixamo\\akai_e_espiritu.fbx" };
	//std::string SceneFilePath{ "..\\Mixamo\\X bot.fbx" };

	const aiScene* pScene = importer.ReadFile(SceneFilePath.string(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (nullptr == pScene) {
		KHAN_ERROR(importer.GetErrorString());
		throw std::exception{ "Failed to import file using assimp" };
	}

	UINT accNumVertices{};
	UINT accNumIndices{};
	UINT numMeshes = pScene->mNumMeshes;
	m_models.reserve(numMeshes);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto pMesh = pScene->mMeshes[i];
		Model model{};
		model.NumVertices = pMesh->mNumVertices;
		model.NumIndices = pMesh->mNumFaces * 3;
		model.BaseVertexLocation = accNumVertices;
		model.StartIndexLocation = accNumIndices;
		m_models.push_back(model);

		accNumVertices += model.NumVertices;
		accNumIndices += model.NumIndices;
	}

	for (UINT i{}; i < numMeshes; i++)
	{
		auto pMesh = pScene->mMeshes[i];
		auto pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) // here, we except texture count is 1. 
		{
			aiString aiPath;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath) == aiReturn_SUCCESS)
			{
				const aiTexture* pAiTexture = pScene->GetEmbeddedTexture(aiPath.C_Str());
				if (pAiTexture)
				{
					m_models[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), pAiTexture);
				}
				else
				{
					std::filesystem::path textureFilePath = aiPath.C_Str();
					if (SceneFilePath.has_parent_path() && textureFilePath.has_filename())
					{
						textureFilePath = SceneFilePath.parent_path() / textureFilePath.filename();
						m_models[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), textureFilePath);
					}
				}
			}			
		}
	}

	std::vector<UINT> m_indices; // I think this would be a member variable... but not now, just let it as a local variable
	m_indices.reserve(accNumIndices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto pMesh = pScene->mMeshes[i];
		for (UINT j{}; j < pMesh->mNumFaces; j++)
		{
			aiFace face = pMesh->mFaces[j];
			m_indices.insert(m_indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}
	}

	std::vector<Vertex> m_vertices; // I think this would be a member variable... but not now, just let it as a local variable
	m_vertices.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto pMesh = pScene->mMeshes[i];
		XMFLOAT3* xmVertices = reinterpret_cast<XMFLOAT3*>(pMesh->mVertices);
		XMFLOAT3* xmNormals = reinterpret_cast<XMFLOAT3*>(pMesh->mNormals);
		XMFLOAT3** xmTexCoords = reinterpret_cast<XMFLOAT3**>(pMesh->mTextureCoords);
		for (UINT j{}; j < pMesh->mNumVertices; j++)
		{
			m_vertices.emplace_back(xmVertices[j], xmTexCoords[0][j], xmNormals[j]);
		}
	}

	m_pVertexBuffer = KhanDx::CreateVertexBuffer(m_pDevice.Get(), m_vertices.data(), sizeof(Vertex) * (UINT)m_vertices.size());
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice.Get(), m_indices.data(), sizeof(m_indices[0]) * (UINT)m_indices.size());

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "PS_3dModelFromAssimp");
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_3dModelFromAssimp");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), m_elementDescs.data(), (UINT)m_elementDescs.size());

	m_pVSDynConstBuf = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice.Get(), 1000U);

	// if need structured buffer
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
	m_pSamplerState = KhanDx::CreateSamplerState_Basic(m_pDevice.Get());
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
	UINT vertexStride = sizeof(Vertex);
	UINT Offset{};
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &vertexStride, &Offset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0U);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0U);




	m_pDeviceContext->VSSetConstantBuffers(0U, 1U, m_pVSDynConstBuf.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1U);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	for (auto& model : m_models)
	{
		m_pDeviceContext->PSSetShaderResources(0U, 1U, model.m_pSRV.GetAddressOf());
		m_pDeviceContext->DrawIndexedInstanced(model.NumIndices, m_numInstance, model.StartIndexLocation, model.BaseVertexLocation, 0U);
	}

}
