#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>

class Window;

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	std::unique_ptr<Window> mWindow;
};

#endif // !APPLICATION_HPP