
/**********************************************************************
 * @file MedImageCML.h
 * @brief Commandline version of MedImageGUI
 * 
 * MedImageCML:
 * 	MedImageCML is a commandline version of MedImageGUI. It is strictly 
 * optimized for commandline functionalities and therefore is a class
 * of its own, unlike being a sub-class of BaseGfxApp in MedImageGUI.
 * 
 * 
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/




#ifndef MEDIMAGE_CML
#define MEDIMAGE_CML
#include <iostream>
#include <string>
#include <png.h>
#include <jpeglib.h>
class ColorData;
class PixelBuffer;


class MedImageCML {
public: // All cmd line methods for available features are open to the public
    
    MedImageCML(int argc, char* argv[], ColorData backgroundColor);
    virtual ~MedImageCML();
    
    void setImageFile(const std::string & filepath);
    bool isValidImageFileName(const std::string & name);
    bool isValidImageFile(const std::string & name);
    bool hasSuffix(const std::string & str, const std::string & suffix);
    std::string getImageNamePlusSeqOffset(const std::string & currentFileName, int offset);
    
    void loadImageToCanvas();
    void loadArrowToStamp();
    void saveCanvasToFile();
    void loadNextImage();
    void loadPreviousImage();
    void applyStampToCanvas(int x, int y);
    
    void applyFilterSharpen();
    void applyFilterEdgeDetect();
    void applyFilterThreshold();
    void applyFilterMultiplyRGB();
    void applyFilterSaturate();
    void applyFilterQuantize();
    void applyFilterBlur();
    void applyFilters(int filterTool);
    
    // COMMANDLINE DECLARATIONS
    void argument_check(int argc, char* argv[]);
    int write_JPEG_file();
    int write_PNG_file();
    bool compareImages(char* file2);
    bool isSequence(char* file1, char* file2);
    void processSequence(int argc, char* argv[]);
    void processSingle(int argc, char* argv[]);
    
    
    
    
	// PRIVATE MEMBER TASKS    
private:    

    void initializeBuffers(ColorData initialColor, int width, int height);

    
    struct {
        float multiply_colorRed;
        float multiply_colorGreen;
        float multiply_colorBlue;
        float threshold_amount;
        float sharpen_amount;
        float blur_amount;
        float saturation_amount;
        int quantize_bins;
    } m_filterParameters;

    //  MEMBER VARAIBLES
    
    // This is the pointer to the buffer where the display PixelBuffer is stored
    PixelBuffer* m_displayBuffer;
    float** mask;
    PixelBuffer* m_stampBuffer;
    
    // These are used to store the selections from the GLUI user interface
    std::string m_fileName;
    std::string m_outputFileName;

};


#endif
