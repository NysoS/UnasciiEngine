#ifndef WINDOW_WINDOWS_HPP
#define WINDOW_WINDOWS_HPP

#include "Runtime/Platform/Window.hpp"

class WindowWindows : public Window
{
public:
	WindowWindows(const WindowInfo& lWinInfo);
	virtual ~WindowWindows();

	virtual bool init(const WindowInfo& lWinInfo) override;

private:
	unsigned short getForegroundColor(ForegroundColor lColor);
	void* mOutHandle;
};

#endif // WINDOW_WINDOWS_HPP
