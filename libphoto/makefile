UNAME = $(shell uname)

CC = g++

OPTS=-g
OPTS=-O0



CPPFLAGS += $(OPTS) -Wall
CPPFLAGS += -I./ -I./include

LIBPHOTO = -L./lib -llibphoto

CPPFILES += BaseGfxApp.cpp
CPPFILES += ColorData.cpp
CPPFILES += PixelBuffer.cpp
CPPFILES +=	Blur.cpp
CPPFILES += Blur_Filter.cpp
CPPFILES += CalPen.cpp
CPPFILES += EdgeDetect_Filter.cpp
CPPFILES += Eraser.cpp
CPPFILES += Filter.cpp
CPPFILES += Highlight.cpp
CPPFILES += MotionBlur_Filter.cpp
CPPFILES += Pen.cpp
CPPFILES += Sharpen_Filter.cpp
CPPFILES += Special.cpp
CPPFILES += SprayCan.cpp
CPPFILES += Tools.cpp

SOURCES=$(addprefix ./src/,  $(CPPFILES))

OBJECTS = $(addprefix ./build/,  $(CPPFILES:.cpp=.o)) 

LIBPHOTO_LIB = lib/libphoto.a

all: setup $(LIBPHOTO_LIB) 

setup: build lib

build:
	mkdir -p build
	mkdir -p lib

$(LIBPHOTO_LIB): $(OBJECTS)
	ar -r $(LIBPHOTO_LIB) $(OBJECTS)

build/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

.o:
	$(CXX) $(CPPFLAGS) -c $<

clean:
	\rm -rf build lib
