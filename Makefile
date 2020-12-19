BigBrother: bin/BigBrother

bin/BigBrother: build/main.o build/BigBrother.o build/BigBrotherClient.o
	g++ -Wall -g build/main.o build/BigBrother.o build/BigBrotherClient.o -o bin/BigBrother -liphlpapi -lws2_32 -lgdi32 -lopengl32

build/main.o: src/main.cpp
	g++ -Wall -c -g src/main.cpp -o build/main.o

build/BigBrother.o: src/BigBrother.cpp
	g++ -Wall -c -g src/BigBrother.cpp -o build/BigBrother.o

build/BigBrotherClient.o: src/BigBrotherClient.cpp
	g++ -Wall -c -g src/BigBrotherClient.cpp -o build/BigBrotherClient.o



KeyLogger: bin/KeyLogger

bin/KeyLogger: build/BigBrotherClient.o build/KeyLogger.o
	g++ -Wall -g build/BigBrotherClient.o build/KeyLogger.o -o bin/KeyLogger -liphlpapi -lws2_32 -lgdi32 -lopengl32

build/KeyLogger.o: src/KeyLogger.cpp
	g++ -Wall -c -g src/KeyLogger.cpp -o build/KeyLogger.o



BigBrotherServer: bin/BigBrotherServer

bin/BigBrotherServer: build/BigBrotherServer.o
	g++ -Wall -g build/BigBrotherServer.o -o bin/BigBrotherServer -liphlpapi -lws2_32 -lgdi32 -lopengl32

build/BigBrotherServer.o: src/BigBrotherServer.cpp
	g++ -Wall -c -g src/BigBrotherServer.cpp -o build/BigBrotherServer.o


clean:
	rm -rf build/*.o bin/BigBrother bin/BigBrotherServer bin/KeyLogger
