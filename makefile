CC     = clang
CFLAGS = -Wall -pedantic -g -Iinclude -I/usr/local/include/
OFILES = component.o entity.o input.o scene.o sprite.o system.o timestep.o world.o worldPosition.o framework/display.o framework/drawing.o framework/fonts.o framework/list.o framework/numbers.o framework/sprite.o framework/strings.o
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm
DEBUGLDFLAGS = -Wl,-rpath,. -L. -lm
DEBUGLDFLAGS-PROF = -Wl,-rpath,. -lprofiler -L. -lm
BINNAME = bin/collage.o

all: cgame
debug: cgame-debug
profile: cgame-profile

cgame: $(OFILES)
		$(CC) $^

cgame-debug: $(OFILES)
		$(CC) $(DEBUGLDFLAGS) -O0 -o $(BINNAME) $^

cgame-profile: $(OFILES)
		$(CC) $(DEBUGLDFLAGS-PROF) -O0 -o $(BINNAME) $^
		
clean:
	rm $(BINNAME) *.o framework/*.o
