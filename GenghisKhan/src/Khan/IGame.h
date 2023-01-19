#pragma once

namespace Khan
{
	class IGame abstract
	{
	public:
		virtual ~IGame() noexcept = default;
		virtual void Logic() abstract;
	};
}
