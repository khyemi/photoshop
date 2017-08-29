
/**********************************************************************
 * @file PhotoShop.cpp
 * @brief A Gui software editting images.
 * 
 * PhotoShop.cpp:
 * 	Specific implementations of the following:
 * 	Useful features such as image loading/saving
 * 	Tools include pen, calligraphy pen, eraser, spray can, special
 *  dusting effect, and more.
 * 	A set of filters to enhance images.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * *******************************************************************/



#include "PhotoShop.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include <png.h>
#include <jpeglib.h>

#include "ColorData.h"
#include "PixelBuffer.h"
#include "Tools.h"
#include "Pen.h"
#include "Eraser.h"
#include "CalPen.h"
#include "SprayCan.h"
#include "Highlight.h"
#include "Special.h"

#include "Filter.h"
#include "Blur_Filter.h"
#include "MotionBlur_Filter.h"
#include "EdgeDetect_Filter.h"
#include "Sharpen_Filter.h"

#include <cstdlib>
#include <cmath>
#include <stdio.h>


using std::cout;
using std::endl;

PhotoShop::PhotoShop(int argc, char* argv[], int width, int height, ColorData backgroundColor) : BaseGfxApp(argc, argv, width, height, 50, 50, GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH, true, width+51, 50)
{
    // Set the name of the window
    setCaption("PhotoShop");
    // Initialize Interface
    initializeBuffers(backgroundColor, width, height);
    m_list.push_back(m_displayBuffer);
    initGlui();
    initGraphics();
}

PhotoShop::PhotoShop(int argc, char* argv[]) : BaseGfxApp(argc, argv) {
	
	initializeBuffers(ColorData(1,1,0.95), 800, 800);

}

void PhotoShop::display()
{
    drawPixels(0, 0, m_width, m_height, m_displayBuffer->getData());
}

PhotoShop::~PhotoShop()
{
    if (m_displayBuffer) {
        delete m_displayBuffer;
    }
}


/*****************************************************************************
 *							Back up!!!								 *
 *****************************************************************************/

void PhotoShop::backup(){
	while(m_list.size() > (unsigned) m_curPos+1)
		m_list.pop_back();
		
	PixelBuffer * temp = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
	m_displayBuffer->copyPixelBuffer(m_displayBuffer, temp);
	m_curPos++;
	m_list.push_back(temp);
	cout << "m_curPos = " << m_curPos << "size = " << m_list.size() << endl;
	undoEnabled((bool)(m_curPos != 0));
    redoEnabled((bool)( (unsigned) m_curPos != m_list.size()-1));
}



/*****************************************************************************
 * 								Done!!!										 *
 *****************************************************************************/


void PhotoShop::mouseDragged(int x, int y)
{
	ColorData *c = new ColorData(m_curColorRed,m_curColorGreen,m_curColorBlue,1);
	ColorData bkgd;
	Tools* tp;
	int width;
	switch(m_curTool){
	case 0:
		tp = new Pen();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 1:
		tp = new Eraser();
		mask = tp->getMask(); 
		width = tp->getWidth();
		c = new ColorData(m_curColorRed,m_curColorGreen,m_curColorBlue,1);
		*c = m_displayBuffer->getBackgroundColor();
		break;
	case 2:
		tp = new SprayCan();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 3:
		tp = new CalPen();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 4:
		tp = new Highlight();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 5:
		tp = new Special();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 6:
		 //std::cout << "tool 6: stamp tool" << std::endl;
		 break;
	case 7:
		//std::cout << "tool 7: blur tool" << std::endl;
		break;
	default:
		break;
	}
	
	if(m_curTool == 6){}
	else if(m_curTool == 7){
		int kwidth = 9;
		Filter *filter = new Blur_Filter(kwidth);
		
		float** kernel = filter->getKernel();
		PixelBuffer* bucket = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
		m_displayBuffer->copyPixelBuffer(m_displayBuffer, bucket);
//		int width = m_displayBuffer->getWidth();
		int height = m_displayBuffer->getHeight();
		y = height - y -1;
		ColorData currentPixel;
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
	} else if(m_curTool == 4){
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i + 1 > m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]*bkgd.getLuminance()+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2]*bkgd.getLuminance())));
			}
		}
	} else if(m_curTool == 5){
		float r, g, b;
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		c = new ColorData(r,g,b,1);
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i +1> m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2])));
			}
		}
	} else {
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i +1> m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2])));
			}
		}
	}
       
}

void PhotoShop::mouseMoved(int x, int y)
{
	
	
	
    
}

void PhotoShop::leftMouseDown(int x, int y)
{
	
	
	
	
	ColorData *c = new ColorData(m_curColorRed,m_curColorGreen,m_curColorBlue,1);
	ColorData bkgd;
	Tools* tp;
	int width;
	switch(m_curTool){
	case 0:
		tp = new Pen();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 1:
		tp = new Eraser();
		mask = tp->getMask(); 
		width = tp->getWidth();
		c = new ColorData(m_curColorRed,m_curColorGreen,m_curColorBlue,1);
		*c = m_displayBuffer->getBackgroundColor();
		break;
	case 2:
		tp = new SprayCan();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 3:
		tp = new CalPen();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 4:
		tp = new Highlight();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 5:
		tp = new Special();
		mask = tp->getMask(); 
		width = tp->getWidth();
		break;
	case 6:
		 std::cout << "tool 6: stamp tool" << std::endl;
		 break;
	case 7:
		std::cout << "tool 7: blur tool" << std::endl;
		break;
		
	default:
		break;
	}
	
	if(m_curTool == 6){
		ColorData *stampPixel = new ColorData();
		for(int i = 0; i < m_stampBuffer->getWidth(); i++){
			for(int j = 0; j < m_stampBuffer->getHeight(); j++){
				if(x-m_stampBuffer->getWidth()/2+i +1> m_displayBuffer->getWidth() || x-m_stampBuffer->getWidth()/2+i < 0 || m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j+1 > m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j < 0)
					continue;
				if(m_stampBuffer->getPixel(i, j).getAlpha() == 0)	continue;
				stampPixel->setRed(m_stampBuffer->getPixel(i, j).getRed()*m_curColorRed);
				stampPixel->setGreen(m_stampBuffer->getPixel(i, j).getGreen()*m_curColorGreen);
				stampPixel->setBlue(m_stampBuffer->getPixel(i, j).getBlue()*m_curColorBlue);
				m_displayBuffer->setPixel(x-m_stampBuffer->getWidth()/2+i, m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j, *stampPixel);
			}
		}
	} else if(m_curTool == 7){
		int kwidth = 9;
		Filter *filter = new Blur_Filter(kwidth);
		
		float** kernel = filter->getKernel();
		PixelBuffer* bucket = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
		m_displayBuffer->copyPixelBuffer(m_displayBuffer, bucket);
//		int width = m_displayBuffer->getWidth();
		int height = m_displayBuffer->getHeight();
		y = height - y -1;
		ColorData currentPixel;
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
		
	} else if(m_curTool == 4){
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i +1> m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]*bkgd.getLuminance()+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2]*bkgd.getLuminance())));
			}
		}
	} else if(m_curTool == 5){
		float r, g, b;
		r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		c = new ColorData(r,g,b,0.9);
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i +1> m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2])));
			}
		}
	} else {
		for(int i = -(width - 1) / 2; i <= (width - 1) / 2; i++){
			for(int j = -(width - 1) / 2; j <= (width - 1) / 2; j++){
				if(x + i +1> m_displayBuffer->getWidth() || x + i < 0 || m_displayBuffer->getHeight()-y - j +1> m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y - j < 0)
					continue;
				bkgd = m_displayBuffer->getPixel(x + i, m_displayBuffer->getHeight()-y - j);
				m_displayBuffer->setPixel(x + i, m_displayBuffer->getHeight()-y - j, (*c*mask[i+(width - 1) / 2][j+(width - 1) / 2]+bkgd*(1-mask[i+(width - 1) / 2][j+(width - 1) / 2])));
			}
		}
	}
    std::cout << "mousePressed " << x << " " << y << std::endl;
}

void PhotoShop::leftMouseUp(int x, int y)
{
    std::cout << "mouseReleased " << x << " " << y << std::endl;
    backup();
}

void PhotoShop::initializeBuffers(ColorData backgroundColor, int width, int height) {
    m_displayBuffer = new PixelBuffer(width, height, backgroundColor);
}

void PhotoShop::initGlui()
{
    // Select first tool (this activates the first radio button in glui)
    m_curTool = 0;
    
    GLUI_Panel *toolPanel = new GLUI_Panel(m_glui, "Tool Type");
    {
        GLUI_RadioGroup *radio = new GLUI_RadioGroup(toolPanel, &m_curTool, UI_TOOLTYPE, s_gluicallback);
        // Create interface buttons for different tools:
        new GLUI_RadioButton(radio, "Pen");
        new GLUI_RadioButton(radio, "Eraser");
        new GLUI_RadioButton(radio, "Spray Can");
        new GLUI_RadioButton(radio, "Caligraphy Pen");
        new GLUI_RadioButton(radio, "Highlighter");
        new GLUI_RadioButton(radio, "Colorful Dust");
        new GLUI_RadioButton(radio, "Stamp");
        new GLUI_RadioButton(radio, "Blur");
    }
    
    GLUI_Panel *colorPanel = new GLUI_Panel(m_glui, "Tool Color");
    {
        m_curColorRed = 0;
        m_gluiControlHooks.spinnerRed  = new GLUI_Spinner(colorPanel, "Red:", &m_curColorRed, UI_COLOR_R, s_gluicallback);
        m_gluiControlHooks.spinnerRed->set_float_limits(0, 1.0);
        
        m_curColorGreen = 0;
        m_gluiControlHooks.spinnerGreen  = new GLUI_Spinner(colorPanel, "Green:", &m_curColorGreen, UI_COLOR_G, s_gluicallback);
        m_gluiControlHooks.spinnerGreen->set_float_limits(0, 1.0);
        
        m_curColorBlue = 0;
        m_gluiControlHooks.spinnerBlue  = new GLUI_Spinner(colorPanel, "Blue:", &m_curColorBlue, UI_COLOR_B, s_gluicallback);
        m_gluiControlHooks.spinnerBlue->set_float_limits(0, 1.0);
        
        new GLUI_Button(colorPanel, "Red", UI_PRESET_RED, s_gluicallback);
        new GLUI_Button(colorPanel, "Orange", UI_PRESET_ORANGE, s_gluicallback);
        new GLUI_Button(colorPanel, "Yellow", UI_PRESET_YELLOW, s_gluicallback);
        new GLUI_Button(colorPanel, "Green", UI_PRESET_GREEN, s_gluicallback);
        new GLUI_Button(colorPanel, "Blue", UI_PRESET_BLUE, s_gluicallback);
        new GLUI_Button(colorPanel, "Purple", UI_PRESET_PURPLE, s_gluicallback);
        new GLUI_Button(colorPanel, "White", UI_PRESET_WHITE, s_gluicallback);
        new GLUI_Button(colorPanel, "Black", UI_PRESET_BLACK, s_gluicallback);
    }
    
    // UNDO,REDO,QUIT
    {
		m_curPos = 0;
        m_gluiControlHooks.undoButton = new GLUI_Button(m_glui, "Undo", UI_UNDO, s_gluicallback);
        undoEnabled(false);
        m_gluiControlHooks.redoButton  = new GLUI_Button(m_glui, "Redo", UI_REDO, s_gluicallback);
        redoEnabled(false);
        
        new GLUI_Separator(m_glui);
        new GLUI_Button(m_glui, "Quit", UI_QUIT, (GLUI_Update_CB)exit);
    }
    
    new GLUI_Column(m_glui, true);
    GLUI_Panel *filterPanel = new GLUI_Panel(m_glui, "Filters");
    {
        GLUI_Panel *blurPanel = new GLUI_Panel(filterPanel, "Blur");
        {
            GLUI_Spinner * filterBlurAmount = new GLUI_Spinner(blurPanel, "Amount:", &m_filterParameters.blur_amount);
            filterBlurAmount->set_int_limits(0, 20);
            
            filterBlurAmount->set_int_val(5);
            
            new GLUI_Button(blurPanel, "Apply", UI_APPLY_BLUR, s_gluicallback);
        }
        
        GLUI_Panel *motionBlurPanel = new GLUI_Panel(filterPanel, "MotionBlur");
        {
            GLUI_Spinner * filterMotionBlurAmount = new GLUI_Spinner(motionBlurPanel, "Amount:", &m_filterParameters.motionBlur_amount);
            filterMotionBlurAmount->set_int_limits(0, 100);
            
            filterMotionBlurAmount->set_int_val(5);
            
            m_filterParameters.motionBlur_direction = 0;
            GLUI_RadioGroup *dirBlur = new GLUI_RadioGroup(motionBlurPanel, &m_filterParameters.motionBlur_direction);
            new GLUI_RadioButton(dirBlur, "North/South");
            new GLUI_RadioButton(dirBlur, "East/West");
            new GLUI_RadioButton(dirBlur, "NorthEast/SouthWest");
            new GLUI_RadioButton(dirBlur, "NorthWest/SouthEast");
            
            new GLUI_Button(motionBlurPanel, "Apply", UI_APPLY_MOTION_BLUR, s_gluicallback);
        }
        GLUI_Panel *sharpenPanel = new GLUI_Panel(filterPanel, "Sharpen");
        {
            GLUI_Spinner * filterSharpAmount = new GLUI_Spinner(sharpenPanel, "Amount:", &m_filterParameters.sharpen_amount);
            filterSharpAmount->set_int_limits(0, 100);
            
            filterSharpAmount->set_int_val(5);
            
            new GLUI_Button(sharpenPanel, "Apply", UI_APPLY_SHARP, s_gluicallback);
        }
        GLUI_Panel *edgeDetPanel = new GLUI_Panel(filterPanel, "Edge Detect");
        
        {
            new GLUI_Button(edgeDetPanel, "Apply", UI_APPLY_EDGE, s_gluicallback);
        }
        GLUI_Panel *thresPanel = new GLUI_Panel(filterPanel, "Threshold");
        {
            GLUI_Spinner * filterThresholdAmount = new GLUI_Spinner(thresPanel, "Level:", &m_filterParameters.threshold_amount);
            filterThresholdAmount->set_float_limits(0, 1);
            filterThresholdAmount->set_float_val(0.5);
            
            new GLUI_Button(thresPanel, "Apply", UI_APPLY_THRESHOLD, s_gluicallback);
        }
        
        GLUI_Panel *saturPanel = new GLUI_Panel(filterPanel, "Saturation");
        {
            GLUI_Spinner * filterSaturationAmount = new GLUI_Spinner(saturPanel, "Amount:", &m_filterParameters.saturation_amount);
            filterSaturationAmount->set_float_limits(-10, 10);
            filterSaturationAmount->set_float_val(1);
            
            new GLUI_Button(saturPanel, "Apply", UI_APPLY_SATURATE, s_gluicallback);
        }
        
        GLUI_Panel *channelPanel = new GLUI_Panel(filterPanel, "Channels");
        {
            GLUI_Spinner * filterChannelRed = new GLUI_Spinner(channelPanel, "Red:", &m_filterParameters.channel_colorRed);
            GLUI_Spinner * filterChannelGreen = new GLUI_Spinner(channelPanel, "Green:", &m_filterParameters.channel_colorGreen);
            GLUI_Spinner * filterChannelBlue = new GLUI_Spinner(channelPanel, "Blue:", &m_filterParameters.channel_colorBlue);
            
            filterChannelRed->set_float_limits(0, 10);
            filterChannelRed->set_float_val(1);
            filterChannelGreen->set_float_limits(0, 10);
            filterChannelGreen->set_float_val(1);
            filterChannelBlue->set_float_limits(0, 10);
            filterChannelBlue->set_float_val(1);
            
            new GLUI_Button(channelPanel, "Apply", UI_APPLY_CHANNEL, s_gluicallback);
        }
        
        GLUI_Panel *quantPanel = new GLUI_Panel(filterPanel, "Quantize");
        {
            GLUI_Spinner * filterQuantizeBins = new GLUI_Spinner(quantPanel, "Bins:", &m_filterParameters.quantize_bins);
            filterQuantizeBins->set_int_limits(2, 256);
            filterQuantizeBins->set_int_val(8);
            filterQuantizeBins->set_speed(0.1);
            
            new GLUI_Button(quantPanel, "Apply", UI_APPLY_QUANTIZE, s_gluicallback);
        }
        
        GLUI_Panel *specialFilterPanel = new GLUI_Panel(filterPanel, "Special Filter"); // YOUR SPECIAL FILTER PANEL
        {
            new GLUI_Button(specialFilterPanel, "Apply", UI_APPLY_SPECIAL_FILTER, s_gluicallback);
        }
    }
  
    new GLUI_Column(m_glui, true);

    GLUI_Panel *imagePanel = new GLUI_Panel(m_glui, "Image I/O");
    {
        m_gluiControlHooks.fileBrowser = new GLUI_FileBrowser(imagePanel, "Choose Image", false, UI_FILE_BROWSER, s_gluicallback);
        
        m_gluiControlHooks.fileBrowser->set_h(400);
        
        m_gluiControlHooks.fileNameBox = new     GLUI_EditText( imagePanel , "Image:", m_fileName, UI_FILE_NAME, s_gluicallback );
        m_gluiControlHooks.fileNameBox->set_w(200);

        new GLUI_Separator(imagePanel);
        
        m_gluiControlHooks.currentFileLabel = new GLUI_StaticText(imagePanel, "Will load image: none");
        m_gluiControlHooks.loadCanvasButton = new GLUI_Button(imagePanel, "Load Canvas", UI_LOAD_CANVAS_BUTTON, s_gluicallback);
        m_gluiControlHooks.loadStampButton = new GLUI_Button(imagePanel, "Load Stamp", UI_LOAD_STAMP_BUTTON, s_gluicallback);
        
        new GLUI_Separator(imagePanel);

        m_gluiControlHooks.saveFileLabel = new GLUI_StaticText(imagePanel, "Will save image: none");
        
        m_gluiControlHooks.saveCanvasButton = new GLUI_Button(imagePanel, "Save Canvas", UI_SAVE_CANVAS_BUTTON, s_gluicallback);
        
        loadCanvasEnabled(false);
        loadStampEnabled(false);
        saveCanvasEnabled(false);
    }
    backup();
    return;
}

void PhotoShop::gluiControl(int controlID)
{
    
    switch (controlID) {
        case UI_PRESET_RED:
            m_curColorRed = 1;
            m_curColorGreen = 0;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_ORANGE:
            m_curColorRed = 1;
            m_curColorGreen = 0.5;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_YELLOW:
            m_curColorRed = 1;
            m_curColorGreen = 1;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_GREEN:
            m_curColorRed = 0;
            m_curColorGreen = 1;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_BLUE:
            m_curColorRed = 0;
            m_curColorGreen = 0;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_PURPLE:
            m_curColorRed = 0.5;
            m_curColorGreen = 0;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_WHITE:
            m_curColorRed = 1;
            m_curColorGreen = 1;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_BLACK:
            m_curColorRed = 0;
            m_curColorGreen = 0;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_APPLY_BLUR:
            applyFilterBlur();
            backup();
            break;
        case UI_APPLY_SHARP:
            applyFilterSharpen();
            backup();
            break;
        case UI_APPLY_MOTION_BLUR:
            applyFilterMotionBlur();
            backup();
            break;
        case UI_APPLY_EDGE:
            applyFilterEdgeDetect();
            backup();
            break;
        case UI_APPLY_THRESHOLD:
            applyFilterThreshold();
            break;
        case UI_APPLY_DITHER:
            applyFilterThreshold();
            break;
        case UI_APPLY_SATURATE:
            applyFilterSaturate();
            break;
        case UI_APPLY_CHANNEL:
            applyFilterChannel();
            break;
        case UI_APPLY_QUANTIZE:
            applyFilterQuantize();
            break;
        case UI_APPLY_SPECIAL_FILTER:
            applyFilterSpecial();
            backup();
            break;
        case UI_FILE_BROWSER:
            setImageFile(m_gluiControlHooks.fileBrowser->get_file());
            break;
        case UI_LOAD_CANVAS_BUTTON:
            loadImageToCanvas();
            break;
        case UI_LOAD_STAMP_BUTTON:
            loadImageToStamp();
            break;
        case UI_SAVE_CANVAS_BUTTON:
            saveCanvasToFile();
            // Reload the current directory:
            m_gluiControlHooks.fileBrowser->fbreaddir(".");
            break;
        case UI_FILE_NAME:
            setImageFile(m_fileName);
            break;
        case UI_UNDO:
            undoOperation();
            break;
        case UI_REDO:
            redoOperation();
            break;
        default:
            break;
    }
    
    // Forces canvas to update changes made in this function
    m_glui->post_update_main_gfx();
}

// **********************
// *** GLUI CALLBACKS ***

// Edit these functions to provide instructions
// for how PhotoShop should respond to the
// button presses.





struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


void PhotoShop::loadImageToCanvas()
{
    cout << "Load Canvas has been clicked for file " << m_fileName << endl;
    
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  ColorData *c;
  
  unsigned last_dot_pos = m_fileName.find_last_of(".\\");
  std::string extension = m_fileName.substr(last_dot_pos + 1);
  if (extension == "png") {
	  
	  
	    png_image image;
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, m_fileName.c_str())) {
        
			//m_width = image.width;  //pixel width of image
			//m_height = image.height; //pixel height of image
			//m_imagePixels = new PixelBuffer(image.width, image.height, ColorData(0.0,0.0,0.0));
			initializeBuffers(m_displayBuffer->getBackgroundColor(), image.width, image.height);
			setWindowDimensions(image.width, image.height);
			
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];
			std::cout << image.format << std::endl;
        
			// png_image_finish_read fills buffer with all the color bytes in a long one dimensional array (ex. rgbargbargbargba...)
			if (buffer && png_image_finish_read(&image, NULL, buffer, 0, NULL)) { 
            
				for (unsigned y = 0; y < image.height; y++) {
					for (unsigned x = 0; x < image.width*4; x=x+4) {
						c = new ColorData(buffer[y*image.width*4+x]/255.0, buffer[y*image.width*4+x+1]/255.0, buffer[y*image.width*4+x+2]/255.0, buffer[y*image.width*4+x+3]/255.0);
						m_displayBuffer->setPixel(x/4, image.height - y - 1, *c);
					}
				}
            
			}
		}
  }
  
  else if (extension != "jpg" && extension != "jpeg") {
	  cout << extension << " is not a supported file type!!" << endl;
  }
  
  else {


/* everything after this line is for jpeg image processing */
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
  if ((infile = fopen(m_fileName.c_str(), "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", m_fileName.c_str());
    exit(1);
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    exit(1);
  }
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, infile);


  (void) jpeg_read_header(&cinfo, TRUE);

  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
   initializeBuffers(m_displayBuffer->getBackgroundColor(), cinfo.output_width, cinfo.output_height);
   setWindowDimensions(cinfo.output_width, cinfo.output_height);
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    for(int i = 0; i < row_stride; i=i+3){
			c = new ColorData(buffer[0][i]/255.0, buffer[0][i+1]/255.0, buffer[0][i+2]/255.0, 1);
			m_displayBuffer->setPixel(i/3, cinfo.output_height - cinfo.output_scanline, *c);
	}
  }
  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  fclose(infile);
	}
	
	backup();
}

void PhotoShop::loadImageToStamp()
{
    cout << "Load Stamp has been clicked for file " << m_fileName << endl;
    
    /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  ColorData *c;
  
  unsigned last_dot_pos = m_fileName.find_last_of(".\\");
  std::string extension = m_fileName.substr(last_dot_pos + 1);
  if (extension == "png") {
	  
	  
	    png_image image;
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, m_fileName.c_str())) {
        
			//m_width = image.width;  //pixel width of image
			//m_heChannelight = image.height; //pixel height of image
			//m_imagePixels = new PixelBuffer(image.width, image.height, ColorData(0.0,0.0,0.0));
			//initializeBuffers(m_stampBuffer->getBackgroundColor(), image.width, image.height);
			//setWindowDimensions(image.width, image.height);
			m_stampBuffer = new PixelBuffer(image.width, image.height, m_displayBuffer->getBackgroundColor());
			
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];
			std::cout << image.format << std::endl;
        
			// png_image_finish_read fills buffer with all the color bytes in a long one dimensional array (ex. rgbargbargbargba...)
			if (buffer && png_image_finish_read(&image, NULL, buffer, 0, NULL)) { 
            
				for (unsigned y = 0; y < image.height; y++) {
					for (unsigned x = 0; x < image.width*4; x=x+4) {
						c = new ColorData(buffer[y*image.width*4+x]/255.0, buffer[y*image.width*4+x+1]/255.0, buffer[y*image.width*4+x+2]/255.0, buffer[y*image.width*4+x+3]/255.0);
						m_stampBuffer->setPixel(x/4, image.height - y - 1, *c);
					}
				}
            
			}
		}
  }
  
  else if (extension != "jpg" && extension != "jpeg") {
	  cout << extension << " is not a supported file type!!" << endl;
  }
  
  else {


/* everything after this line is for jpeg image processing */
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
  if ((infile = fopen(m_fileName.c_str(), "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", m_fileName.c_str());
    exit(1);
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    exit(1);
  }
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, infile);


  (void) jpeg_read_header(&cinfo, TRUE);

  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

   m_stampBuffer = new PixelBuffer(cinfo.output_width, cinfo.output_height, m_displayBuffer->getBackgroundColor());
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    for(int i = 0; i < row_stride; i=i+3){
			c = new ColorData(buffer[0][i]/255.0, buffer[0][i+1]/255.0, buffer[0][i+2]/255.0, 1);
			m_stampBuffer->setPixel(i/3, cinfo.output_height - cinfo.output_scanline, *c);
	}
  }
  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  fclose(infile);
	}
    

}

int PhotoShop::write_JPEG_file(std::string outputName)
{
	struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;
	
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    
    if ((outfile = fopen(outputName.c_str() , "wb")) == NULL) {
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);
    
    cinfo.image_width = m_displayBuffer->getWidth(); 	/* image width and height, in pixels */
    cinfo.image_height = m_displayBuffer->getHeight();
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
    
    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, 100/*quality*/, TRUE /* limit to baseline-JPEG values */);
    
    jpeg_start_compress(&cinfo, TRUE);


    row_stride = cinfo.image_width * 3;	/* JSAMPLEs per row in image_buffer */
    
    const int width = m_displayBuffer->getWidth();
    const int height = m_displayBuffer->getHeight();
    JSAMPLE* image_buffer = new JSAMPLE[row_stride*cinfo.image_height];
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ColorData currentPixel = m_displayBuffer->getPixel(x, y);
            image_buffer[((height-(y+1))*width+x)*3] = (JSAMPLE) (currentPixel.getRed()*255.0);
            image_buffer[((height-(y+1))*width+x)*3+1] = (JSAMPLE) (currentPixel.getGreen()*255.0);
            image_buffer[((height-(y+1))*width+x)*3+2] = (JSAMPLE) (currentPixel.getBlue()*255.0);
        }
    }
    
    
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    
    delete[] image_buffer;
    
    
    jpeg_finish_compress(&cinfo);

    fclose(outfile);

    jpeg_destroy_compress(&cinfo);

	return 1;
}


int PhotoShop::write_PNG_file(std::string outputName){
	
	png_image image;
	memset(&image, 0, (sizeof image));
	image.version = PNG_IMAGE_VERSION;
	image.width = m_displayBuffer->getWidth();
	image.height = m_displayBuffer->getHeight();
			
	png_bytep buffer;
	image.format = PNG_FORMAT_RGBA;
	buffer = new png_byte[PNG_IMAGE_SIZE(image)];

	for(int h = 0; h < m_displayBuffer->getHeight(); h++){
		for(int w = 0; w < m_displayBuffer->getWidth()*4; w=w+4){
			buffer[h*m_displayBuffer->getWidth()*4+w] = m_displayBuffer->getPixel(w/4, m_displayBuffer->getHeight()-h-1).getRed()*255.0;
			buffer[h*m_displayBuffer->getWidth()*4+w+1] = m_displayBuffer->getPixel(w/4, m_displayBuffer->getHeight()-h-1).getGreen()*255.0;
			buffer[h*m_displayBuffer->getWidth()*4+w+2] = m_displayBuffer->getPixel(w/4, m_displayBuffer->getHeight()-h-1).getBlue()*255.0;
			buffer[h*m_displayBuffer->getWidth()*4+w+3] = m_displayBuffer->getPixel(w/4, m_displayBuffer->getHeight()-h-1).getAlpha()*255.0;
		}
	}
			
	if (png_image_write_to_file(&image,  outputName.c_str(), 0/*convert_to_8bit*/,
		buffer, 0/*row_stride*/, NULL/*colormap*/) != 0) {
		return 1;
	}
	else {
		return -1;
	}

}


void PhotoShop::saveCanvasToFile()
{
	std::string outputName = m_gluiControlHooks.fileNameBox->get_text();
    unsigned last_dot_pos =  outputName.find_last_of(".\\");
	std::string extension = outputName.substr(last_dot_pos + 1);
	
	if (extension == "png" || extension == "PNG") {
		
		if(write_PNG_file(outputName) > 0){
			cout << "write png image sucessfully!" << endl;
		}
		else{
			cout << "image is not saved." << endl;
		}
		
	}
	
	else if (extension != "jpg" && extension != "jpeg" && extension != "JPG" && extension != "JPEG") {
	  cout << extension << " is not a supported file type!!" << endl;
	}
	
	else {
		if(write_JPEG_file(outputName) > 0){
			cout << "write jpg image sucessfully!" << endl;
		}
		else{
			cout << "image is not saved." << endl;
		}
	}
}

void PhotoShop::saveCanvas(std::string outputName){
	
	unsigned last_dot_pos =  outputName.find_last_of(".\\");
	std::string extension = outputName.substr(last_dot_pos + 1);
	
	if (extension == "png" || extension == "PNG") {
		
		if(write_PNG_file(outputName) > 0){
			cout << "write png image sucessfully!" << endl;
		}
		else{
			cout << "image is not saved." << endl;
		}
		
	}
	
	else if (extension != "jpg" && extension != "jpeg" && extension != "JPG" && extension != "JPEG") {
	  cout << extension << " is not a supported file type!!" << endl;
	}
	
	else {
		if(write_JPEG_file(outputName) > 0){
			cout << "write jpg image sucessfully!" << endl;
		}
		else{
			cout << "image is not saved." << endl;
		}
	}
	
}



void PhotoShop::applyFilters(int filterTool){
	Filter *filter;
	float amount;
	int kwidth;
	switch(filterTool){
		case 1:
			amount = m_filterParameters.blur_amount;
			kwidth = (int) amount;
			kwidth = (kwidth % 2 != 0)? kwidth : kwidth + 1;
			filter = new Blur_Filter(kwidth);
			break;
		case 2:
			amount = m_filterParameters.motionBlur_amount;
			kwidth = (int) amount;
			kwidth = (kwidth % 2 != 0)? kwidth : kwidth + 1;
			filter = new MotionBlur_Filter(m_filterParameters.motionBlur_direction, kwidth);
			break;
		case 3:
			amount = m_filterParameters.sharpen_amount;
			kwidth = (int) amount;
			kwidth = (kwidth % 2 != 0)? kwidth : kwidth + 1;
			filter = new Sharpen_Filter(kwidth);
			break;
		case 4:
			kwidth = 3;
			filter = new EdgeDetect_Filter();
			break;
		default:
			break;
	}
	
	
    //int kwidth =5;
    
    float** kernel = filter->getKernel();
    PixelBuffer* bucket = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
	m_displayBuffer->copyPixelBuffer(m_displayBuffer, bucket);
    int width = m_displayBuffer->getWidth();
    int height = m_displayBuffer->getHeight();
    ColorData currentPixel;
    float red = 0;
    float green = 0;
    float blue = 0;
    
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			for(int k = i-kwidth/2; k <= i + kwidth/2; k++){
				for(int l = j-kwidth/2; l <= j+kwidth/2; l++){
					if(k < m_displayBuffer->getWidth() && k >= 0 && l < m_displayBuffer->getHeight() && l >= 0){
						currentPixel = bucket->getPixel(k,l);
						
						//if(k-i >= 0 && k-i < kwidth && l-j >= 0 && l-j < kwidth){
							red += (currentPixel.getRed() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
							green += (currentPixel.getGreen() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
							blue += (currentPixel.getBlue() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
						//}
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
    
    //backup();	
}


void PhotoShop::applyFilterThreshold()
{
    cout << "Apply has been clicked for Threshold has been clicked with amount =" << m_filterParameters.threshold_amount << endl;
    
	ColorData current_pixel;
	float red, green, blue;
	float amount = 1- m_filterParameters.threshold_amount;
	
	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
				red = (current_pixel.getRed() > amount)? 1: 0;
				green = (current_pixel.getGreen() > amount)? 1:0;
				blue = (current_pixel.getBlue() > amount)? 1: 0;
			
			current_pixel.setRed(red);
			current_pixel.setGreen(green);
			current_pixel.setBlue(blue);
			m_displayBuffer->setPixel(i, j, current_pixel);
		}
	}
	backup();
}

void PhotoShop::applyFilterChannel()
{
    cout << "Apply has been clicked for Channels with red = " << m_filterParameters.channel_colorRed
    << ", green = " << m_filterParameters.channel_colorGreen
    << ", blue = " << m_filterParameters.channel_colorBlue << endl;
    
    ColorData current_pixel;
	float red, green, blue;
	float cred, cgreen, cblue;
	cred = m_filterParameters.channel_colorRed;
	cgreen = m_filterParameters.channel_colorGreen;
	cblue = m_filterParameters.channel_colorBlue;

	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
			
				red = current_pixel.getRed() * cred;
				green = current_pixel.getGreen() * cgreen;
				blue = current_pixel.getBlue() * cblue;
			current_pixel.setRed(red);
			current_pixel.setGreen(green);
			current_pixel.setBlue(blue);
			m_displayBuffer->setPixel(i, j, current_pixel);
		}
	}
	
    backup();
    
}

void PhotoShop::applyFilterSaturate()
{
    cout << "Apply has been clicked for Saturate with amount = " << m_filterParameters.saturation_amount << endl;

    ColorData current_pixel;
	float red, green, blue, luminance;
	float amount = m_filterParameters.saturation_amount;

	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
				luminance = current_pixel.getLuminance();	
			//0.2126*m_red + 0.7152*m_green + 0.0722*m_blue;
				
				red = luminance + (current_pixel.getRed() - luminance) * amount;
				green = luminance + (current_pixel.getGreen() - luminance) * amount;
				blue = luminance + (current_pixel.getBlue() - luminance) * amount;
			current_pixel.setRed(red);
			current_pixel.setGreen(green);
			current_pixel.setBlue(blue);
			m_displayBuffer->setPixel(i, j, current_pixel);
		}
	}
	    backup();

}

void PhotoShop::applyFilterBlur()
{
    cout << "Apply has been clicked for Blur with amount = " << m_filterParameters.blur_amount << endl;
    applyFilters(1);
    
}

void PhotoShop::applyFilterSharpen()
{
    cout << "Apply has been clicked for Sharpen with amount = " << m_filterParameters.sharpen_amount << endl;
    applyFilters(3);
    
}

void PhotoShop::applyFilterMotionBlur()
{
    cout << "Apply has been clicked for Sharpen with amount = " << m_filterParameters.motionBlur_amount
    << " and direction " << m_filterParameters.motionBlur_direction << endl;
    applyFilters(2);
}

void PhotoShop::applyFilterEdgeDetect() {
    cout << "Apply has been clicked for Edge Detect" << endl;
    
    applyFilters(4);
}

void PhotoShop::applyFilterQuantize() {
    cout << "Apply has been clicked for Quantize with bins = " << m_filterParameters.quantize_bins << endl;
    
    int bin_number = m_filterParameters.quantize_bins;
    float range = 1.0 / (bin_number -1);
    ColorData current_pixel;
	float red, green, blue;
	float checkpoint1, checkpoint2, d1,d2;

	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
			red = current_pixel.getRed();
			green = current_pixel.getGreen();
			blue = current_pixel.getBlue();
				
				for(float k = 1+range; k > 0; k -= range){
					 checkpoint1 = k;
					 checkpoint2 = k - range;
					//CASE RED
					 d1 = fabsf(checkpoint1 - current_pixel.getRed());
					 d2 = fabsf(checkpoint2 - current_pixel.getRed());
					if(current_pixel.getRed() <= checkpoint1 && current_pixel.getRed() >= checkpoint2){
						red = (d1 < d2)? checkpoint1 : checkpoint2;
					}
					//CASE GREEN
					d1 = fabsf(checkpoint1 - current_pixel.getGreen());
					d2 = fabsf(checkpoint2 - current_pixel.getGreen());
					if(current_pixel.getGreen() <= checkpoint1 && current_pixel.getGreen() >= checkpoint2){
						green = (d1 < d2)? checkpoint1 : checkpoint2;
					}
					//CASE BLUE
					d1 = fabsf(checkpoint1 - current_pixel.getBlue());
					d2 = fabsf(checkpoint2 - current_pixel.getBlue());
					if(current_pixel.getBlue() <= checkpoint1 && current_pixel.getBlue() >= checkpoint2){
						blue = (d1 < d2)? checkpoint1 : checkpoint2;
					}
				}			
			current_pixel.setRed(red);
			current_pixel.setGreen(green);
			current_pixel.setBlue(blue);
			m_displayBuffer->setPixel(i, j, current_pixel);
		}
	}    
    backup();
}

void PhotoShop::applyFilterSpecial() {
    cout << "Apply has been clicked for Special" << endl;
   
    ColorData current_pixel;
	float red, green, blue;
	applyFilters(4);
	
	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
				red = 0.9 - current_pixel.getRed();
				green = 0.9 - current_pixel.getGreen();
				blue = 0.9 - current_pixel.getBlue();
			
			current_pixel.setRed(red);
			current_pixel.setGreen(green);
			current_pixel.setBlue(blue);
			m_displayBuffer->setPixel(i, j, current_pixel);
		}
	}
    
    
    //backup();
}

void PhotoShop::undoOperation()
{
    cout << "Undoing..." << endl;
    undoEnabled((bool)(--m_curPos != 0));
    redoEnabled((bool)((unsigned) m_curPos != m_list.size()-1));
    initializeBuffers(m_displayBuffer->getBackgroundColor(), m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
	setWindowDimensions(m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
    m_displayBuffer->copyPixelBuffer(m_list[m_curPos], m_displayBuffer);
}

void PhotoShop::redoOperation()
{
    cout << "Redoing..." << endl;
    redoEnabled((bool)( unsigned (++m_curPos) != m_list.size()-1));
    undoEnabled((bool)((unsigned) m_curPos != 0));
    initializeBuffers(m_displayBuffer->getBackgroundColor(), m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
	setWindowDimensions(m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
    m_displayBuffer->copyPixelBuffer(m_list[m_curPos], m_displayBuffer);
}
// ** END OF CALLBACKS **
// **********************


// **********************
// Provided code for managing the
// GLUI interface.

void PhotoShop::buttonEnabled(GLUI_Button * button, bool enabled) {
    if(enabled) button->enable();
    else button->disable();
    button->redraw();
}

void PhotoShop::redoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.redoButton, enabled);
}

void PhotoShop::undoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.undoButton, enabled);
}

void PhotoShop::saveCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.saveCanvasButton, enabled);
}

void PhotoShop::loadStampEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.loadStampButton, enabled);
}

void PhotoShop::loadCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.loadCanvasButton, enabled);
}

void PhotoShop::updateColors() {
    m_gluiControlHooks.spinnerBlue->set_float_val(m_curColorBlue);
    m_gluiControlHooks.spinnerGreen->set_float_val(m_curColorGreen);
    m_gluiControlHooks.spinnerRed->set_float_val(m_curColorRed);
}

bool PhotoShop::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}


bool PhotoShop::isValidImageFileName(const std::string & name) {
    
    if (hasSuffix(name, ".png")
        || hasSuffix(name, ".jpg")
        || hasSuffix(name, ".jpeg")
        )
        return true;
    else
        return false;
}

bool PhotoShop::isValidImageFile(const std::string & name) {
    
    FILE *f;
    bool isValid = false;
    if (isValidImageFileName(name)) {
        if ((f = fopen( name.c_str(), "r"))) {
            isValid = true;
            fclose(f);
        }
    }
    return isValid;
}

void PhotoShop::setImageFile(const std::string & fileName)
{
    // If a directory was selected
    // instead of a file, use the
    // latest file typed or selected.
    std::string imageFile = fileName;
    if (!isValidImageFileName(imageFile)) {
        imageFile = m_fileName;
    }
    
    
    // TOGGLE SAVE FEATURE
    // If no file is selected or typed,
    // don't allow file to be saved. If
    // there is a file name, then allow
    // file to be saved to that name.
    
    if (!isValidImageFileName(imageFile)) {
        m_gluiControlHooks.saveFileLabel->set_text("Will save image: none");
        saveCanvasEnabled(false);
    } else {
        m_gluiControlHooks.saveFileLabel->set_text((std::string("Will save image: ") + imageFile).c_str());
        saveCanvasEnabled(true);
    }
    
    // TOGGLE LOAD FEATURE
    
    // If the file specified cannot be opened,
    // then disable stamp and canvas loading.
    if (isValidImageFile(imageFile)) {
        loadStampEnabled(true);
        loadCanvasEnabled(true);
        
        m_gluiControlHooks.currentFileLabel->set_text((std::string("Will load: ") + imageFile).c_str());
        m_gluiControlHooks.fileNameBox->set_text(imageFile);
    } else {
        loadStampEnabled(false);
        loadCanvasEnabled(false);
        m_gluiControlHooks.currentFileLabel->set_text("Will load: none");
    }
}

void PhotoShop::setCurTool(int c){
	m_curTool = c;
}

void PhotoShop::loadStamp(std::string f){
	m_fileName = f;
	loadImageToStamp();
}

void PhotoShop::resetCanvas(){
	 
	 if (m_displayBuffer) {
        delete m_displayBuffer;
    }
	initializeBuffers(ColorData(1,1,0.95), 800, 800);

}












