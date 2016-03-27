all: CImg.h a3.cpp Classifier.h NearestNeighbor.h Baseline.h
	g++ a3.cpp -o a3 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib

test: part1.cpp part1.h
	g++ part1.cpp -o part1 -lX11 -lpthread -I. -Isiftpp siftpp/sift.cpp -L/usr/X11/lib

clean:
	rm a3
