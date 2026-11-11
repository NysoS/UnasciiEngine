#include "Exar/Internal/Queue.hpp"

Exar::Queue_::Queue_()
{}

Exar::Queue_::~Queue_()
{
	mRunner.request_stop();
	mControlWorker.release();
	mRunner.join();
}

void Exar::Queue_::runProc(std::function<void(std::stop_token, Queue)> pProc)
{
	mRunner = std::jthread(std::move(pProc), this);
}

void Exar::Queue_::waitForWork()
{
	mControlWorker.acquire();
}

void Exar::Queue_::notifyWork()
{
	mControlWorker.release();
}
