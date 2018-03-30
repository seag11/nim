CFLAGS = -std=c++0x -lncurses

nim: nim.cpp
	g++ $(CFLAGS) nim.cpp -o nim

clean:
	rm nim

#target: dependencies
#	action
