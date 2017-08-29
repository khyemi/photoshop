

/**********************************************************************
 * @file main.cpp
 * @brief Initilizing the GUI of Photoshop application.
 * 
 * See PhotoShop.h for more information.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * *******************************************************************/



#include "PhotoShop.h"
#include "ColorData.h"

int main(int argc, char* argv[])
{
	PhotoShop *app = new PhotoShop(argc, argv,800,800, ColorData(1,1,0.95));
	app->runMainLoop();
	exit(0);
}
