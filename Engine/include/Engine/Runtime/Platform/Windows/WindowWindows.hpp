#ifndef WINDOW_WINDOWS_HPP
#define WINDOW_WINDOWS_HPP

#include "Engine/Runtime/Platform/Window.hpp"

namespace UnasciiEngine {
	class UAE_API WindowWindows : public Window
	{
	public:
		WindowWindows(const WindowInfo& lWinInfo);
		virtual ~WindowWindows();

		virtual bool init(const WindowInfo& lWinInfo) override;

	private:
		unsigned short getForegroundColor(ForegroundColor lColor);
		void* mOutHandle;
	};
}



#endif // WINDOW_WINDOWS_HPP
