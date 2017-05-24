## A Makefile for Photoshop
# Include nvidia drivers if necessary:
	NVIDIA_LIB = $(wildcard /usr/lib/nvidia-304)
	ifneq ($(NVIDIA_LIB), )
		LINK_LIBS += -L$(NVIDIA_LIB)
	endif

UNAME = $(shell uname)
CC 		= g++
CFLAGS 	= -c -Wall
LDFLAGS =
GLUI = glui
PNG = png16
JPEG = jpeg
LIBPHOTO = libphoto
GLUI_PATH = ./glui
PNG_PATH = ./libpng-1.6.16
JPEG_PATH = ./jpeg-9a
LIBPHOTO_PATH = ./libphoto
LIBPHOTO_LIB = $(LIBPHOTO_PATH)/lib/libphoto.a
GLUI_LIB = $(GLUI_PATH)/lib/lib$(GLUI).a
PNG_LIB = $(PNG_PATH)/lib/lib$(PNG).a
JPEG_LIB = $(JPEG_PATH)/lib/lib$(JPEG).a
LINK_LIBS += -L./$(GLUI)/lib/ -l$(GLUI)
LINK_LIBS += -L$(PNG_PATH)/lib/ -l$(PNG)
LINK_LIBS += -L$(JPEG_PATH)/lib/ -l$(JPEG)
LINK_LIBS += -L$(LIBPHOTO_PATH)/lib/ -lphoto
LINK_LIBS += -lz
PHOTOSHOP_OBJECT_DIR = Photoshop/object_files
PHOTOSHOP_SOURCE_DIR = Photoshop/src
MEDIMAGE_OBJECT_DIR = MedImage/object_files
MEDIMAGE_SOURCE_DIR = MedImage/src
TEST_OBJECTS_DIR = tests/object_files
TEST_SOURCE_DIR = tests/src

INCLUDE = -I./$(GLUI)/include
INCLUDE += -I./$(PNG_PATH)/
INCLUDE += -I./$(JPEG_PATH)/
INCLUDE += -I./$(LIBPHOTO_PATH)/include
INCLUDE += -I./$(PHOTOSHOP_SOURCE_DIR)/ # for testing
INCLUDE += -I./$(MEDIMAGE_SOURCE_DIR)/ # for testing

PHOTOSHOP_OBJECTS = $(addprefix $(PHOTOSHOP_OBJECT_DIR)/,  $(PHOTOSHOP_FILENAMES:.cpp=.o)) 
PHOTOSHOP_CPPFILES = $(wildcard $(PHOTOSHOP_SOURCE_DIR)/*.cpp)
PHOTOSHOP_FILENAMES =  $(notdir $(PHOTOSHOP_CPPFILES))

MEDIMAGE_OBJECTS =  $(addprefix $(MEDIMAGE_OBJECT_DIR)/,  $(MEDIMAGE_FILENAMES:.cpp=.o))
MEDIMAGE_CPPFILES = $(wildcard $(MEDIMAGE_SOURCE_DIR)/*.cpp)
MEDIMAGE_FILENAMES =  $(notdir $(MEDIMAGE_CPPFILES))

TEST_OBJECTS =  $(addprefix $(TEST_OBJECTS_DIR)/,  $(TEST_FILENAMES:.cpp=.o))
TEST_CPPFILES = $(wildcard $(TEST_SOURCE_DIR)/*.cpp)
TEST_FILENAMES =  $(notdir $(TEST_CPPFILES))

ifeq ($(UNAME), Darwin) # Mac OSX 
	LINK_LIBS += -framework glut -framework opengl
	PHOTOSHOP_EXECUTABLE = Photoshop/photoshop
	MEDIMAGE_EXECUTABLE = MedImage/medimage
	TEST_EXECUTABLE = tests/test
else # LINUX
	LINK_LIBS += -lglut -lGL -lGLU
	PHOTOSHOP_EXECUTABLE = Photoshop/photoshop
	MEDIMAGE_EXECUTABLE = MedImage/medimage
	TEST_EXECUTABLE = tests/test
endif


build: setup $(LIBPHOTO_LIB) $(PHOTOSHOP_EXECUTABLE) $(MEDIMAGE_EXECUTABLE) $(TEST_EXECUTABLE)

rebuild: clean build


# Create the object_files directory only if it does not exist. 
setup: | $(PHOTOSHOP_OBJECT_DIR) $(MEDIMAGE_OBJECT_DIR) $(TEST_OBJECTS_DIR)

$(PHOTOSHOP_OBJECT_DIR): 
	mkdir -p $(PHOTOSHOP_OBJECT_DIR)
	
$(MEDIMAGE_OBJECT_DIR): 
	mkdir -p $(MEDIMAGE_OBJECT_DIR)
	
$(TEST_OBJECTS_DIR):
	mkdir -p $(TEST_OBJECTS_DIR)
	mkdir -p tests/generated

$(PHOTOSHOP_EXECUTABLE): $(GLUI_LIB) $(LIBPHOTO_LIB) $(PHOTOSHOP_OBJECTS)
	$(CC) $(LDFLAGS) $(PHOTOSHOP_OBJECTS) $(LINK_LIBS) -o $@

$(MEDIMAGE_EXECUTABLE): $(GLUI_LIB) $(LIBPHOTO_LIB) $(MEDIMAGE_OBJECTS)
	$(CC) $(LDFLAGS) $(MEDIMAGE_OBJECTS) $(LINK_LIBS) -o $@

# Explicitly defined build targets because of dependancy on other header files
$(PHOTOSHOP_OBJECT_DIR)/PhotoShop.o: $(PHOTOSHOP_SOURCE_DIR)/PhotoShop.cpp $(PHOTOSHOP_SOURCE_DIR)/PhotoShop.h
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<
	
$(PHOTOSHOP_OBJECT_DIR)/main.o: $(PHOTOSHOP_SOURCE_DIR)/main.cpp $(PHOTOSHOP_SOURCE_DIR)/PhotoShop.h
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<


# Pattern build targets
$(PHOTOSHOP_OBJECT_DIR)/%.o: $(PHOTOSHOP_SOURCE_DIR)/%.cpp $(PHOTOSHOP_SOURCE_DIR)/%.h
	#@echo 'Compiling arbitrary .cpp file with .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

$(PHOTOSHOP_OBJECT_DIR)/%.o: $(PHOTOSHOP_SOURCE_DIR)/%.cpp
	#@echo 'Compiling arbitrary .cpp file without .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<


########################################################################

	
# Explicitly defined build targets because of dependancy on other header files
$(MEDIMAGE_OBJECT_DIR)/MedImageGUI.o: $(MEDIMAGE_SOURCE_DIR)/MedImageGUI.cpp $(MEDIMAGE_SOURCE_DIR)/MedImageGUI.h
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<
	
$(MEDIMAGE_OBJECT_DIR)/MedImageCML.o: $(MEDIMAGE_SOURCE_DIR)/MedImageCML.cpp $(MEDIMAGE_SOURCE_DIR)/MedImageCML.h
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

$(MEDIMAGE_OBJECT_DIR)/main.o: $(MEDIMAGE_SOURCE_DIR)/main.cpp $(MEDIMAGE_SOURCE_DIR)/MedImageGUI.h $(MEDIMAGE_SOURCE_DIR)/MedImageCML.cpp $(MEDIMAGE_SOURCE_DIR)/MedImageCML.h 
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

# Pattern build targets
$(MEDIMAGE_OBJECT_DIR)/%.o: $(MEDIMAGE_SOURCE_DIR)/%.cpp $(MEDIMAGE_SOURCE_DIR)/%.h
	#@echo 'Compiling arbitrary .cpp file with .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

$(MEDIMAGE_OBJECT_DIR)/%.o: $(MEDIMAGE__SOURCE_DIR)/%.cpp
	#@echo 'Compiling arbitrary .cpp file without .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

########################################################################

$(TEST_EXECUTABLE): $(GLUI_LIB) $(LIBPHOTO_LIB) $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS_DIR)/main.o $(PHOTOSHOP_OBJECT_DIR)/PhotoShop.o $(MEDIMAGE_OBJECT_DIR)/MedImageCML.o $(LINK_LIBS) -o $@
	
$(TEST_OBJECTS_DIR)/main.o: $(TEST_SOURCE_DIR)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<	

clean:
	\rm -rf $(MEDIMAGE_OBJECT_DIR) $(PHOTOSHOP_OBJECT_DIR) $(PHOTOSHOP_EXECUTABLE) $(MEDIMAGE_EXECUTABLE) $(TEST_OBJECTS_DIR) $(TEST_EXECUTABLE) tests/generated

cleanglui:	
	 $(MAKE)  -C ./$(GLUI)/ clean

$(GLUI_LIB): 
	$(MAKE) -C ./$(GLUI)/ all
	
$(LIBPHOTO_LIB):
	$(MAKE) -C ./$(LIBPHOTO)/
