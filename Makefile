all: CImg.h a3.cpp Classifier.h NearestNeighbor.h EigenVec.h BagOfWords.h Baseline.h Deep.h haar.h
	g++ a3.cpp -o a3 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib  -lopencv_core -lopencv_highgui


test: part1.cpp part1.h
	g++ part1.cpp -o part1 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib

clean:
	rm a3
