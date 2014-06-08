# Variables
TARGET = example

CPP = clang++
CPPFLAGS = -Wall -std=c++1y
INC = -I./../include
LIB = -lboost_system -lboost_coroutine


# Makefile scripts
all: release

debug: CPPFLAGS += -DDEBUG -g
debug: $(TARGET)

release: CPPFLAGS += -O3
release: $(TARGET)

$(TARGET): main.cpp
	$(CPP) $< -o $@ $(CPPFLAGS) $(LIB) $(INC)

clean:
	rm -f $(TARGET)


# Special targets
.PHONY: all debug release clean
