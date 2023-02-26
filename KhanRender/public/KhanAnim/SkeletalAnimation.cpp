#include "pch.h"
#include "SkeletalAnimation.h"

#include <KhanTools/Log.h>
// Additional dependencies
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

KhanAnim::SkeletalAnimation::SkeletalAnimation(std::filesystem::path filePath)
{
	using namespace DirectX;
	Assimp::Importer importer;
	
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* pScene = importer.ReadFile(filePath.string(), aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
	if (nullptr == pScene) {
		KHAN_ERROR(importer.GetErrorString());
		throw std::exception{ "Failed to import file using assimp" };
	}

	if (!pScene->HasAnimations())
	{
		KHAN_ERROR("This file has not animation");
		throw std::exception{ "This file has no animation" };
	}

	// assume the number of animation is only one // unsigned int numAnim = pScene->mNumAnimations; 
	
	const aiAnimation* anim = pScene->mAnimations[0];
	m_Duration = static_cast<unsigned int>(anim->mDuration + 0.0001/*prevent something like 0.9999 to 0 */);
	m_TicksPerSecond = anim->mTicksPerSecond == 0.0 ? 30.0F : static_cast<float>(anim->mTicksPerSecond);
	for (unsigned int i{}; i < anim->mNumChannels; i++)
	{
		const aiNodeAnim* channel = anim->mChannels[i];
		const std::string nodeName = channel->mNodeName.C_Str();
		const unsigned int numKeys = channel->mNumPositionKeys;
		// assimp bug: aiAnimBehaviours are not correct, always these are DEFAULT, but sometimes need to be CONSTANT or REPEAT
		
		m_AnimNodeTransforms[nodeName] = {};
		m_AnimNodeTransforms[nodeName].reserve(m_Duration);

		// bake animation, if there is only one key, take same pose for all keys
		if (numKeys == 1)
		{
			XMVECTOR scale = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mScalingKeys[0].mValue));
			XMVECTOR position = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mPositionKeys[0].mValue));
			aiQuaternion aiQuat = channel->mRotationKeys[0].mValue;
			XMFLOAT4 xmQuat{ aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w };
			XMVECTOR rotation = XMLoadFloat4(&xmQuat);

			const XMMATRIX animNodeTransform = XMMatrixTranspose(
				XMMatrixAffineTransformation(scale, { 0.0F, 0.0F, 0.0F, 1.0F }, rotation, position));
			//m_AnimNodeTransforms[nodeName].push_back(animNodeTransform);
			m_AnimNodeTransforms[nodeName].resize(m_Duration, animNodeTransform);
			continue;
		}
		
		// bake animation, with interpolation
		unsigned int key{};
		while (key < numKeys && m_AnimNodeTransforms[nodeName].size() < m_Duration)
		{
			assert(channel->mPositionKeys[key].mTime == channel->mRotationKeys[key].mTime && channel->mPositionKeys[key].mTime == channel->mScalingKeys[key].mTime && "Is it possible?");

			XMVECTOR scale = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mScalingKeys[key].mValue));
			XMVECTOR position = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mPositionKeys[key].mValue));
			aiQuaternion aiQuat = channel->mRotationKeys[key].mValue;
			XMFLOAT4 xmQuat{ aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w };
			XMVECTOR rotation = XMLoadFloat4(&xmQuat);

			int time = static_cast<int>(channel->mPositionKeys[key].mTime + 0.0001/*prevent something like 0.9999 to 0 */);
			int targetTime = static_cast<int>(m_AnimNodeTransforms[nodeName].size());

			if (targetTime == time)
			{
				key++;
			}
			else /* Need to interpolate */
			{
				assert(key != 0 && "The first key cant be here");
				unsigned int prevKey{ key - 1 };
				int prevTime = static_cast<int>(channel->mScalingKeys[prevKey].mTime + 0.0001/*prevent something like 0.9999 to 0 */);

				XMVECTOR prevS = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mScalingKeys[prevKey].mValue));
				XMVECTOR prevT = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&channel->mPositionKeys[prevKey].mValue));
				aiQuat = channel->mRotationKeys[prevKey].mValue;
				XMFLOAT4 prevQuat{ aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w };

				float timeWeight = float(targetTime - prevTime) / (time - prevTime);
				scale = XMVectorLerp(prevS, scale, timeWeight);
				position = XMVectorLerp(prevT, position, timeWeight);
				rotation = XMQuaternionSlerp(XMLoadFloat4(&prevQuat), rotation, timeWeight);
			}

			XMMATRIX animNodeTransform = XMMatrixTranspose(
				XMMatrixAffineTransformation(scale, { 0.0F, 0.0F, 0.0F, 1.0F }, rotation, position));
			m_AnimNodeTransforms[nodeName].push_back(animNodeTransform);
		}
	}
}

KhanAnim::SkeletalAnimation::~SkeletalAnimation() noexcept
{
}
