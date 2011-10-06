# The Makefile
#
# If you're using Windows and you don't know what this file is,
# don't worry about it. Just use Visual C++ Express Edition or
# Dev-C++ to work on your code.

MyTronBot: MyTronBot.o bot.o map.o
	g++ -o MyTronBot MyTronBot.o bot.o map.o

MyTronBot.o: MyTronBot.cc
	g++ -O3 -funroll-loops -c -o MyTronBot.o MyTronBot.cc

bot.o: bot.cpp map.o
	g++ -O3 -funroll-loops -c -o bot.o bot.cpp

map.o: map.cpp
	g++ -O3 -funroll-loops -c -o map.o map.cpp

clean:
	rm -rf *.o MyTronBot

test: test.o
	g++ -O3 -o test test.o

test.o: test.cpp
	g++ -O3 -c -o test.o test.cpp

