CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -Iinclude -D_GNU_SOURCE -MMD -MP
TARGET  = jogo
SOURCES = src/main.c src/game.c src/ui.c src/input.c src/ai_client.c src/audio.c src/score.c src/narrativa.c src/conquistas.c
OBJECTS = $(SOURCES:.c=.o)
DEPS    = $(OBJECTS:.o=.d)

all: $(TARGET)


TEST_OBJS = $(filter-out src/main.o,$(OBJECTS))

test: $(TEST_OBJS) tests/test_runner.c
	$(CC) $(filter-out -MMD -MP,$(CFLAGS)) -o tests/test_runner tests/test_runner.c $(TEST_OBJS)
	./tests/test_runner

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Compilacao concluida!"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

run: all
	./$(TARGET)

clean:
	rm -f src/*.o src/*.d $(TARGET) jogo_gui
	@echo "Limpeza concluida!"

# ---- Interface grafica (raylib) - PI2-189 -----------------------------------
# Requer a raylib instalada (veja o README). Usa ui_raylib.c no lugar de ui.c/input.c.
GUI_SOURCES = src/main.c src/game.c src/ui_raylib.c src/ai_client.c src/audio.c src/score.c src/narrativa.c src/conquistas.c

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
  HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
  GUI_CFLAGS = $(filter-out -MMD -MP,$(CFLAGS)) -I$(HOMEBREW_PREFIX)/include -DUSE_RAYLIB
  GUI_LIBS   = -L$(HOMEBREW_PREFIX)/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm
else
  GUI_CFLAGS = $(filter-out -MMD -MP,$(CFLAGS)) \
               $(shell pkg-config --cflags raylib 2>/dev/null || echo -I/usr/local/include -I/usr/include) \
               -DUSE_RAYLIB
  GUI_LIBS   = $(shell pkg-config --libs raylib 2>/dev/null || echo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11)
endif

gui: $(GUI_SOURCES)
	$(CC) $(GUI_CFLAGS) -o jogo_gui $(GUI_SOURCES) $(GUI_LIBS)
	@echo "Interface grafica compilada! Execute: ./jogo_gui"

run-gui: gui
	./jogo_gui
