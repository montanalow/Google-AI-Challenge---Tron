CC = g++-mp-4.3
O = -O3 -funroll-loops
SRC = ./src/
PROFILE = #-fprofile-use
BUILD = ./build/


zug_zug: profile MyTronBot.o bot.o map.o
	$(CC) $(O) $(PROFILE) -o $(BUILD)zug_zug $(BUILD)MyTronBot.o $(BUILD)bot.o $(BUILD)map.o

MyTronBot.o: $(SRC)MyTronBot.cc
	$(CC) -c $(O) -o $(BUILD)MyTronBot.o $(SRC)MyTronBot.cc

bot.o: $(SRC)bot.cpp map.o 
	$(CC) -c $(O) -o $(BUILD)bot.o $(SRC)bot.cpp

map.o: $(SRC)map.cpp
	$(CC) -c $(O) $(PROFILE) -o $(BUILD)map.o $(SRC)map.cpp


profile: profile_zug_zug 
	cd build; ./profile_zug_zug; cp profile_map.gcda map.gcda; cp profile_map.gcno map.gcno
	
profile_zug_zug: profile_zug_zug.o profile_map.o 
	$(CC) $(O) -fprofile-generate -o $(BUILD)profile_zug_zug $(BUILD)profile_zug_zug.o $(BUILD)profile_map.o

profile_zug_zug.o: $(SRC)profile_zug_zug.cpp profile_map.o 
	$(CC) -c $(O) -o $(BUILD)profile_zug_zug.o $(SRC)profile_zug_zug.cpp

profile_map.o: $(SRC)map.cpp 
	$(CC) -c $(O) -fprofile-generate -o $(BUILD)profile_map.o $(SRC)map.cpp



clean:
	rm -rf $(BUILD) cpp.zip; mkdir -p $(BUILD)




test: test.o
	$(CC) $(O) -o $(BUILD)test $(BUILD)test.o $(BUILD)map.o $(BUILD)bot.o

test.o: $(SRC)test.cpp map.o bot.o
	$(CC) $(O) -c -o $(BUILD)test.o $(SRC)test.cpp




benchmark: benchmark.o
	$(CC) -o $(BUILD)benchmark $(BUILD)benchmark.o $(BUILD)map.o $(BUILD)bot.o

benchmark.o: $(SRC)benchmark.cpp map.o bot.o
	$(CC) $(O) -c -o $(BUILD)benchmark.o $(SRC)benchmark.cpp


package: clean zug_zug
	mkdir -p cpp; cp -R $(SRC)MyTronBot.cc $(SRC)bot.h $(SRC)bot.cpp $(SRC)map.h $(SRC)map.cpp $(BUILD)map.gcda $(BUILD)map.gcno ./cpp/ && touch ./cpp/config.h && zip -r cpp.zip cpp; rm -rf ./cpp;