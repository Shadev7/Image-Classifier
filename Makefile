all: CImg.h a3.cpp Classifier.h NearestNeighbor.h
	g++ a3.cpp -o a3 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib

clean:
	rm a3
