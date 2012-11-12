/*
*	main.cpp
*
*	Created on: 2012-10-31
*	Author: Janic Duplessis
*/

#include "define.h"
#include "Chunk.h"
#include <iostream>

int main()
{
	int * test20 = new int[20];
	test20[19] = NULL;
	int i = 0;
	while(test20 != NULL) {
		std::cout << *test20 << "    " << i << std::endl;
		test20++;
		i++;
	}


	Chunk c;
	c.GetBloc(9,9,9);
	c.GetBloc(0,0,0);
	BlocInfo bi(c.GetBloc(3,7,1), "TEST");
	bi.SetDurabilite(5);
	bi.Afficher();
	Chunk d = c;
	c.SetBloc(1,5,3,BTYPE_GRASS);
	d.SetBloc(1,5,3,BTYPE_DIRT);
	BlocInfo bi2(d.GetBloc(1,5,3), "test copie");
	BlocInfo bi3(c.GetBloc(1,5,3), "test copie");
	bi2.Afficher();
	bi3.Afficher();

	BlocArray3d test(5,5,5);
	BlocArray3d test2 = test;

	
	system("pause");
	return 0;
}