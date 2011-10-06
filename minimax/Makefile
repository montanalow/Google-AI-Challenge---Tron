# The Makefile
#
# If you're using Windows and you don't know what this file is,
# don't worry about it. Just use Visual C++ Express Edition or
# Dev-C++ to work on your code.

MyTronBot: MyTronBot.o Map.o
	g++ -O3 -funroll-loops -o MyTronBot MyTronBot.o Map.o

MyTronBot.o: MyTronBot.cc
	g++ -O3 -funroll-loops -c -o MyTronBot.o MyTronBot.cc

Map.o: Map.cc
	g++ -O3 -funroll-loops -c -o Map.o Map.cc

clean:
	rm -rf *.o a.out MyTronBot