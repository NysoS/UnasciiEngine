#ifndef EXAR_FENCE_HPP
#define EXAR_FENCE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"

namespace Exar
{
	enum class FenceState
	{
		Signaled,
		Waiting
	};

	class Fence_ 
	{
	public:
		Fence_();
		~Fence_();

		Fence_(const Fence_&) = delete;
		Fence_(Fence_&) = delete;
		Fence_& operator=(const Fence_&) = delete;

		FenceState getCurrentState() const noexcept;
		void setCurrentState(FenceState pState) noexcept;
		void notifyOne() noexcept;
		void notifyAll() noexcept;

		void wait() noexcept;

	private:
		mutable std::mutex mMutex;
		std::condition_variable mCv;
		FenceState mState = FenceState::Signaled;
	};
}

#endif // !EXAR_FENCE_HPP
