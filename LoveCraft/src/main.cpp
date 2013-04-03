#include <SFML/Window.hpp>
#include <iostream>
#include "engine/engine.h"
#include "engine/son.h"
#include "util/tool.h"
#include "util/array.h"



int main()
{
    Tool::CheckTypes();
	// Zone de test
	//Array<int> tab1 = Array<int>(3);
	//Array<int> tab2 = Array<int>(2);
	//tab1.Set(0, 2);
	//tab1.Set(1, 4);
	//tab1.Set(2, 8);
	//tab2.Set(0, 20);
	//tab2.Set(1, 40);
	//tab1.Show();
	//tab2.Show();
	//tab1.Fuse(tab2);
	//tab1.Show();

	//system("pause");
    Engine engine;
	Son::PlayMusic(MUSIC_MENU);
    engine.SetMaxFps(60);
	int resX = GetSystemMetrics(SM_CXSCREEN);
	int resY = GetSystemMetrics(SM_CYSCREEN);
	std::cout << "Screen resolution : " << resX << " x " << resY << std::endl; 
	//FreeConsole();
    engine.Start("LoveCraft", resX, resY, true);
}
