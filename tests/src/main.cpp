/**********************************************************************
 * @file main.cpp
 * @brief A generated test program for the photoshop interactive tools
 * 
 * This test program will generate a set of test images and compare
 * them against a directory of correct images.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "PhotoShop.h"
#include "ColorData.h"
#include "MedImageCML.h"

int main(int argc, char* argv[])
{	
	

	PhotoShop *app = new PhotoShop(argc, argv);
	std::string out;
	
	// Pen test
	out = "./generated/pen.png";
	app->setCurTool(0); // pen
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);

	
	out = "./generated/eraser.png";
	app->setCurTool(1); // eraser
	// this will delete all points marked by the pen
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
	
	
	out = "./generated/spraycan.png";
	app->setCurTool(2); // spraycan
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
	app->resetCanvas();
	
	
	out = "./generated/calpen.png";
	app->setCurTool(3); // cal pen
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
	app->resetCanvas();
	
	
	
	
	out = "./generated/highlight.png";
	app->setCurTool(4); // highlight
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
	app->resetCanvas();
	
	
	out = "./generated/special.png";
	app->setCurTool(5); // special
	
	for(int i = 0; i < 800; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
//	app->resetCanvas();
	
/*	
	out = "./generated/stamp.png";
	app->setCurTool(6); // stamp
	app->leftMouseDown(97, 55);
	app->leftMouseDown(99, 57);
	app->leftMouseDown(101, 59);
	app->leftMouseDown(103, 61);
	app->saveCanvas(out);
*/
	
	
	out = "./generated/blur.png";
	app->setCurTool(7); // blur
	for(int i = 0; i < 400; i++){
		app->leftMouseDown(i, i);
	}
	app->saveCanvas(out);
	app->resetCanvas();
	

	exit(0);
}
