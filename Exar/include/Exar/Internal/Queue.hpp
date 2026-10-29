#ifndef EXAR_QUEUE_HPP
#define EXAR_QUEUE_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar
{
	class Queue_
	{
	public:
		Queue_();
		~Queue_();

		Queue_(const Queue_&) = delete;
		Queue_(Queue_&) = delete;
		Queue_& operator=(const Queue_&) = delete;

		void runProc(std::function<void(std::stop_token)> pProc);

		const QueueFamily family;
	private:
		std::counting_semaphore<MAX_FRAME_IN_FLIGHT> mJob{0};
		std::jthread mRunner;

		friend Device_;
	};
}

#endif // !EXAR_QUEUE_HPP
