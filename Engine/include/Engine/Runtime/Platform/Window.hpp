#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Engine/Engine.hpp"

namespace UnasciiEngine {
	enum class UAE_API ForegroundColor {
		Blue,
		Green,
		Red,
		White
	};

	struct UAE_API WindowInfo
	{
		int mWidth = 800;
		int mHeight = 600;
		int fontSize = 10;
		ForegroundColor foregroundColor = ForegroundColor::White;
	};

	class UAE_API Window {
	public:
		Window() = default;
		virtual ~Window() {};

		virtual bool init(const WindowInfo& lWinInfo) = 0;

		static Window* Create(const WindowInfo& lWinInfo = WindowInfo());
	};

}
#endif // !WINDOW_HPP
