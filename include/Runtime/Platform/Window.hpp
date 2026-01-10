#ifndef WINDOW_HPP
#define WINDOW_HPP

enum class ForegroundColor {
	Blue,
	Green,
	Red,
	White
};

struct WindowInfo
{
	int mWidth = 800;
	int mHeight = 600;
	int fontSize = 10;
	ForegroundColor foregroundColor = ForegroundColor::White;
};

class Window {
public:
	Window() = default;
	virtual ~Window() {};

	virtual bool init(const WindowInfo& lWinInfo) = 0;

	static Window* Create(const WindowInfo& lWinInfo = WindowInfo());
};

#endif // !WINDOW_HPP
