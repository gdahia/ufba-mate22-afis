CXX = g++

CXXFLAGS = -std=c++11

LDFLAGS = `pkg-config --libs opencv`

INCLUDE = `pkg-config --cflags opencv` -I $(CURDIR)/include

all: build/normalize.o build/clahe.o build/otsu_binarization.o build/extract_minutiae.o \
	build/orientation_image.o build/wavelength_image.o build/gabor.o \
	build/guo_thinning.o build/hong_enhancement.o build/minutiae_extractor.o \
	build/save_minutiae.o extract_minutiae.out

build/%.o: src/%.cpp include/afis.hpp
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDE) -o $@

extract_minutiae.out: build/extract_minutiae.o build/otsu_binarization.o \
	build/normalize.o build/clahe.o	build/orientation_image.o \
	build/wavelength_image.o build/gabor.o build/guo_thinning.o\
	build/hong_enhancement.o build/minutiae_extractor.o build/save_minutiae.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@rm -f build/*.o
