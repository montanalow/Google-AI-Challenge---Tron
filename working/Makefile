CC = g++-mp-4.3
O = -O2

zug_zug: MyTronBot.o bot.o map.o util.o
	$(CC) -o zug_zug MyTronBot.o bot.o map.o util.o

MyTronBot.o: MyTronBot.cc
	$(CC) -c $(O) -o MyTronBot.o MyTronBot.cc

bot.o: bot.cpp map.o util.o
	$(CC) -c $(O) -o bot.o bot.cpp

map.o: map.cpp util.o
	$(CC) -c $(O) -o map.o map.cpp

util.o: util.cpp
	$(CC) -c $(O) -o util.o util.cpp




clean:
	rm -rf *.o zug_zug benchmark test cpp cpp.zip




test: test.o
	$(CC) -o test test.o map.o util.o bot.o

test.o: test.cpp map.o bot.o util.o
	$(CC) $(O) -c -o test.o test.cpp




benchmark: benchmark.o
	$(CC) -o benchmark benchmark.o map.o util.o bot.o

benchmark.o: benchmark.cpp map.o bot.o util.o
	$(CC) $(O) -c -o benchmark.o benchmark.cpp


package:
	rm cpp.zip; mkdir cpp; cp -R MyTronBot.cc bot.h bot.cpp map.h map.cpp util.h util.cpp maps ./cpp/ && zip -r cpp.zip cpp; rm -rf ./cpp;