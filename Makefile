CC = gcc
SDL2_PATH = /opt/sdl2/lin/
OPT = 0
DBG = 3
MYCFLAGS =
MYLDFLAGS =
GIT_VERSION = $(shell git rev-parse --short HEAD)-$(shell date +%y%m%d)
CFLAGS = -std=c99 -pedantic $(shell $(SDL2_PATH)/bin/sdl2-config --cflags) -DGIT_VERSION="\"$(GIT_VERSION)\"" $(MYCFLAGS) 
LDFLAGS = $(DYNAMIC) $(shell $(SDL2_PATH)/bin/sdl2-config --libs) -lm $(MYLDFLAGS)
OBJDIR = obj
OUT = augshot

.PHONY: clean
default: release

release: CFLAGS += -O2 -g0
release: $(OUT)

debug: CFLAGS += -O0 -g3
debug: $(OUT)

SRC = $(wildcard src/*.c)
HDR = $(wildcard src/*.h)
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

$(OBJDIR)/%.o: src/%.c $(HDR)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) -c $<

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LDFLAGS)

clean:
	@rm -rf $(OUT) $(OBJDIR)
