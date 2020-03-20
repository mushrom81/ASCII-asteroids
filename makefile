ASCIIsteroids.exe: terminalCanvas.o getKey.o ASCIIsteroids.o chars.txt
	x86_64-w64-mingw32-gcc -O3 -mtune=native -o ASCIIsteroids.exe terminalCanvas.o getKey.o ASCIIsteroids.o

ASCIIsteroids.o: ASCIIsteroids.c ASCIIsteroids.h
	x86_64-w64-mingw32-gcc -c -O3 -mtune=native -o ASCIIsteroids.o ASCIIsteroids.c

getKey.o: getKey.c getKey.h
	x86_64-w64-mingw32-gcc -c -O3 -mtune=native -o getKey.o getKey.c

terminalCanvas.o: fastTerminalCanvas.c fastTerminalCanvas.h
	x86_64-w64-mingw32-gcc -c -O3 -mtune=native -o terminalCanvas.o fastTerminalCanvas.c

chars.txt: visualChars.txt
	python formatChars.py