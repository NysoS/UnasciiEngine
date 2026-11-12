#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Engine/Engine.hpp"
#include "Engine/Core/Types.hpp"

namespace UnasciiEngine {
	enum class UAE_API ForegroundColor {
		Blue,
		Green,
		Red,
		White
	};

	struct UAE_API WindowInfo
	{
		StringC mTitle = "Unascii Termminal";
		u32 mWidth = 800;
		u32 mHeight = 600;
		u32 fontSize = 10;
		ForegroundColor foregroundColor = ForegroundColor::White;
	};

	class UAE_API Window {
	public:
		Window() = default;
		virtual ~Window() {};

		virtual bool init() = 0;
		virtual void render() = 0;

		static Window* Create(const WindowInfo& pWinInfo = WindowInfo());
	};

}
#endif // !WINDOW_HPP
