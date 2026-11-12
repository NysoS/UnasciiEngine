#include "Exar/Semaphore.hpp"

Exar::Semaphore_::Semaphore_()
	: mSemaphore(0)
{}

Exar::Semaphore_::~Semaphore_()
{}

void Exar::Semaphore_::acquire()
{
	mSemaphore.acquire();
}

void Exar::Semaphore_::release()
{
	mSemaphore.release();
}
