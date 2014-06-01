# Variables
TARGET=example

CPP = clang++
CPPFLAGS = -Wall -std=c++1y
LIBS = -lboost_system


# Makefile scripts
$(TARGET): main.cpp types.h
	$(CPP) $< -o $@ $(CPPFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)


# Special targets
.PHONY: clean
