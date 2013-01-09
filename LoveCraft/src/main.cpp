#include <SFML/Window.hpp>
#include <iostream>
#include "engine/engine.h"
#include "util/tool.h"



int main()
{
    Tool::CheckTypes();

    Engine engine;
    engine.SetMaxFps(60);
	int resX = GetSystemMetrics(SM_CXSCREEN);
	int resY = GetSystemMetrics(SM_CYSCREEN);
	std::cout << "Screen resolution : " << resX << " x " << resY << std::endl; 
	Info::Get().Sound().PlayMusic();
	FreeConsole();
    engine.Start("LoveCraft", resX, resY, false);
}
