#pragma once
// standard libraries
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <filesystem>

// additional dependencies
#include <directxmath.h>

namespace KhanAnim
{
	class SkeletalAnimation
	{
	public:
		SkeletalAnimation(std::filesystem::path filePath);
		~SkeletalAnimation() noexcept;
	
		unsigned int m_Duration{}; // The number of total ticks
		double m_TicksPerSecond{};
		std::unordered_map<std::string, std::vector<DirectX::XMMATRIX>> m_AnimNodeTransforms;
	};
}