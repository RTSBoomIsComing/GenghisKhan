#include "pch.h"
#include "SkeletalMeshRenderer.h"
#include "KhanDx/KhanDxComponents.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <KhanTools/Log.h>

KhanRender::SkeletalMeshRenderer::SkeletalMeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath)
	:
	Renderer(renderer)
{
	using namespace DirectX;

	// Load the model using Assimp
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(SceneFilePath.string(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (nullptr == pScene) {
		KHAN_ERROR(importer.GetErrorString());
		throw std::exception{ "Failed to import file using assimp" };
	}

	UINT accNumVertices{};
	UINT accNumIndices{};
	UINT accNumBones{};
	UINT numMeshes = pScene->mNumMeshes;
	m_meshInfos.reserve(numMeshes);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		MeshInfo meshInfo{};
		meshInfo.NumVertices = pMesh->mNumVertices;
		meshInfo.NumIndices = pMesh->mNumFaces * 3;
		meshInfo.BaseVertexLocation = accNumVertices;
		meshInfo.StartIndexLocation = accNumIndices;
		m_meshInfos.push_back(meshInfo);

		accNumVertices += meshInfo.NumVertices;
		accNumIndices += meshInfo.NumIndices;
		accNumBones += pMesh->mNumBones;
	}

	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		auto* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) < 1) { continue; }

		aiString aiPath;
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath) != aiReturn_SUCCESS) { continue; }

		const aiTexture* pAiTexture = pScene->GetEmbeddedTexture(aiPath.C_Str());
		if (pAiTexture)
		{
			m_meshInfos[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), pAiTexture);
		}
		else
		{
			std::filesystem::path textureFilePath = aiPath.C_Str();
			assert(SceneFilePath.has_parent_path() && textureFilePath.has_filename() && "Something wrong with file path");

			textureFilePath = SceneFilePath.parent_path() / textureFilePath.filename();
			m_meshInfos[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), textureFilePath);
		}
	}

	std::vector<UINT> m_indices; // I think this would be a member variable... but not now, just let it as a local variable
	m_indices.reserve(accNumIndices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		for (UINT j{}; j < pMesh->mNumFaces; j++)
		{
			aiFace face = pMesh->mFaces[j];
			m_indices.insert(m_indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}
	}

	std::vector<XMFLOAT3> v_positions; 
	v_positions.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		int numVertices = pMesh->mNumVertices;
		XMFLOAT3* First = reinterpret_cast<XMFLOAT3*>(pMesh->mVertices);
		XMFLOAT3* Last = First + numVertices;
		v_positions.insert(v_positions.end(), First, Last);
	}

	std::vector<XMFLOAT3> v_normals; 
	v_normals.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		int numVertices = pMesh->mNumVertices;
		XMFLOAT3* First = reinterpret_cast<XMFLOAT3*>(pMesh->mNormals);
		XMFLOAT3* Last = First + numVertices;
		v_normals.insert(v_normals.end(), First, Last);
	}

	std::vector<XMFLOAT3> v_texCoords;
	v_texCoords.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		int numVertices = pMesh->mNumVertices;
		XMFLOAT3* First = reinterpret_cast<XMFLOAT3*>(pMesh->mTextureCoords[0]);
		XMFLOAT3* Last = First + numVertices;
		v_texCoords.insert(v_texCoords.end(), First, Last);
	}

	//UINT accNumWeights{};
	//for (UINT i{}; i < numMeshes; i++)
	//{
	//	auto* pMesh = pScene->mMeshes[i];
	//	UINT numBones = pMesh->mNumBones;

	//	std::vector<XMFLOAT4X4> offsetMatrices(numBones);
	//	for (UINT j{}; j < numBones; j++)
	//	{
	//		auto* bone = pMesh->mBones[j];
	//		XMFLOAT4X4* pOffMat = reinterpret_cast<XMFLOAT4X4*>(&bone->mOffsetMatrix);
	//		std::copy(pOffMat, pOffMat + 1, &offsetMatrices[j]);


	//		UINT numWeights = bone->mNumWeights;	
	//		auto* weights = bone->mWeights;
	//		for (UINT k{}; k < numWeights; k++)
	//		{
	//			float weight = weights[k].mWeight;
	//		}
	//		accNumWeights += numWeights;
	//	}
	//}


	m_VBuf_Strides.push_back(sizeof(v_positions[0]));
	m_pVBuf_Positions = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_positions.data(), m_VBuf_Strides.back() * v_positions.size());
	m_VBuf_Ptrs.push_back(m_pVBuf_Positions.Get());
	m_VBuf_Offsets.push_back(0);

	m_VBuf_Strides.push_back(sizeof(v_texCoords[0]));
	m_pVBuf_TexCoords = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_texCoords.data(), m_VBuf_Strides.back() * v_texCoords.size());
	m_VBuf_Ptrs.push_back(m_pVBuf_TexCoords.Get());
	m_VBuf_Offsets.push_back(0);

	m_VBuf_Strides.push_back(sizeof(v_normals[0]));
	m_pVBuf_Normals = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_normals.data(), m_VBuf_Strides.back() * v_normals.size());
	m_VBuf_Ptrs.push_back(m_pVBuf_Normals.Get());
	m_VBuf_Offsets.push_back(0);


	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice.Get(), m_indices.data(), sizeof(m_indices[0]) * (UINT)m_indices.size());

	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "SkeletalMesh_PS");
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("SkeletalMesh_VS");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), m_elementDescs.data(), (UINT)m_elementDescs.size());

	m_pCBuf_VS_Worlds = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice.Get(), 1000);
	m_pCBuf_VS_ViewProjection = KhanDx::CreateDynConstBuf<DirectX::XMFLOAT4X4>(m_pDevice.Get(), 1);

	m_CBuf_VS_Ptrs.push_back(m_pCBuf_VS_Worlds.Get());
	m_CBuf_VS_Ptrs.push_back(m_pCBuf_VS_ViewProjection.Get());

	// if need structured buffer
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
	m_pSamplerState = KhanDx::CreateSamplerState_Basic(m_pDevice.Get());
}

void KhanRender::SkeletalMeshRenderer::Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat)
{
	using namespace DirectX;

	// ConstantBuffer for World matrices. These matrices are used for instancing
	m_numInstance = (UINT)worldMats.size();
	std::vector <XMFLOAT4X4> TransposedWorldMats(m_numInstance);
	for (UINT i{}; i < m_numInstance; ++i)
	{
		XMStoreFloat4x4(&TransposedWorldMats[i], XMMatrixTranspose(worldMats[i]));
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_pCBuf_VS_Worlds.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, TransposedWorldMats.data(), sizeof(XMFLOAT4X4) * m_numInstance);
	m_pDeviceContext->Unmap(m_pCBuf_VS_Worlds.Get(), 0);

	// ConstantBuffer for View * Projection matrix
	XMFLOAT4X4 TransposedViewProjMat{};
	XMStoreFloat4x4(&TransposedViewProjMat, XMMatrixTranspose(viewProjMat));

	mappedResource = {};
	m_pDeviceContext->Map(m_pCBuf_VS_ViewProjection.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &TransposedViewProjMat, sizeof(XMFLOAT4X4));
	m_pDeviceContext->Unmap(m_pCBuf_VS_ViewProjection.Get(), 0);

	// Bone Matrix Update, maybe seperate AnimUpdate function is needed
	// TODO
}

void KhanRender::SkeletalMeshRenderer::Render()
{
	m_pDeviceContext->IASetVertexBuffers(0, m_VBuf_Ptrs.size(), m_VBuf_Ptrs.data(), m_VBuf_Strides.data(), m_VBuf_Offsets.data());
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	m_pDeviceContext->VSSetConstantBuffers(0, m_CBuf_VS_Ptrs.size(), m_CBuf_VS_Ptrs.data());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	for (auto& meshInfo : m_meshInfos)
	{
		m_pDeviceContext->PSSetShaderResources(0U, 1U, meshInfo.m_pSRV.GetAddressOf());
		m_pDeviceContext->DrawIndexedInstanced(meshInfo.NumIndices, m_numInstance, meshInfo.StartIndexLocation, meshInfo.BaseVertexLocation, 0);
	}
}
