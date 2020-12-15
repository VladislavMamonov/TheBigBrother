# TheBigBrother
Compiling:

g++ -Wall -g .\BigBrotherServer.cpp -o BigBrotherServer -liphlpapi -lws2_32 -lgdi32 -lopengl32

g++ -Wall -g BigBrotherClient.cpp KeyLogger.cpp -o KeyLogger.exe -liphlpapi -lws2_32 -lgdi32 -lopengl32

g++ -Wall -g main.cpp BigBrother.cpp -o BigBrother.exe -liphlpapi -lws2_32 -lgdi32 -lopengl32
