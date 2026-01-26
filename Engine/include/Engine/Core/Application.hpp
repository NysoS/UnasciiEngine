#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Engine/Engine.hpp"
#include "Engine/Runtime/Platform/Window.hpp"

#include "Engine/uaepch.h"

class Window;

namespace UnasciiEngine {
	class UAE_API Application
	{
	public:
		Application(const WindowInfo& mWinInfo);
		~Application();

		bool init();
		void run();

		inline Window& getWindow() { return *mWindow; }

	private:
		std::unique_ptr<Window> mWindow;
		bool mIsRunning;

		static constexpr float MaxFPS = 1.f / 60.f;
	};
}

#endif // !APPLICATION_HPP