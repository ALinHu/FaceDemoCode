# target
TARGET = genFaceFeature

# sources
SOURCES = genFaceFeature.cpp jpegfile.cpp

OBJS = genFaceFeature.o jpegfile.o
INC  = -I../inc
LIBS = -L../lib/linux_x64 -larcsoft_fsdk_face_detection -larcsoft_fsdk_face_recognition -ljpeg

# compile param
CXX = g++
CXXFLAGS = $(INC) -O2 -Wall -std=c++11 -fPIC -m64 -ffunction-sections -fpermissive

# rules
all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LIBS) 
	
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<  
	
clean:
	rm -rf $(OBJS)
	rm -f $(TARGET)
	
.PHONY: clean all mk_dirs

