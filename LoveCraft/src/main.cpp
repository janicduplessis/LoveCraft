#include <SFML/Window.hpp>
#include <iostream>
#include "engine/engine.h"
#include "engine/son.h"
#include "util/tool.h"



int main()
{
    Tool::CheckTypes();

    Engine engine;
	Info::Get().Sound().PlayMusic(Son::MUSIC_MENU);
    engine.SetMaxFps(60);
	int resX = GetSystemMetrics(SM_CXSCREEN);
	int resY = GetSystemMetrics(SM_CYSCREEN);
	std::cout << "Screen resolution : " << resX << " x " << resY << std::endl; 
	//FreeConsole();
    engine.Start("LoveCraft", resX, resY, false);
}
