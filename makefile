CC     = clang
CFLAGS = -Wall -pedantic -g -Iinclude -I/usr/local/include/
OFILES = src/tests/test-Worlds.c src/tests/ui.o src/tests/items.o src/tests/levels.o src/tests/actors.o src/tests/abilities.o src/tests/components/health.o src/tests/components/physics.o src/world.o src/input.o src/entity.o src/scene.o src/sprite.o src/timestep.o src/worldPosition.o src/component.o src/system.o src/framework/list.o src/framework/drawing.o src/framework/fonts.o src/framework/strings.o src/framework/numbers.o src/framework/display.o src/framework/sprite.o
LDFLAGS = -Wl,-rpath,. -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -L. -lm
DEBUGLDFLAGS = -Wl,-rpath,. -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -L. -lm
DEBUGLDFLAGS-PROF = -Wl,-rpath,. -lprofiler -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -L. -lm
BINNAME = bin/testWorlds

all: cgame
debug: cgame-debug
profile: cgame-profile

cgame: $(OFILES)
		$(CC) $(LDFLAGS) -o $(BINNAME) $^

cgame-debug: $(OFILES)
		$(CC) $(DEBUGLDFLAGS) -O0 -o $(BINNAME) $^

cgame-profile: $(OFILES)
		$(CC) $(DEBUGLDFLAGS-PROF) -O0 -o $(BINNAME) $^
		
clean:
	rm $(BINNAME) src/*.o src/framework/*.o src/tests/*.o
