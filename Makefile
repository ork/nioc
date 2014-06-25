EXEC     = nioc
CC       = gcc -fdiagnostics-color=always

CFLAGS   = -std=c99 -O3 -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS  += $(shell pkg-config --cflags gstreamer-interfaces-0.10 gtk+-3.0 girara-gtk3 gstreamer-0.10)
LDFLAGS  = $(shell pkg-config --libs   gstreamer-interfaces-0.10 gtk+-3.0 girara-gtk3 gstreamer-0.10)

SRC      = $(wildcard *.c)
OBJ      = $(SRC:.c=.o)

all: $(EXEC)

${EXEC}: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
			