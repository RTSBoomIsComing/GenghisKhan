#include "pch.h"
#include "SkeletalMeshRenderer.h"
#include "KhanDx/KhanDxComponents.h"

#include <KhanTools/Log.h>
#include "KhanAnim/SkeletalAnimation.h"

// standard libraries
#include <unordered_map>
#include <queue>
// additional dependencies
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


KhanRender::SkeletalMeshRenderer::SkeletalMeshRenderer(const Renderer& renderer, const std::filesystem::path SceneFilePath)
	:
	Renderer(renderer)
{
	using namespace DirectX;

	// Load the model using Assimp
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* pScene = importer.ReadFile(SceneFilePath.string(), aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
	if (nullptr == pScene) {
		KHAN_ERROR(importer.GetErrorString());
		throw std::exception{ "Failed to import file using assimp" };
	}

	DirectX::XMMATRIX rootNodeTransform = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&pScene->mRootNode->mTransformation));

	unsigned int accNumVertices{};
	unsigned int accNumIndices{};
	unsigned int numMeshes = pScene->mNumMeshes;
	m_MeshInfos.reserve(numMeshes);
	for (unsigned int i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		MeshInfo meshInfo{};
		meshInfo.NumVertices = pMesh->mNumVertices;
		meshInfo.NumIndices = pMesh->mNumFaces * 3;
		meshInfo.BaseVertexLocation = static_cast<unsigned int>(accNumVertices);
		meshInfo.StartIndexLocation = static_cast<unsigned int>(accNumIndices);
		m_MeshInfos.push_back(meshInfo);

		accNumVertices += meshInfo.NumVertices;
		accNumIndices += meshInfo.NumIndices;
	}

	// get texture per mesh, and create shader resource view
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
			m_MeshInfos[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), pAiTexture);
		}
		else
		{
			std::filesystem::path textureFilePath = aiPath.C_Str();
			assert(SceneFilePath.has_parent_path() && textureFilePath.has_filename() && "Something wrong with file path");

			textureFilePath = SceneFilePath.parent_path() / std::filesystem::path{ "textures" } / textureFilePath.filename();
			m_MeshInfos[i].m_pSRV = KhanDx::CreateSRV_Texture2D(m_pDevice.Get(), textureFilePath);
		}
	}

	std::vector<UINT> m_indices;
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

	std::vector<XMFLOAT3> v_texCoords;
	v_texCoords.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		int numVertices = pMesh->mNumVertices;	// the size of textureCoords equals to numVertices
		XMFLOAT3* First = reinterpret_cast<XMFLOAT3*>(pMesh->mTextureCoords[0]);
		assert(nullptr != First && "There are no textureCoords, check assimp importer read-file flag, aiProcess_GenUVCoords");
		XMFLOAT3* Last = First + numVertices;
		v_texCoords.insert(v_texCoords.end(), First, Last);
	}

	std::vector<XMFLOAT3> v_normals;
	v_normals.reserve(accNumVertices);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		int numVertices = pMesh->mNumVertices;  // the size of normals equals to numVertices
		XMFLOAT3* First = reinterpret_cast<XMFLOAT3*>(pMesh->mNormals);
		assert(nullptr != First && "There are no normals, check assimp importer read-file flag, aiProcess_GenNormals");
		XMFLOAT3* Last = First + numVertices;
		v_normals.insert(v_normals.end(), First, Last);
	}

	//store node from root to leaves(breadth first search)
	std::vector<const aiNode*> nodes_bfs;
	{
		std::queue<const aiNode*> bfsQueue;
		const aiNode* pRootNode = pScene->mRootNode;
		bfsQueue.push(pRootNode);
		while (!bfsQueue.empty())
		{
			const aiNode* pNode = bfsQueue.front();
			bfsQueue.pop();
			nodes_bfs.push_back(pNode); // visit
			for (unsigned int i{}; i < pNode->mNumChildren; i++)
			{
				bfsQueue.push(pNode->mChildren[i]);
			}
		}
	}
	std::unordered_map<const aiNode*, const aiBone*> nodeToBone;
	std::vector<std::array<uint32_t, MAX_NUM_BONE_WEIGHTS>> v_boneIndices(accNumVertices);
	std::vector<std::array<float, MAX_NUM_BONE_WEIGHTS>> v_weights(accNumVertices);

	for (unsigned int i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		for (unsigned int j{}; j < pMesh->mNumBones; j++)
		{
			auto* pBone = pMesh->mBones[j];
			if (!nodeToBone.contains(pBone->mNode))
			{
				nodeToBone[pBone->mNode] = pBone;
			}
		}
	}

	m_TotalNumBones = static_cast<unsigned int>(nodeToBone.size());

	std::vector<const aiBone*> bones;
	bones.reserve(m_TotalNumBones);
	for (const aiNode* pNode : nodes_bfs)
	{
		if (nodeToBone.contains(pNode))
		{
			bones.push_back(nodeToBone[pNode]);
		}
	}

	std::unordered_map<const aiNode*, unsigned int> idPerNode;
	for (int i{}; i < bones.size(); i++)
	{
		const aiNode* pNode = bones[i]->mNode;
		idPerNode[pNode] = i;
	}

	std::vector<unsigned int>		parentNodes(m_TotalNumBones, -1);
	std::vector<DirectX::XMMATRIX>	defaultBoneTransforms;
	std::vector<DirectX::XMMATRIX>	boneOffsets;
	std::vector<std::string>		nodeNames;
	defaultBoneTransforms.reserve(m_TotalNumBones);
	boneOffsets.reserve(m_TotalNumBones);
	nodeNames.reserve(m_TotalNumBones);


	for (unsigned int i{}; i < m_TotalNumBones; i++)
	{
		boneOffsets.push_back(XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&bones[i]->mOffsetMatrix)));
		defaultBoneTransforms.push_back(XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&bones[i]->mNode->mTransformation)));
		nodeNames.push_back(bones[i]->mName.C_Str());
		if (idPerNode.contains(bones[i]->mNode->mParent))
		{
			parentNodes[i] = idPerNode[bones[i]->mNode->mParent];
		}
	}

	for (unsigned int i{}; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		for (unsigned int j{}; j < pMesh->mNumBones; j++)
		{
			const aiBone* pBone = pMesh->mBones[j];
			for (unsigned int k{}; k < pBone->mNumWeights; k++)
			{
				const auto weightInfo = pBone->mWeights[k];

				const unsigned int vertexId = weightInfo.mVertexId + m_MeshInfos[i].BaseVertexLocation;
				const float weight = weightInfo.mWeight;

				for (int l{}; l < MAX_NUM_BONE_WEIGHTS; l++)
				{
					if (v_weights[vertexId][l] == 0)
					{
						v_boneIndices[vertexId][l] = idPerNode[pBone->mNode];
						v_weights[vertexId][l] = weight;
						break;

					}
				}
			}
		}
	}
	unsigned int accNumBoneTransforms{};
	KhanAnim::SkeletalAnimation animation{ "..\\Assets\\standing draw arrow.fbx" };
	m_AnimationInfos.emplace_back(accNumBoneTransforms, animation.m_Duration, animation.m_TicksPerSecond);
	accNumBoneTransforms += animation.m_Duration * m_TotalNumBones;

	m_FinalBoneTransforms.reserve(static_cast<size_t>(accNumBoneTransforms));
	for (unsigned int i{}; i < animation.m_Duration; i++)
	{
		for (unsigned int j{}; j < m_TotalNumBones; j++)
		{

			if (animation.m_AnimNodeTransforms.contains(nodeNames[j])
				&& i < animation.m_AnimNodeTransforms[nodeNames[j]].size())
			{
				m_FinalBoneTransforms.push_back(animation.m_AnimNodeTransforms[nodeNames[j]][i]);
			}
			else
			{
				m_FinalBoneTransforms.push_back(defaultBoneTransforms[j]);
			}

			unsigned int parentId = parentNodes[j];
			if (parentId != -1)
			{
				m_FinalBoneTransforms.back() = m_FinalBoneTransforms[static_cast<size_t>(m_TotalNumBones) * i + parentId] * m_FinalBoneTransforms.back();
			}
		}

		for (unsigned int j{}; j < m_TotalNumBones; j++)
		{
			const size_t id = static_cast<size_t>(m_TotalNumBones) * i + j;
			m_FinalBoneTransforms[id] = m_FinalBoneTransforms[id] * boneOffsets[j];
			m_FinalBoneTransforms[id] = rootNodeTransform * m_FinalBoneTransforms[id];
		}
	}

	// Create Vertex Buffers in SOA pattern

	// Vertex buffer cant be empty
	size_t vertexBufId = static_cast<size_t>(VertexElement::POSITION);
	m_VBuffer.Bufs[vertexBufId] = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_positions.data(), (UINT)v_positions.size() * m_VBuffer.Strides[vertexBufId]);
	m_VBuffer.Addresses[vertexBufId] = m_VBuffer.Bufs[vertexBufId].Get();

	if (!v_texCoords.empty())
	{
		vertexBufId = static_cast<size_t>(VertexElement::TEXCOORD);
		m_VBuffer.Bufs[vertexBufId] = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_texCoords.data(), (UINT)v_texCoords.size() * m_VBuffer.Strides[vertexBufId]);
		m_VBuffer.Addresses[vertexBufId] = m_VBuffer.Bufs[vertexBufId].Get();
	}
	if (!v_normals.empty())
	{
		vertexBufId = static_cast<size_t>(VertexElement::NORMAL);
		m_VBuffer.Bufs[vertexBufId] = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_normals.data(), (UINT)v_normals.size() * m_VBuffer.Strides[vertexBufId]);
		m_VBuffer.Addresses[vertexBufId] = m_VBuffer.Bufs[vertexBufId].Get();
	}
	if (!v_boneIndices.empty())
	{
		vertexBufId = static_cast<size_t>(VertexElement::BLENDINDICES);
		m_VBuffer.Bufs[vertexBufId] = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_boneIndices.data(), (UINT)v_boneIndices.size() * m_VBuffer.Strides[vertexBufId]);
		m_VBuffer.Addresses[vertexBufId] = m_VBuffer.Bufs[vertexBufId].Get();
	}
	if (!v_weights.empty())
	{
		vertexBufId = static_cast<size_t>(VertexElement::BLENDWEIGHT);
		m_VBuffer.Bufs[vertexBufId] = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_weights.data(), (UINT)v_weights.size() * m_VBuffer.Strides[vertexBufId]);
		m_VBuffer.Addresses[vertexBufId] = m_VBuffer.Bufs[vertexBufId].Get();
	}

	// Create Index buffer
	// Index buffer cant be empty
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice.Get(), m_indices.data(), (uint32_t)m_indices.size() * sizeof(m_indices[0]));

	// Create Pixel shader
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "PS_SkeletalMesh");

	// Create Vertex shader and Input layout
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("VS_SkeletalMesh");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), m_elementDescs, NUM_VERTEX_ELEMENTS);

	// Create Constant bufferes
	m_VSCBuffer.Worlds = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(XMMATRIX), 1000);
	m_VSCBuffer.Addresses[0] = m_VSCBuffer.Worlds.Get();

	m_VSCBuffer.ViewProjection = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(XMMATRIX), 1);
	m_VSCBuffer.Addresses[1] = m_VSCBuffer.ViewProjection.Get();

	m_VSCBuffer.Blending = KhanDx::CreateDynConstBuf(m_pDevice.Get(), sizeof(uint32_t), 1000);
	m_VSCBuffer.Addresses[2] = m_VSCBuffer.Blending.Get();


	m_pSRV_VS_FinalBoneTransforms = KhanDx::CreateSRV_StaticStructBuf(m_pDevice.Get(), m_FinalBoneTransforms.data(), sizeof(DirectX::XMMATRIX), static_cast<uint32_t>(m_FinalBoneTransforms.size()));

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
	m_pSamplerState = KhanDx::CreateSamplerState_Basic(m_pDevice.Get());
}

size_t KhanRender::SkeletalMeshRenderer::AddInstance(DirectX::XMMATRIX const& worldMatrix, const uint32_t AnimationId, const float runningTime)
{
	using namespace DirectX;
	m_InstanceWorldMatrices.push_back(XMMatrixTranspose(worldMatrix));

	unsigned int animId = AnimationId;
	if (animId >= m_AnimationInfos.size())
	{
		KHAN_ERROR("Out of range, AnimationId");
		DebugBreak();
	}

	const auto& animInfo = m_AnimationInfos[animId];
	const uint32_t tick = static_cast<uint32_t>(runningTime * animInfo.TicksPerSecond) % animInfo.Durations;

	m_InstanceBoneTransformStartLocations.push_back((animInfo.AnimationStartOffset + tick) * m_TotalNumBones);


	return m_InstanceWorldMatrices.size();
}

void KhanRender::SkeletalMeshRenderer::Update(DirectX::XMMATRIX const& viewProjMat)
{
	using namespace DirectX;

	// ConstantBuffer for View-Projection matrix
	XMMATRIX viewProjMatTransposed = XMMatrixTranspose(viewProjMat);

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	m_pDeviceContext->Map(m_VSCBuffer.ViewProjection.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, &viewProjMatTransposed, sizeof(XMMATRIX));
	m_pDeviceContext->Unmap(m_VSCBuffer.ViewProjection.Get(), 0);


	// ConstantBuffer for World matrices. These matrices are used for instancing
	m_NumInstances = static_cast<uint32_t>(m_InstanceWorldMatrices.size());

	mappedResource = {};
	m_pDeviceContext->Map(m_VSCBuffer.Worlds.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, m_InstanceWorldMatrices.data(), sizeof(XMMATRIX) * m_NumInstances);
	m_pDeviceContext->Unmap(m_VSCBuffer.Worlds.Get(), 0);



	mappedResource = {};
	m_pDeviceContext->Map(m_VSCBuffer.Blending.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	std::memcpy(mappedResource.pData, m_InstanceBoneTransformStartLocations.data(), sizeof(uint32_t[4]) * m_NumInstances);
	m_pDeviceContext->Unmap(m_VSCBuffer.Blending.Get(), 0);

	m_InstanceWorldMatrices.resize(0);
	m_InstanceBoneTransformStartLocations.resize(0);
}

void KhanRender::SkeletalMeshRenderer::Render()
{
	m_pDeviceContext->IASetVertexBuffers(0, NUM_VERTEX_ELEMENTS, m_VBuffer.Addresses.data(), m_VBuffer.Strides.data(), m_VBuffer.Offsets.data());
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, static_cast<UINT>(m_VSCBuffer.Addresses.size()), m_VSCBuffer.Addresses.data());
	m_pDeviceContext->VSSetShaderResources(0, 1, m_pSRV_VS_FinalBoneTransforms.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);

	for (auto& meshInfo : m_MeshInfos)
	{
		m_pDeviceContext->PSSetShaderResources(0, 1, meshInfo.m_pSRV.GetAddressOf());
		m_pDeviceContext->DrawIndexedInstanced(meshInfo.NumIndices, m_NumInstances, meshInfo.StartIndexLocation, meshInfo.BaseVertexLocation, 0);
	}
}
