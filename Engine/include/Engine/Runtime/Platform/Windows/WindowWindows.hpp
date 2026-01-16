#ifndef WINDOW_WINDOWS_HPP
#define WINDOW_WINDOWS_HPP

#include "Engine/Runtime/Platform/Window.hpp"

namespace UnasciiEngine {
	class UAE_API WindowWindows : public Window
	{
	public:
		explicit WindowWindows(const WindowInfo& pWinInfo);
		virtual ~WindowWindows();

		WindowWindows(WindowWindows&&) = delete;
		WindowWindows& operator=(const WindowWindows&) = delete;

		virtual bool init() override;
		virtual void render() override;

	private:
		WindowInfo mWinInfo;
		unsigned short getForegroundColor(ForegroundColor lColor);
		void* mOutHandle;
	};
}



#endif // WINDOW_WINDOWS_HPP
