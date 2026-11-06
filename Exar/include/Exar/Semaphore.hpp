#ifndef EXAR_SEMAPHORE_HPP
#define EXAR_SEMAPHORE_HPP

#include "Exar/exarpch.h"

namespace Exar {
	class Semaphore_
	{
	public:
		Semaphore_();
		~Semaphore_();

		Semaphore_(const Semaphore_&) = delete;
		Semaphore_& operator=(const Semaphore_&) = delete;
		Semaphore_(Semaphore_&&) = delete;
		Semaphore_& operator=(Semaphore_&&) = delete;

		void acquire();
		void release();

	private:
		std::binary_semaphore mSemaphore;
	};
}

#endif // !EXAR_SEMAPHORE_HPP
