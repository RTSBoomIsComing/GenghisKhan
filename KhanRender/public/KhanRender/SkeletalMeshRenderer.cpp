#include "pch.h"
#include "SkeletalMeshRenderer.h"
#include "KhanDx/KhanDxComponents.h"

#include <KhanTools/Log.h>

// standard libraries
#include <unordered_map>
#include <array>
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
	const aiScene* pScene = importer.ReadFile(SceneFilePath.string(), aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights | aiProcess_GenUVCoords | aiProcess_GenNormals);
	if (nullptr == pScene) {
		KHAN_ERROR(importer.GetErrorString());
		throw std::exception{ "Failed to import file using assimp" };
	}

	UINT accNumVertices{};
	UINT accNumIndices{};
	UINT numMeshes = pScene->mNumMeshes;
	m_meshInfos.reserve(numMeshes);
	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];
		MeshInfo meshInfo{};
		meshInfo.NumVertices = pMesh->mNumVertices;
		meshInfo.NumIndices = pMesh->mNumFaces * 3;
		meshInfo.BaseVertexLocation = static_cast<UINT>(accNumVertices);
		meshInfo.StartIndexLocation = static_cast<UINT>(accNumIndices);
		m_meshInfos.push_back(meshInfo);

		accNumVertices += meshInfo.NumVertices;
		accNumIndices += meshInfo.NumIndices;
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

			textureFilePath = SceneFilePath.parent_path() / std::filesystem::path{ "textures" } / textureFilePath.filename();
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

	constexpr uint32_t MaxNumBoneWeights{ 4 };
	constexpr uint32_t MaxNumBones{ 100 }; // I think maybe the number of bones are not more than 100

	m_boneOffsets.reserve(MaxNumBones);

	std::vector<std::array<uint32_t, MaxNumBoneWeights>> v_boneIndices;
	v_boneIndices.resize(accNumVertices);

	std::vector<std::array<float, MaxNumBoneWeights>> v_weights;
	v_weights.resize(accNumVertices);

	

	for (UINT i{}; i < numMeshes; i++)
	{
		auto* pMesh = pScene->mMeshes[i];

		const UINT numBones = pMesh->mNumBones;
		for (UINT j{}; j < numBones; j++)
		{
			auto* pBone = pMesh->mBones[j];

			//NOTE: mNode := current node of current bone
			//NOTE: mArmature := parent of mNode
			uint32_t currentBoneIndex{};
			if (m_NodeNameToBoneIndex.contains(pBone->mName.C_Str()))
			{
				currentBoneIndex = m_NodeNameToBoneIndex[pBone->mName.C_Str()];
			}
			else
			{
				XMMATRIX boneOffset = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&pBone->mOffsetMatrix));
				m_boneOffsets.push_back(boneOffset);

				currentBoneIndex = (uint32_t)m_boneOffsets.size() - 1;
				m_NodeNameToBoneIndex[pBone->mName.C_Str()] = currentBoneIndex;
			}
			const UINT numWeights = pBone->mNumWeights;
			for (UINT k{}; k < numWeights; k++)
			{
				const auto weightInfo = pBone->mWeights[k];

				const uint32_t affectedVertexId = weightInfo.mVertexId + m_meshInfos[i].BaseVertexLocation;
				const float weight = weightInfo.mWeight;

				for (int l{}; l < MaxNumBoneWeights; l++)
				{
					if (v_weights[affectedVertexId][l] == 0)
					{
						v_boneIndices[affectedVertexId][l] = currentBoneIndex;
						v_weights[affectedVertexId][l] = weight;
						break;
					}
				}
			}
		}
	}

	int numAnimations = pScene->mNumAnimations;
	m_AnimNodeTransforms.resize(numAnimations);
	for (int i{}; i < numAnimations; i++)
	{
		auto* pAnim = pScene->mAnimations[i];
		const double duration = pAnim->mDuration;
		const double ticksPerSecond = pAnim->mTicksPerSecond;
		const uint32_t numChannels = pAnim->mNumChannels;
		for (uint32_t j{}; j < numChannels; j++)
		{
			aiNodeAnim* channel = pAnim->mChannels[j];
			aiString nodeName = channel->mNodeName;

			const uint32_t numKeys = channel->mNumPositionKeys;
			for (uint32_t k{}; k < numKeys; k++)
			{
				XMVECTOR vecS = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mScalingKeys[k].mValue));
				XMVECTOR vecT = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mPositionKeys[k].mValue));
				aiQuaternion aiQuat = channel->mRotationKeys[k].mValue;
				XMFLOAT4 xmQuat{ aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w };

				XMMATRIX animNodeTransform = XMMatrixTranspose(
					XMMatrixAffineTransformation(vecS, { 0.0F, 0.0F, 0.0F, 1.0F }, XMLoadFloat4(&xmQuat), vecT));

				m_AnimNodeTransforms[i][nodeName.C_Str()].push_back(animNodeTransform);
			}
		}
	}

	auto GetNodeTransform = [&](std::string nodeName, int animIndex, int key) -> XMMATRIX
	{
		XMMATRIX NodeTransform{};
		if (m_AnimNodeTransforms[animIndex].contains(nodeName))
		{
			NodeTransform = m_AnimNodeTransforms[animIndex][nodeName][key];
		}
		else
		{
			NodeTransform = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&pScene->mRootNode->FindNode(nodeName.c_str())->mTransformation));
		}
		return NodeTransform;
	};

	m_bones.resize(m_boneOffsets.size());
	for (const auto& [nodeName, boneIndex] : m_NodeNameToBoneIndex)
	{
		auto* node = pScene->mRootNode->FindNode(nodeName.c_str());
		XMMATRIX AccNodeTransform = GetNodeTransform(nodeName, 0, 0);

		for (aiNode* currentNode{ node->mParent }; currentNode != nullptr; currentNode = currentNode->mParent)
		{
			AccNodeTransform = GetNodeTransform(currentNode->mName.C_Str(), 0, 0) * AccNodeTransform;
		}
		XMStoreFloat4x4(&m_bones[boneIndex], AccNodeTransform);
	}

	for (uint32_t i{}; i < m_boneOffsets.size(); i++)
	{
		XMStoreFloat4x4(&m_bones[i], XMLoadFloat4x4(&m_bones[i]) * m_boneOffsets[i]);
	}

	// Create Vertex Buffers in SOA pattern
	m_pVBuf_Positions = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_positions.data(), (UINT)v_positions.size() * m_VBuf_Strides[static_cast<int>(VertexElement::POSITION)]);
	m_VBuf_Ptrs[static_cast<int>(VertexElement::POSITION)] = m_pVBuf_Positions.Get();

	if (!v_texCoords.empty())
	{
		m_pVBuf_TexCoords = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_texCoords.data(), (UINT)v_texCoords.size() * m_VBuf_Strides[static_cast<int>(VertexElement::TEXCOORD)]);
		m_VBuf_Ptrs[static_cast<int>(VertexElement::TEXCOORD)] = m_pVBuf_TexCoords.Get();
	}
	if (!v_normals.empty())
	{
		m_pVBuf_Normals = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_normals.data(), (UINT)v_normals.size() * m_VBuf_Strides[static_cast<int>(VertexElement::NORMAL)]);
		m_VBuf_Ptrs[static_cast<int>(VertexElement::NORMAL)] = m_pVBuf_Normals.Get();
	}
	if (!v_boneIndices.empty())
	{
		m_pVBuf_BlendIndices = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_boneIndices.data(), (UINT)v_boneIndices.size() * m_VBuf_Strides[static_cast<int>(VertexElement::BLENDINDICES)]);
		m_VBuf_Ptrs[static_cast<int>(VertexElement::BLENDINDICES)] = m_pVBuf_BlendIndices.Get();
	}
	if (!v_weights.empty())
	{
		m_pVBuf_BlendWeight = KhanDx::CreateVertexBuffer(m_pDevice.Get(), v_weights.data(), (UINT)v_weights.size() * m_VBuf_Strides[static_cast<int>(VertexElement::BLENDWEIGHT)]);
		m_VBuf_Ptrs[static_cast<int>(VertexElement::BLENDWEIGHT)] = m_pVBuf_BlendWeight.Get();
	}

	// Create Index buffer
	m_pIndexBuffer = KhanDx::CreateIndexBuffer(m_pDevice.Get(), m_indices.data(), (uint32_t)m_indices.size() * sizeof(m_indices[0]));

	// Create Pixel shader
	m_pPixelShader = KhanDx::CreatePixelShader(m_pDevice.Get(), "SkeletalMesh_PS");

	// Create Vertex shader and Input layout
	ComPtr<ID3DBlob> pBlob = KhanDx::CreateShaderBlob("SkeletalMesh_VS");
	m_pVertexShader = KhanDx::CreateVertexShader(m_pDevice.Get(), pBlob.Get());
	m_pInputLayout = KhanDx::CreateInputLayout(m_pDevice.Get(), pBlob.Get(), m_elementDescs, NUM_VERTEX_ELEMENTS);

	// Create Constant bufferes
	m_pCBuf_VS_Worlds = KhanDx::CreateDynConstBuf<XMFLOAT4X4>(m_pDevice.Get(), 1000);
	m_pCBuf_VS_ViewProjection = KhanDx::CreateDynConstBuf<XMFLOAT4X4>(m_pDevice.Get(), 2);
	m_pCBuf_VS_Bones = KhanDx::CreateDynConstBuf<XMFLOAT4X4>(m_pDevice.Get(), 1000);

	// Save the pointers of Constant buffers, it is used to VSSetConstantBuffers()
	m_CBuf_VS_Ptrs.push_back(m_pCBuf_VS_Worlds.Get());
	m_CBuf_VS_Ptrs.push_back(m_pCBuf_VS_ViewProjection.Get());
	m_CBuf_VS_Ptrs.push_back(m_pCBuf_VS_Bones.Get());

	// if need structured buffer
	//m_pVSDynStructBuf = KhanDx::CreateDynStructBuf<DirectX::XMFLOAT4X4>(m_pDevice, m_numInstance);
	//m_pSRV = KhanDx::CreateSRV_StructBuf(m_pDevice, m_pVSDynStructBuf, 0U, m_numInstance);

	m_pBlendState = nullptr; // blend off
	m_pRasterizerState = KhanDx::CreateRasterizerState_Solid(m_pDevice.Get());
	m_pDepthStencilState = KhanDx::CreateDepthStencilState_Default(m_pDevice.Get());
	m_pSamplerState = KhanDx::CreateSamplerState_Basic(m_pDevice.Get());
}

void KhanRender::SkeletalMeshRenderer::Update(std::vector<DirectX::XMMATRIX> const& worldMats, DirectX::XMMATRIX const& viewProjMat, float debugScalar)
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
	XMFLOAT4X4 TransposedViewProjMat[2]{};
	XMStoreFloat4x4(&TransposedViewProjMat[0], XMMatrixTranspose(viewProjMat));

	// for DebugScalar
	TransposedViewProjMat[1]._11 = debugScalar;

	mappedResource = {};
	m_pDeviceContext->Map(m_pCBuf_VS_ViewProjection.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, &TransposedViewProjMat, sizeof(XMFLOAT4X4) * 2);
	m_pDeviceContext->Unmap(m_pCBuf_VS_ViewProjection.Get(), 0);

	// Bone Matrix Update, maybe seperate AnimUpdate function is needed
	// TODO
	mappedResource = {};
	m_pDeviceContext->Map(m_pCBuf_VS_Bones.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	::memcpy(mappedResource.pData, m_bones.data(), sizeof(XMFLOAT4X4) * m_bones.size());
	m_pDeviceContext->Unmap(m_pCBuf_VS_Bones.Get(), 0);
}

void KhanRender::SkeletalMeshRenderer::Render()
{
	m_pDeviceContext->IASetVertexBuffers(0, NUM_VERTEX_ELEMENTS, m_VBuf_Ptrs, m_VBuf_Strides, m_VBuf_Offsets);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, static_cast<uint32_t>(m_CBuf_VS_Ptrs.size()), m_CBuf_VS_Ptrs.data());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	m_pDeviceContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);

	for (auto& meshInfo : m_meshInfos)
	{
		m_pDeviceContext->PSSetShaderResources(0, 1, meshInfo.m_pSRV.GetAddressOf());
		m_pDeviceContext->DrawIndexedInstanced(meshInfo.NumIndices, m_numInstance, meshInfo.StartIndexLocation, meshInfo.BaseVertexLocation, 0);
	}
}
