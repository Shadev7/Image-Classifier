all: CImg.h a3.cpp Classifier.h NearestNeighbor.h EigenVec.h BagOfWords.h
	g++ a3.cpp -o a3 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib  -lopencv_core -lopencv_highgui
clean:
	rm a3
