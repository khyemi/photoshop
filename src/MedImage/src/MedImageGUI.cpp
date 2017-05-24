
/**********************************************************************
 * @file MedImageGUI.cpp
 * @brief A medical MRI scans processing software.
 * 
 * MedImageGUI.cpp:
 * 	Features include the following:
 * 	A red arrow stamp and pen tool for circling locations in image.
 *	A set of filters to process MRI scans
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "MedImageGUI.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include <png.h>
#include <jpeglib.h>

#include "Tools.h"
#include "Pen.h"
#include <sstream>

#include <cstdlib>
#include <cmath>
#include <stdio.h>

#include "Filter.h"
#include "Blur_Filter.h"
#include "MotionBlur_Filter.h"
#include "EdgeDetect_Filter.h"
#include "Sharpen_Filter.h"
using std::cout;
using std::endl;

MedImageGUI::MedImageGUI(int argc, char* argv[], int width, int height, ColorData backgroundColor) : BaseGfxApp(argc, argv, width, height, 50, 50, GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH, true, width+51, 50)
{
	// Set the name of the window
	setCaption("MedImage");
	// Initialize Interface
	initializeBuffers(backgroundColor, width, height);
	initGlui();
	initGraphics();

}


void MedImageGUI::display()
{
    drawPixels(0, 0, m_width, m_height, m_displayBuffer->getData());
}

MedImageGUI::~MedImageGUI()
{
    if (m_displayBuffer) {
        delete m_displayBuffer;
    }
}


void MedImageGUI::mouseDragged(int x, int y)
{
	ColorData *c = new ColorData(1,0,0,1); // red color
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
	// fall through.
    default:
		break;
	}
	
	if(m_curTool == 1){ // do nothing
	}
	else{
	
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

void MedImageGUI::mouseMoved(int x, int y)
{
    
}

void MedImageGUI::leftMouseDown(int x, int y)
{
    std::cout << "mousePressed " << x << " " << y << std::endl;
    ColorData *c = new ColorData(1,0,0,1); // red color
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
		std::cout << "Arrow tool selected" << x << " " << y << std::endl;
		// load arrow tool to stamp in init method already.
		
    default:
		break;
	}
	
	if(m_curTool == 1){
		
		ColorData *stampPixel = new ColorData();
		for(int i = 0; i < m_stampBuffer->getWidth(); i++){
			for(int j = 0; j < m_stampBuffer->getHeight(); j++){
				if(x-m_stampBuffer->getWidth()/2+i +1> m_displayBuffer->getWidth() || x-m_stampBuffer->getWidth()/2+i < 0 || m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j+1 > m_displayBuffer->getHeight() || m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j < 0)
					continue;
				if(m_stampBuffer->getPixel(i, j).getAlpha() == 0)	continue;
				stampPixel->setRed(1);
				stampPixel->setGreen(0);
				stampPixel->setBlue(0);
				m_displayBuffer->setPixel(x-m_stampBuffer->getWidth()/2+i, m_displayBuffer->getHeight()-y-m_stampBuffer->getHeight()/2+j, *stampPixel);
			}
		}
	}
	else{
	
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

void MedImageGUI::leftMouseUp(int x, int y)
{
    std::cout << "mouseReleased " << x << " " << y << std::endl;
    backup();
}

void MedImageGUI::initializeBuffers(ColorData backgroundColor, int width, int height) {
    m_displayBuffer = new PixelBuffer(width, height, backgroundColor);
}

void MedImageGUI::initGlui()
{
    // Select first tool (this activates the first radio button in glui)
    m_curTool = 0;
    
    
    
    new GLUI_Column(m_glui, false);
    GLUI_Panel *toolPanel = new GLUI_Panel(m_glui, "Tool Type");
    {
        GLUI_RadioGroup *radio = new GLUI_RadioGroup(toolPanel, &m_curTool, UI_TOOLTYPE, s_gluicallback);
        // Create interface buttons for different tools:
        new GLUI_RadioButton(radio, "Pen");
        new GLUI_RadioButton(radio, "Arrow");
        
    }
    
    
    GLUI_Panel *filterPanel = new GLUI_Panel(m_glui, "Filters");
    {
        GLUI_Panel *sharpenPanel = new GLUI_Panel(filterPanel, "Sharpen");
        {
            GLUI_Spinner * filterSharpAmount = new GLUI_Spinner(sharpenPanel, "Amount:", &m_filterParameters.sharpen_amount);
            filterSharpAmount->set_int_limits(1, 10);
            filterSharpAmount->set_int_val(1);
            filterSharpAmount->set_speed(0.1);
            
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
        
        GLUI_Panel *multiplyPanel = new GLUI_Panel(filterPanel, "Multiply RGB");
        {
            GLUI_Spinner * filterChannelRed = new GLUI_Spinner(multiplyPanel, "R:", &m_filterParameters.multiply_colorRed);
            GLUI_Spinner * filterChannelGreen = new GLUI_Spinner(multiplyPanel, "G:", &m_filterParameters.multiply_colorGreen);
            GLUI_Spinner * filterChannelBlue = new GLUI_Spinner(multiplyPanel, "B:", &m_filterParameters.multiply_colorBlue);
            
            filterChannelRed->set_float_limits(0, 10);
            filterChannelRed->set_float_val(1);
            filterChannelGreen->set_float_limits(0, 10);
            filterChannelGreen->set_float_val(1);
            filterChannelBlue->set_float_limits(0, 10);
            filterChannelBlue->set_float_val(1);
            
            new GLUI_Button(multiplyPanel, "Apply", UI_APPLY_MULTIPLY_RGB, s_gluicallback);
        }
        
        GLUI_Panel *quantPanel = new GLUI_Panel(filterPanel, "Quantize");
        {
            GLUI_Spinner * filterQuantizeBins = new GLUI_Spinner(quantPanel, "Bins:", &m_filterParameters.quantize_bins);
            filterQuantizeBins->set_int_limits(2, 256);
            filterQuantizeBins->set_int_val(8);
            filterQuantizeBins->set_speed(0.1);
            
            new GLUI_Button(quantPanel, "Apply", UI_APPLY_QUANTIZE, s_gluicallback);
        }
        
        GLUI_Panel *grayPanel = new GLUI_Panel(filterPanel, "Grayscale");
        {
            
            new GLUI_Button(grayPanel, "Apply", UI_APPLY_GRAYSCALE, s_gluicallback);
        }


        
        // UNDO,REDO,QUIT
        {
            m_gluiControlHooks.undoButton = new GLUI_Button(m_glui, "Undo", UI_UNDO, s_gluicallback);
            undoEnabled(false);
            m_gluiControlHooks.redoButton  = new GLUI_Button(m_glui, "Redo", UI_REDO, s_gluicallback);
            redoEnabled(false);
            
            new GLUI_Separator(m_glui);
            new GLUI_Button(m_glui, "Quit", UI_QUIT, (GLUI_Update_CB)exit);
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
        
        new GLUI_Separator(imagePanel);

        m_gluiControlHooks.saveFileLabel = new GLUI_StaticText(imagePanel, "Will save image: none");
        
        m_gluiControlHooks.saveCanvasButton = new GLUI_Button(imagePanel, "Save Canvas", UI_SAVE_CANVAS_BUTTON, s_gluicallback);
        
        
        m_gluiControlHooks.previousImageButton = new GLUI_Button(imagePanel, "Previous Image", UI_PREVIOUS_IMAGE_BUTTON, s_gluicallback);
        m_gluiControlHooks.nextImageButton = new GLUI_Button(imagePanel, "Next Image", UI_NEXT_IMAGE_BUTTON, s_gluicallback);
        
        
        loadCanvasEnabled(false);
        saveCanvasEnabled(false);
        nextImageEnabled(false);
        previousImageEnabled(false);
    }
    loadArrowToStamp();
    backup(); // initial backup an empty display buffer.
    return;
}

void MedImageGUI::gluiControl(int controlID)
{
    
    switch (controlID) {
        case UI_APPLY_SHARP:
            applyFilterSharpen();
            break;
        case UI_APPLY_EDGE:
            applyFilterEdgeDetect();
            break;
        case UI_APPLY_THRESHOLD:
            applyFilterThreshold();
            break;
        case UI_APPLY_GRAYSCALE:
            applyFilterGrayScale();
            break;
        case UI_APPLY_MULTIPLY_RGB:
            applyFilterMultiplyRGB();
            break;
        case UI_APPLY_QUANTIZE:
            applyFilterQuantize();
            break;
        case UI_FILE_BROWSER:
            setImageFile(m_gluiControlHooks.fileBrowser->get_file());
            break;
        case UI_LOAD_CANVAS_BUTTON:
            loadImageToCanvas();
            break;
        case UI_SAVE_CANVAS_BUTTON:
            saveCanvasToFile();
            // Reload the current directory:
            m_gluiControlHooks.fileBrowser->fbreaddir(".");
            break;
        case UI_NEXT_IMAGE_BUTTON:
            loadNextImage();
            break;
        case UI_PREVIOUS_IMAGE_BUTTON:
            loadPreviousImage();
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
// for how MedImageGUI should respond to the
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


void MedImageGUI::loadImageToCanvas()
{
    cout << "Load Canvas has been clicked for file " << m_fileName << endl;
    
    // TODO: Perform loading task
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
        
			initializeBuffers(m_displayBuffer->getBackgroundColor(), image.width, image.height);
			setWindowDimensions(image.width, image.height);
			
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];
			std::cout << image.format << std::endl;
        
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
    
    
    
    
    
    
    
    
    
    ///////////////////////////////////////////////////////////
    
    // Determining whether there are next or previous images
    m_nextFileName = getImageNamePlusSeqOffset(m_fileName, 1);
    m_prevFileName = getImageNamePlusSeqOffset(m_fileName, -1);
    
    nextImageEnabled(isValidImageFile(m_nextFileName));
    previousImageEnabled(isValidImageFile(m_prevFileName));
}

void MedImageGUI::loadArrowToStamp(){
	
  ColorData *c;
  
	    png_image image;
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, "marker_smaller.png")) {
        
			m_stampBuffer = new PixelBuffer(image.width, image.height, m_displayBuffer->getBackgroundColor());
			
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];
			std::cout << image.format << std::endl;

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

int MedImageGUI::write_JPEG_file ()
{
	struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;
	std::string ouputName = m_gluiControlHooks.fileNameBox->get_text();
	
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    
    if ((outfile = fopen(ouputName.c_str() , "wb")) == NULL) {
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


int MedImageGUI::write_PNG_file(){
	
	std::string outputName = m_gluiControlHooks.fileNameBox->get_text();
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


void MedImageGUI::saveCanvasToFile()
{
	std::string outputName = m_gluiControlHooks.fileNameBox->get_text();
    unsigned last_dot_pos =  outputName.find_last_of(".\\");
	std::string extension = outputName.substr(last_dot_pos + 1);
	
	if (extension == "png" || extension == "PNG") {
		
		if(write_PNG_file() > 0){
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
		if(write_JPEG_file() > 0){
			cout << "write jpg image sucessfully!" << endl;
		}
		else{
			cout << "image is not saved." << endl;
		}
	}
}


void MedImageGUI::applyFilterThreshold()
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

void MedImageGUI::applyFilterMultiplyRGB()
{
    cout << "Apply has been clicked for Multiply RGB with red = " << m_filterParameters.multiply_colorRed
    << ", green = " << m_filterParameters.multiply_colorGreen
    << ", blue = " << m_filterParameters.multiply_colorBlue << endl;
    ColorData current_pixel;
	float red, green, blue;
	float cred, cgreen, cblue;
	cred = m_filterParameters.multiply_colorRed;
	cgreen = m_filterParameters.multiply_colorGreen;
	cblue = m_filterParameters.multiply_colorBlue;

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

void MedImageGUI::applyFilterGrayScale()
{
    cout << "Apply has been clicked for Grayscale" << endl;
    
    ColorData current_pixel;
	float red, green, blue, luminance;
	float amount = 0.1;

	for(int i = 0; i < m_displayBuffer->getWidth(); i++){
		for(int j = 0; j < m_displayBuffer->getHeight(); j++){
			current_pixel = m_displayBuffer->getPixel(i, j);
				luminance = current_pixel.getLuminance();
				
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

void MedImageGUI::applyFilters(int filterTool){
	Filter *filter;
	float amount;
	int kwidth;
	switch(filterTool){
		
		case 1:
			amount = m_filterParameters.sharpen_amount;
			kwidth = (int) amount;
			kwidth = (kwidth % 2 != 0)? kwidth : kwidth + 1;
			filter = new Sharpen_Filter(kwidth);
			break;
		case 2:
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
						red += (currentPixel.getRed() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
						green += (currentPixel.getGreen() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
						blue += (currentPixel.getBlue() * kernel[k+kwidth/2-i][l+kwidth/2-j]);
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

void MedImageGUI::applyFilterSharpen()
{
    cout << "Apply has been clicked for Sharpen with amount = " << m_filterParameters.sharpen_amount << endl;
    applyFilters(1);
}

void MedImageGUI::applyFilterEdgeDetect() {
    cout << "Apply has been clicked for Edge Detect" << endl;
    applyFilters(2);
}

void MedImageGUI::applyFilterQuantize() {
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

void MedImageGUI::backup(){
	while(m_list.size() > (unsigned) m_curPos+1)
		m_list.pop_back();
		
	PixelBuffer * temp = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
	m_displayBuffer->copyPixelBuffer(m_displayBuffer, temp);
	m_curPos++;
	m_list.push_back(temp);
	//cout << "m_curPos = " << m_curPos << "size = " << m_list.size() << endl;
	undoEnabled((bool)(m_curPos != 0));
    redoEnabled((bool)( (unsigned) m_curPos != m_list.size()-1));
}

void MedImageGUI::undoOperation()
{
    cout << "Undoing..." << endl;
    undoEnabled((bool)(--m_curPos != 0));
    redoEnabled((bool)((unsigned) m_curPos != m_list.size()-1));
    initializeBuffers(m_displayBuffer->getBackgroundColor(), m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
	setWindowDimensions(m_list[m_curPos]->getWidth(), m_list[m_curPos]->getHeight());
    m_displayBuffer->copyPixelBuffer(m_list[m_curPos], m_displayBuffer);
}

void MedImageGUI::redoOperation()
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

void MedImageGUI::buttonEnabled(GLUI_Button * button, bool enabled) {
    if(enabled) button->enable();
    else button->disable();
    button->redraw();
}

void MedImageGUI::redoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.redoButton, enabled);
}

void MedImageGUI::undoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.undoButton, enabled);
}

void MedImageGUI::saveCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.saveCanvasButton, enabled);
}

void MedImageGUI::loadCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.loadCanvasButton, enabled);
}

void MedImageGUI::previousImageEnabled(bool enabled) {
    buttonEnabled(m_gluiControlHooks.previousImageButton, enabled);
}

void MedImageGUI::nextImageEnabled(bool enabled) {
    buttonEnabled(m_gluiControlHooks.nextImageButton, enabled);
}

void MedImageGUI::loadNextImage()
{
    setImageFile(m_nextFileName);
    loadImageToCanvas();
}

void MedImageGUI::loadPreviousImage()
{
    setImageFile(m_prevFileName);
    loadImageToCanvas();
}


bool MedImageGUI::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}

bool MedImageGUI::isValidImageFileName(const std::string & name) {
    
    if (hasSuffix(name, ".png")
        || hasSuffix(name, ".jpg")
        || hasSuffix(name, ".jpeg")
        )
        return true;
    else
        return false;
}

bool MedImageGUI::isValidImageFile(const std::string & name) {
    
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

std::string MedImageGUI::getImageNamePlusSeqOffset(const std::string & currentFileName, int offset)
{
    
    int digitCount = 3;
    
    std::string extension, name, number;
    size_t dotPos = currentFileName.find_last_of(".");
    if (dotPos ==  std::string::npos || dotPos == 0) {
        return "";
    }
    
    extension = currentFileName.substr(dotPos+1);
    name = currentFileName.substr(0,dotPos);
    if ( name.length() < (unsigned) digitCount) {
        return "";
    }
    
    number = name.substr(name.length()-digitCount);
    name = name.substr(0,name.length()-digitCount);
    int num;
    std::istringstream ( number ) >> num;
    int output_num = num +offset;
    if (output_num < 0) {
        return "";
    }
    std::stringstream ss;
    ss << output_num;
    std::string output_number;
    ss >> output_number;
    
    // Append zero chars
    size_t str_length = output_number.length();
    for (unsigned i = 0; i < digitCount - str_length; i++)
        output_number = "0" + output_number;

    return (name + output_number + "." + extension);
}

void MedImageGUI::setImageFile(const std::string & fileName)
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
        loadCanvasEnabled(true);
        
        m_gluiControlHooks.currentFileLabel->set_text((std::string("Will load: ") + imageFile).c_str());
        m_gluiControlHooks.fileNameBox->set_text(imageFile);
    } else {
        loadCanvasEnabled(false);
        m_gluiControlHooks.currentFileLabel->set_text("Will load: none");
    }
}


