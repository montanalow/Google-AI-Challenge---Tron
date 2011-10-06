CC = g++-mp-4.3
O = -O3

zug_zug: main.o bot.o map.o util.o
	$(CC) -o zug_zug main.o bot.o map.o util.o

main.o: main.cpp
	$(CC) -c $(O) -o main.o main.cpp

bot.o: bot.cpp map.o util.o
	$(CC) -c $(O) -o bot.o bot.cpp

map.o: map.cpp util.o
	$(CC) -c $(O) -o map.o map.cpp

util.o: util.cpp
	$(CC) -c $(O) -o util.o util.cpp




clean:
	rm -rf *.o zug_zug benchmark test




test: test.o
	$(CC) -o test test.o map.o util.o bot.o

test.o: test.cpp map.o bot.o util.o
	$(CC) $(O) -c -o test.o test.cpp




benchmark: benchmark.o
	$(CC) -o benchmark benchmark.o map.o util.o bot.o

benchmark.o: benchmark.cpp map.o bot.o util.o
	$(CC) $(O) -c -o benchmark.o benchmark.cpp


intall:
	rm ../cpp/* && cp -R main.cpp bot.h bot.cpp map.h map.cpp util.h util.cpp benchmark.h maps ./cpp