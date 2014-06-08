# Variables
TARGET=example

CPP = clang++
CPPFLAGS = -Wall -std=c++1y
LIBS = -lboost_system -lboost_coroutine


# Makefile scripts
all: release

debug: CPPFLAGS += -DDEBUG -g
debug: $(TARGET)

release: CPPFLAGS += -O3
release: $(TARGET)

$(TARGET): main.cpp types.h helpers.h
	$(CPP) $< -o $@ $(CPPFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)


# Special targets
.PHONY: clean
