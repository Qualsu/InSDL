COMPILE = g++ src/main.cpp -g -o build/build.exe -lSDL3 -lSDL3_ttf -lSDL3_Image -Iheader -Llib -Iinclude -Iinclude/InSDL
RUN = build/build.exe
REMOVE = rm -f build/*.exe

default:
	${COMPILE}
	${RUN}

compile:
	${COMPILE}

run:
	${RUN}

clean:
	${REMOVE}