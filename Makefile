.PHONY: clean lib

CC = gcc
CPP = g++
SRC = lib
LIB = libvl53l1_api.a
LDFLAGS :=
CFLAGS := -Ilib/core -Ilib/platform -Ilib/i2c -std=gnu99
CPPFLAGS := -Ilib/core -Ilib/platform -Ilib/i2c


SRC_FILES := $(wildcard $(SRC)/*/*.c)
SRC_FILES := $(wildcard $(SRC)/*/*.c)
OBJ_FILES := $(patsubst $(SRC)/*/*.c,$(SRC)/*/%.o,$(SRC_FILES))

$(LIB): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(CFLAGS) -fPIC -shared -o $@ $^

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(LDFLAGS) $(CFLAGS) -c -o $@ $<

test: test.cpp
	$(CPP) $(CPPFLAGS) $(LDFLAGS) -Wl,-rpath ./lib -L./lib  -lvl53l1_api -o test test.cpp
