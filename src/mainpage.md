@mainpage Resources 
 - @subpage PhotoshopForUsers
 - @subpage MedimageForUsers
 - @subpage ConvertingAndSegmentingCTScan
 - @subpage DesignAndCodingStyleGuide



@page PhotoshopForUsers Getting Started with %PhotoShop

  @section Brushes
   - <b>%Pen:</b>&nbsp;This tool draws a line like a pen.
   - <b>Calligraphy %Pen:</b>&nbsp;This tool draws a line like a calligraphy pen.
   - <b>Highlighter:</b>&nbsp;This tool draws a thick line like a highlighter marker.
   - <b>Spray Can:</b>&nbsp;This tool is designed to mimic the look of paint from a spray can.
   - <b>%Eraser:</b>&nbsp;This tool erases parts of an image.
   - <b>Colorful Dust:</b>&nbsp;This tool is like a large "plus" shaped pen which generates random colors.
   - <b>Rubber Stamp:</b>&nbsp;This tool takes an image loaded from a file and uses it as a stamp that can be placed anywhere on the canvas.
   - <b>%Blur Tool:</b>&nbsp;This tool unfocuses image areas. 
  
  @section filters Image Filters
   - <b>%Blur:</b>&nbsp;It blurs the image proportional to the amount specified in the input.
   - <b>Sharpen:</b>&nbsp;It sharpens the image proportional to the amount specified in the input.
   - <b>Edge Detection:</b>&nbsp;It creates a representation of the rate-of-change in the image.
   - <b>Motion %Blur:</b>&nbsp;It blurs the image in a directional manner.
   - <b>Threshold:</b>&nbsp;This filter transforms the current layer or the selection into a black and white image, where white pixels represent the pixels of the image whose Value is in the threshold range, and black pixels represent pixels with Value out of the threshold range.
   - <b>Adjust saturation:</b>&nbsp;This filter either increase, decrease, or invert the colorfulness of your image.
   - <b>Adjust R,G,B levels:</b>&nbsp;This filter increase or decrease the intensity of each color channel.
   - <b>Quantize:</b>&nbsp;It reduce the number of possible colors by binning each color value into the the number of bins specified.
   - <b>%Special %Filter:</b>&nbsp;This filter makes an image looks likes a pencil drawing with color.
  
 

@page MedimageForUsers Getting Started with MedImage
<h3>Loading images and annotating them</h3>
<p>From the %Image I/O Controls Box located on the right side, navigate to the directory your images are located and double click on an image. Next, click the "Load Canvas" button. The image will now be loaded into the canvas. </p>

<p>Next, you can click on the specific tools / filters you want to use to
annotate the image.</p>


- <b>Tool Type:</b>
 - %Pen:<br>
 &nbsp;&nbsp;This will let you use a pen to draw or mark red lines on top of the loaded image.
 - Arrow:<br>
 &nbsp;&nbsp;This will let you stamp a red cross arrow, circling important location on the loaded image.</p>
 
- <b>Filters:</b><br>
 &nbsp;&nbsp;Adjust the "Amount" input box to specify how much sharpen is needed.
- <b>Edge Detect:</b><br>
 &nbsp;&nbsp;This feature will make all edges become more visible.
- <b>Threshold:</b><br>
 &nbsp;&nbsp;This feature will adjust the loaded image base on a grayscale range.
- <b>Multiply RGB:</b><br>
 &nbsp;&nbsp;This feature will addjust the each of the color (Red, Green, Blue) of the loaded image base on the values input into the boxes.
- <b>Quantize:</b><br>
 &nbsp;&nbsp;Theis feature will simplify the colors of the image based on the number of bins given.
- <b>Grayscale:</b><br>
 &nbsp;&nbsp;This feature will set the saturation of the image to become black and white.

- <b>Saving image:</b><br>
 &nbsp;&nbsp;After you are done editing an image, in the image I/O Controls box, you will see an "Image" text box where you can insert text. Specify the new name of the image you want to save and press enter, then click "Save Canvas".

</p>




@page ConvertingAndSegmentingCTScan Converting and Segmenting a CT Scan Using MedImage 

<h3>How to convert an image stack saved as a set of numbered images in a directory to a quantized version that has 4 tissue types?</h3>

<p>The approach for this should be to first slightly blur the image (to reduce noise), then quantize the result into 4 segments, then save the resulting images to a new directory.</p>

<p>Here is how to do it:</p>

 - <b>The easiest way:</b><br><br>
   The easiest way would be to use a command line command. If you are unfamiliar with what's this is, please read (or skim) the link below before proceed.
   
   <p>http://en.wikipedia.org/wiki/Command-line_interface</P>
    
   <p>Assume that you already know how to open a command line interface. In Linux and Mac OS, it is called "terminal" and in windows it is called "cmd".</p>

   <p>In the terminal windows, navigate to where the program is located. You can use the command "cd" which means "change directory" to do so:<br>
   - cd <medimage executable location>
	
   - Do not put the arrow bracket (< and >) in when you execute any command.</p><br>
	
   <p>After that, you can use the following to generate the desired result:<br>
	
	&nbsp;&nbsp;&nbsp;&nbsp;./medimage <image name>###.<extension> -blur <amount> -quantize <amount> <output image name>###.<new extension>
   - image name is the name of an image
   - extension is the sub string after the last dot (.) of the file name, in this case it'll be either "png" or "jpg".
   - -blur <amount> is the filter options specified by the amount.
   - ### is the pattern to recognize all number with the pattern 000 to 999</p>
  <p>This should finish the work after a few seconds even if the number of images are up in the hundreds.</p>


 - <b>The harder way:</b><br><br>
   Since the MedImage GUI application doesn't have a blurring option, you'll need to use the Photoshop application to first blur the images, saving everything in a directory. Then, switch over to MedImage.<br><br>
   MedImage GUI has the "next" and "prev" buttons which will let you load the next and previous images to the canvas quickly This will make it easier for you to apply the quantize filter on each of the image you want to save.<br><br>
   This method will take a long time if you have a stack of hundreds of images.
	

@page DesignAndCodingStyleGuide Design and Coding Style Guide

 - @subpage ProjectDesign
 - @subpage AddingNewBrush
 - @subpage AddingNewFilter
 - @subpage CodingStyleAndDocumentation
 - @subpage ProgrammersBlog 


@page ProjectDesign Project Design

<h3>The overall design of our project is as follow:</h3>
<p>
Because both applications share the use of image editing tools and the set of kernel-based filters, we decided to move all subclasses of the "Tools" class and "Filter" class into a library called "libphoto". This libphoto library also contains the glui class, BaseGfxApps, and %PixelBuffer since these three classes are required for the GUI designs and color processing.
</p>
<p>
The Photoshop application is now updated with 4 new public methods that allows another class to access shared contents. This makes it easier to write test programs.
</p>
<p>
The MedImage application is actually further divided into two new classes. %MedImageGUI is a subclass of BaseGfxApps, which is a GUI version. It only supports a small set of filters (sharpen, edge detect, multiply RGB, quantize, and gray scale).
</p>
<p>
%MedImageCML is an independent class of its own. Even though it shares many functionality with the GUI version,
however there are unnecessary inheritance requirements and method calls from
</p>
<p>
BaseGfxApps if it is to be inherited from this class, or to be inherited from %MedImageGUI.
</p>
<p>
Therefore, %MedImageCML is a new class optimized only for terminal command line operations.
</p>
<p>
It further supports 4 more features that the GUI version doesn't: blur filter, saturate filter, the compare command option, and
sequence processing command option.</p>
<p>
It is unsafe to modify %BaseGfxApp to contain an empty constructor for the effect of suppressing %GLUI requirements. This technique may be needed if a programmer wants to utilize the common codes between %MedImageGUI and %MedImageCML and decides to have only one MedImage that supports both the command line and GUI implementations. It is unsafe because if some functions in the command line implementation trying to modify parts of the GUI declared member variables (which belongs to %BaseGfxApp), the result is an segmentation fault!</p>
<p>
To make it easier for Photoshop to be used for testing, we actually utilize the above technique. However, we have modified the public methods to only access member variables declared within Photoshop (doesn't belong to %BaseGfxApp and Glui). This protected our implementation from the above error.
</p>

Below is a UML Diagram of our projects:

 - Class reference and diagrams for @ref PhotoShop

 - Class reference and diagrams for @ref MedImageCML


@page AddingNewBrush Adding a New Brush
<h3>A tutorial on how to add a new brush to Photoshop:</h3>
1. Navigate to the _libphoto/include_ directory to create a new header .h file for your new brush. The .cpp file can be created in the  _libphoto/src_ directory.<br>
2. Start with the new header file. Let's say you want to create a new brush called “the plus brush”, which creates a path of plus signs after mouseDragged is called. You should modify the header file as follow:

 	#ifndef PLUS_H

 	#define PLUS_H

 	#include "Tools.h"

 class PlusBrush : public %Tools 

 {

 public:

	// constructor:

	PlusBrush();

	~PlusBrush();

};

 #endif


The statements
after a “#” character are
for the the complier
to not keep including the
header multiple times. Since
a “plus brush” is a %tool, it belongs to the Tools class, which is
specified by the declaration:

	class PlusBrush : public Tools

1. 
	Note
	that %Tools.h has the following member variables:<br><br>
	float** m_mask<br> 
	int m_width; 

We have to tell the PlusBrush.cpp's constructor to modify these
variables.

Below is a version of PlusBrush.cpp:

	<div class="code"><pre>
#include "PlusBrush.h"

PlusBrush::PlusBrush(){

	m_width = 15;

	m_mask = new float*[m_width];

	for(int i = 0; i &lt; m_width; i++){

		m_mask[i] = new float[m_width];

	}	

	for(int i = 0; i &lt; m_width; i++) {

		for (int j = 0; j&lt;m_width; j++){

			m_mask[i][j] = 0;

		}

	}

// Some for loops here to cut 4 corners of a rectangle so that it
becomes a plus sign

}

PlusBrush::~PlusBrush(){}

</pre></div>

Note that the example is incomplete. The constructor fist assignes
the width as 15, then it creates a 15x15 of floating point values.
This values is the alpha color (0 means completely transparent, 1
means  completely shown).  Depending on the functionality of your 
brush, you may want to have different transparency. As of for the
Plush Brush example, the desired effect can be achieved by setting
the alpha values in the four corners of the mask becomes 0. This is
left for you to implement.





@page AddingNewFilter Adding a New %Filter

There are 2 kind of Filters that you can create. The filters that is
not based on kernel convolution and the ones that do.

Without Kernel Convolution:

To add a new filter that is not based on the kernel convolution,
creates a new method declaration in PhotoShop.h and its specification
in PhotoShop.cpp.

For example, if you want to create a new filter called “invert
filter”, that invert every pixel's color data. In other word, if
“r” is the value for red, “g” is the value for green, and “b”
is the value for blue; the invert of red is 1.0 – r, the invert of
green is 1.0 -g and the invert of blue is 1.0 – b.

In photoshop.h, you should declare the arguments needed for your
filter, in this case you don't need an argument for the invert
filter.

In photoshop.cpp, you will need to write the implementation of the
method declared. Which is simply iterating through the m_pixelBuffer,
getting its current pixel, modifying its values as the rule mentioned
above and then update it back using methods in PhotoShop.cpp.

With Kernel Convolution:

 Similar to adding a new Brush tool, for the kernel-based filters,
you will need to declare a subclass for the filter class, which then
can be used to create a 2D kernel for your effect. Kernel Convolution
is simply a 2D floating point array that you will use to sweep over
the entire canvas, getting all neighbor color data (within the range
of the kernel) to calculate the resulted effects.


@page CodingStyleAndDocumentation Coding Style and Documentation

@page ProgrammersBlog Programmers' Blog

  @section blog1 Xiaoxun Meng's Blog

The following code is to implement blur tool. For the blur tool, I hardcoded the width of the blur tool to 9 by setting `int kwidth = 9`.
Then, I created a special filter which is blur filter and got the kernel I will use it later. `x` and `y` is the position that the user
wants to blur. So, in the `for` loops, I applied the kernel for each point in a circle area around the position. Each point has green,
blue, and red colors, so I applied the kernel for each color of a point. After that, `m_displayBuffer->setPixel(i,j, currentPixel);` is
to set the modified pixel back to its position.
<pre>
<code>
if(m_curTool == 7){
		int kwidth = 9;
		%Filter *filter = new %Blur_Filter(kwidth);

		float** kernel = filter->getKernel();
		PixelBuffer* bucket = new %PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
		m_displayBuffer->copyPixelBuffer(m_displayBuffer, bucket);
		int height = m_displayBuffer->getHeight();
		y = height - y -1;
		%ColorData currentPixel;
		float red = 0;
		float green = 0;
		float blue = 0;
		int radius = 20;

		for(int i = x-radius; i <= x+radius; i++){
			for(int j = y-radius; j <= y+radius; j++){
				if((i-x)*(i-x)+(j-y)*(j-y) < radius*radius){
					if(i < m_displayBuffer->getWidth() && i >= 0 && j < m_displayBuffer->getHeight() && j >= 0){
						for(int k = -kwidth/2; k <= kwidth/2; k++){
							for(int l = -kwidth/2; l <= kwidth/2; l++){
								if(i+k < m_displayBuffer->getWidth() && i+k >= 0 && j+l < m_displayBuffer->getHeight() && j+l >= 0){
									currentPixel = bucket->getPixel(i+k,j+l);
									red += (currentPixel.getRed() * kernel[kwidth/2+k][kwidth/2+l]);
									green += (currentPixel.getGreen() * kernel[kwidth/2+k][kwidth/2+l]);
									blue += (currentPixel.getBlue() * kernel[kwidth/2+k][kwidth/2+l]);
								}
							}
						}
						currentPixel.setRed(red);
						currentPixel.setGreen(green);
						currentPixel.setBlue(blue);
						m_displayBuffer->setPixel(i,j, currentPixel);
						red = 0;
						green = 0;
						blue = 0;
					}
				}
			}
		}
	}
</code>
</pre>	


