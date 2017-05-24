
/**********************************************************************
 * @file MedImageCML.cpp
 * @brief Commandline version of MedImageGUI
 * 
 * MedImageCML:
 * 	This class uses the filters and some of the tools declared in
 *  liphoto. See MedImageCML.h for more information.
 * 
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "MedImageCML.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include "Tools.h"
#include "Pen.h"
#include <sstream>

#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <string.h>

#include "Filter.h"
#include "EdgeDetect_Filter.h"
#include "Sharpen_Filter.h"
#include "Blur_Filter.h"

using std::cout;
using std::endl;

MedImageCML::MedImageCML(int argc, char* argv[], ColorData backgroundColor)
{

	initializeBuffers(backgroundColor, 800, 800);
}

MedImageCML::~MedImageCML()
{
    if (m_displayBuffer) {
        delete m_displayBuffer;
    }
}


void MedImageCML::initializeBuffers(ColorData backgroundColor, int width, int height) {
    m_displayBuffer = new PixelBuffer(width, height, backgroundColor);
}


// Structures:

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


void MedImageCML::loadImageToCanvas()
{

  ColorData *c;
  
  unsigned last_dot_pos = m_fileName.find_last_of(".\\");
  std::string extension = m_fileName.substr(last_dot_pos + 1);
  if (extension == "png") {
	  
	    png_image image;
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, m_fileName.c_str())) {
        
			initializeBuffers(m_displayBuffer->getBackgroundColor(), image.width, image.height);
		
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];

        
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
		    FILE * infile;		/* source file */
			JSAMPARRAY buffer;		/* Output row buffer */
			int row_stride;		/* physical row width in output buffer */
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

}

void MedImageCML::loadArrowToStamp(){
	
	ColorData *c;
  
	    png_image image;
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, "marker_smaller.png")) {
        
			m_stampBuffer = new PixelBuffer(image.width, image.height, m_displayBuffer->getBackgroundColor());
			
			png_bytep buffer;
			image.format = PNG_FORMAT_RGBA;
			buffer = new png_byte[PNG_IMAGE_SIZE(image)];

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


int MedImageCML::write_JPEG_file ()
{
	struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;

    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    
    if ((outfile = fopen(m_outputFileName.c_str(), "wb")) == NULL) {
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

int MedImageCML::write_PNG_file(){
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
			
	if (png_image_write_to_file(&image, m_outputFileName.c_str(), 0/*convert_to_8bit*/,
		buffer, 0/*row_stride*/, NULL/*colormap*/) != 0) {
		return 1;
	}
	else {
		return -1;
	}

}
void MedImageCML::saveCanvasToFile()
{

    unsigned last_dot_pos = m_outputFileName.find_last_of(".\\");
	std::string extension = m_outputFileName.substr(last_dot_pos + 1);
	
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


void MedImageCML::applyFilterThreshold()
{
  
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
}

void MedImageCML::applyFilterMultiplyRGB()
{
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
    
    
}

void MedImageCML::applyFilterSaturate()
{
    
    ColorData current_pixel;
	float red, green, blue, luminance;
	float amount = m_filterParameters.saturation_amount;

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
}

void MedImageCML::applyFilters(int filterTool){
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
		case 3:
			amount = m_filterParameters.blur_amount;
			kwidth = (int) amount;
			kwidth = (kwidth % 2 != 0)? kwidth : kwidth + 1;
			filter = new Blur_Filter(kwidth);
			break;
		default:
			break;
	}
    
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


void MedImageCML::applyFilterSharpen()
{
 
    applyFilters(1);
}

void MedImageCML::applyFilterEdgeDetect() {

    applyFilters(2);
}

void MedImageCML::applyFilterBlur()
{
    
    applyFilters(3);
    
}

void MedImageCML::applyFilterQuantize() {

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
}


bool MedImageCML::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}

bool MedImageCML::isValidImageFileName(const std::string & name) {
    
    if (hasSuffix(name, ".png")
        || hasSuffix(name, ".jpg")
        || hasSuffix(name, ".jpeg")
        )
        return true;
    else
        return false;
}

bool MedImageCML::isValidImageFile(const std::string & name) {
    
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

std::string MedImageCML::getImageNamePlusSeqOffset(const std::string & currentFileName, int offset)
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

void MedImageCML::setImageFile(const std::string & fileName)
{
    // If a directory was selected
    // instead of a file, use the
    // latest file typed or selected.
    std::string imageFile = fileName;
    if (!isValidImageFileName(imageFile)) {
        imageFile = m_fileName;
    }
    
    if (!isValidImageFileName(imageFile)) {
        printf("Not a valid image file name -> won't be saved!\n");

    } 
    else {
        printf("Will save image: %s\n", imageFile.c_str());
    }
}
void MedImageCML::applyStampToCanvas(int x, int y){
	
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

void MedImageCML::argument_check(int argc, char* argv[]){
	
	if (strcmp(argv[1], "-h") == 0){
		std::cout << "  -h -> This help message. " << std::endl;
		std::cout << "  -sharpen <int> " << std::endl;
		std::cout << "  -edgedetect " << std::endl;
		std::cout << "  -thresh <float> " << std::endl;
		std::cout << "  -quantize <int> " << std::endl;
		std::cout << "  -blur <float> " << std::endl;
		std::cout << "  -saturate <float> " << std::endl;
		std::cout << "  -multrgb <float> <float> <float>" << std::endl;
		std::cout << "  -stamp <int> <int> " << std::endl;
		std::cout << "  -compare" << std::endl;

	}
	
	else if(isSequence(argv[1], argv[argc -1]) == true){
		processSequence(argc, argv);
	}
	else {
		processSingle(argc, argv);
	}
	
}

bool MedImageCML::compareImages(char* file2){
	bool is_same = true;
	PixelBuffer * temp = new PixelBuffer(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(), m_displayBuffer->getBackgroundColor());
	m_displayBuffer->copyPixelBuffer(m_displayBuffer, temp);
	m_fileName = file2;
	loadImageToCanvas();
	int width = m_displayBuffer->getWidth();
	int height = m_displayBuffer->getHeight();
	if(width != temp->getWidth() || height != temp->getHeight()){
		is_same = false;
	}
	else{
		ColorData currentPixel;
		ColorData comparePixel;
		for(int i = 0; i < width; i++){
			if(is_same == false){
				break;
			}
			for(int j = 0; j < height; j++){
				currentPixel = temp->getPixel(i,j);
				comparePixel = m_displayBuffer->getPixel(i,j);
				if(currentPixel.getRed() != comparePixel.getRed()){
					is_same = false;
				}
				if(currentPixel.getGreen() != comparePixel.getGreen()){
					is_same = false;
				}
				if(currentPixel.getBlue() != comparePixel.getBlue()){
					is_same = false;
				}
				if(currentPixel.getAlpha() != comparePixel.getAlpha()){
					is_same = false;
				}
				
				
				
				if(is_same == false){
					break;
				}
			}				
		}
	}
	return is_same;
}


bool MedImageCML::isSequence(char* file1, char* file2){
	std::string input = file1;
	std::string output = file2;
	
	int hash_pos1 = input.find("###");
	int hash_pos2 = output.find("###");
	if(hash_pos1 < 0 || hash_pos2 < 0){
		return false;
	}
	else{
		return true;
	}
}

void MedImageCML::processSingle(int argc, char* argv[]){
	
	int x;
	int y;
	int i = 2;
	bool should_save = true;
	bool is_same = true;

	if(isValidImageFile(argv[1]) == true && isValidImageFileName(argv[argc-1]) == true){
		m_fileName = argv[1];
		m_outputFileName = argv[argc -1];
		loadImageToCanvas();
		while(i < argc-1){
			if(strcmp(argv[i], "-sharpen") == 0){
				cout << "Sharpening " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0){
					m_filterParameters.sharpen_amount = atoi(argv[i+1]); 
					applyFilterSharpen();
					i += 2;
				}
				else{					
					printf("%s isn't a valid integer for sharpen amount!\n", argv[i+1]);
					i++;
					should_save = false;
				}
			}
			else if(strcmp(argv[i], "-edgedetect") == 0){
					cout << "Edgedetecting " << argv[argc-1] << endl;
					applyFilterEdgeDetect();
					i++;
			}
			else if(strcmp(argv[i], "-thresh") == 0){
				cout << "Thresholding " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0){
					m_filterParameters.threshold_amount = atof(argv[i+1]); 
					applyFilterThreshold();
					i += 2;
				}
				else{					
					printf("%s isn't a valid float for threshhold amount!\n", argv[i+1]);
					i++;
					should_save = false;
				}	
			}
			else if(strcmp(argv[i], "-quantize") == 0){
				cout << "Quantizing " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0){
					m_filterParameters.quantize_bins = atoi(argv[i+1]); 
					applyFilterQuantize();
					i += 2;
				}
				else{					
					printf("%s isn't a valid integer for quantize bins amount!\n", argv[i+1]);
					i++;
					should_save = false;
				}
			}
			else if(strcmp(argv[i], "-blur") == 0){
				cout << "Blurring " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0){
					m_filterParameters.blur_amount = atof(argv[i+1]); 
					applyFilterBlur();
					i += 2;
				}
				else{					
					printf("%s isn't a valid float for blur amount!\n", argv[i+1]);
					i++;
					should_save = false;
				}
			}
			else if(strcmp(argv[i], "-saturate") == 0){
				cout << "Saturating " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0){
					m_filterParameters.saturation_amount = atof(argv[i+1]); 
					applyFilterSaturate();
					i += 2;
				}
				else{					
					printf("%s isn't a valid float for saturation amount!\n", argv[i+1]);
					i++;
					should_save = false;
				}
			}
			
			else if(strcmp(argv[i], "-multrgb") == 0){
				cout << "Multrgb " << argv[argc-1] << endl;
				if(isdigit(*argv[i+1]) > 0 && isdigit(*argv[i+2]) > 0 && isdigit(*argv[i+3]) > 0){
					m_filterParameters.multiply_colorRed = atof(argv[i+1]);
					m_filterParameters.multiply_colorGreen = atof(argv[i+2]);
					m_filterParameters.multiply_colorBlue = atof(argv[i+3]);
					applyFilterMultiplyRGB();
					i += 4;
				}
				else{					
					printf("One (or more) of the three arguments %s , %s, %s  is not a valid float for multrgb amounts!\n", argv[i+1], argv[i+2], argv[i+3]);
					i++;
					should_save = false;
				}	
			}
			else if(strcmp(argv[i], "-stamp") == 0){
				cout << "Stamping " << argv[argc-1] << endl;
				
				if(isdigit(*argv[i+1]) > 0 && isdigit(*argv[i+2]) > 0){
					x = atoi(argv[i+1]);
					y = atoi(argv[i+2]);
					loadArrowToStamp();
					applyStampToCanvas(x, y);
					i+=3;
				}
				else{				
					printf("Either position x or position y is not integers in the arguments %s and %s \n", argv[i+1], argv[i+2]);
					i++;
					should_save = false;
				}
			}
			
			else if(strcmp(argv[i], "-compare") == 0){
					//std::cout << "Comparing " << argv[1] << " and " << argv[argc-1] << endl;
					should_save = false;
					is_same = true;
					
					if(isValidImageFile(argv[i+1]) == false){
						printf("%s is not a valid image file\n", argv[i+1]);
						is_same = false;
					}
					else{
						is_same = compareImages(argv[i+1]);
					}
					if(is_same == true){
//						printf("Two files are the same!\n");
						printf(" PASSED 1\n");
					}
					else{
//						printf("Two files are different!\n");
						printf(" FAILED 0\n");
					}
					i+=2;
			}
			
			else{
				i++;
			}
			
		}
		
		if(should_save == true){
			printf("Saving %s\n", argv[argc-1]);
			saveCanvasToFile();
		}
		
	}
	else{
		printf("Either input file %s or output file %s is not a valid image!\n", argv[1], argv[argc-1]);
	}
	
	
	
	
}

void MedImageCML::processSequence(int argc, char* argv[]){
	std::string pattern;
	std::string input = argv[1];
	std::string output = argv[argc-1];
	int hash_pos1 = input.find("###");
	int hash_pos2 = output.find("###");
	int last_dot_in = input.find_last_of(".\\");
	int last_dot_out = output.find_last_of(".\\");
	std::string extension_in = input.substr(last_dot_in + 1);
	std::string extension_out = output.substr(last_dot_out + 1);

	for(int i = 0; i<1000; i++){
			
		char buffer[4];
		snprintf(buffer, sizeof(buffer), "%03d", i);
		pattern = buffer;
		input = input + pattern  + "." + extension_in;
		output = output + pattern + "." + extension_out;
		
		if(isValidImageFile(input.c_str()) == true){
			
			char * in = new char[input.size() + 1];
			char * out = new char[output.size() + 1];
			std::copy(input.begin(), input.end(), in);
			std::copy(output.begin(), output.end(), out);
			in[input.size()] = '\0';
			out[output.size()] = '\0';
			argv[1] = in;
			argv[argc-1] = out;
			processSingle(argc, argv);
			delete[] in;
			delete[] out;
		}
		memset(buffer, 0, sizeof(buffer));
		input.resize(hash_pos1,' ');
		output.resize(hash_pos2,' ');
	}
	

}































