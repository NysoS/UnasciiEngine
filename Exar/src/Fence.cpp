#include "Exar/Fence.hpp"

Exar::Fence_::Fence_()
{
}

Exar::Fence_::~Fence_()
{
}

Exar::FenceState Exar::Fence_::getCurrentState() const
{
	std::scoped_lock<std::mutex> lLock(mMutex);
	return mState;
}

void Exar::Fence_::setCurrentState(Exar::FenceState pState)
{
	std::scoped_lock<std::mutex> lLock(mMutex);
	mState = pState;
}

void Exar::Fence_::notifyOne()
{
	mCv.notify_one();
}

void Exar::Fence_::notifyAll()
{
	mCv.notify_all();
}
