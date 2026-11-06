#include "Exar/Internal/Queue.hpp"

Exar::Queue_::Queue_()
{}

Exar::Queue_::~Queue_()
{
	mRunner.request_stop();
	mRunner.join();
}

void Exar::Queue_::runProc(std::function<void(std::stop_token)> pProc)
{
	mRunner = std::jthread(std::move(pProc));
}
