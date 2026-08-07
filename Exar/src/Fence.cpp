#include "Exar/Fence.hpp"

Exar::Fence_::Fence_()
{
}

Exar::Fence_::~Fence_()
{
}

Exar::FenceState Exar::Fence_::getCurrentState() const noexcept
{
	std::scoped_lock<std::mutex> lLock(mMutex);
	return mState;
}

void Exar::Fence_::setCurrentState(Exar::FenceState pState) noexcept
{
	std::scoped_lock<std::mutex> lLock(mMutex);
	mState = pState;
}

void Exar::Fence_::notifyOne() noexcept
{
	mCv.notify_one();
}

void Exar::Fence_::notifyAll() noexcept
{
	mCv.notify_all();
}

void Exar::Fence_::wait() noexcept
{
	std::unique_lock<std::mutex> lLock(mMutex);
	mCv.wait(lLock, [&] {
		return mState == FenceState::Signaled;
	});
}
