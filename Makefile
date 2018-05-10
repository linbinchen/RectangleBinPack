# Makefile
# copyright 2013, Stefan Beller
# This file is also public domain.

OBJS = MaxRectsBinPackTest/HybMatLayout.o  MaxRectsBinPack.o  Rect.o
CXX = g++
# If you want to compile it faster try these options (gcc only)
# CPPFLAGS = -march=native -mtune=native -Ofast -g -flto -Wall -Wextra
# LDFLAGS = -march=native -mtune=native -Ofast -g -flto -fwhole-program

CPPFLAGS = -std=c++11 -I/opt/X11/include
LDFLAGS  = -L/opt/X11/lib -lplotter -lXaw -lXt -lSM -lICE -lXext -lX11 -lpng -lz -lm

all: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(OBJS) -o HybMatLayout
