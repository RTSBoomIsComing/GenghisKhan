#pragma once
namespace Khan
{
	class ITest abstract
	{
	public:
		virtual ~ITest() noexcept = default;
		virtual void Logic() abstract;
	};
}