all: Source.o StringCompareUtil.o
	g++ -o typo Source.o StringCompareUtil.o

Source.o: Source.cpp
	g++ -c Source.cpp

StringCompareUtil.o: StringCompareUtil.cpp
	g++ -std=c++0x -c StringCompareUtil.cpp -fno-deduce-init-list

clean:
	rm -f typo *.o
