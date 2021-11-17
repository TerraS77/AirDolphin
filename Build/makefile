# SmartList
moduleList = AirManager \ bddManager \ Control \ Renderer \ SmarLit
subModuleList = AirManager \ cmdprint \ bddManager \ Control \ Renderer \ SmarLit

SL = AirManager \ cmdPrint

# AirManager
S_SG = ../Source/SmartGrid
B_SG = ./SmartGrid

# Procedural
S_GEN = ../Source/Procedural
B_GEN = ./Procedural

# Menu
S_MENU = ../Source/Menu
B_MENU = ./Menu

# GfxLib
S_GFX = ../Source/GfxLib
B_GFX = ./GfxLib

# Blob Engine 
S_BE = ../Source/BlobEngine
B_BE = ./BlobEngine

# EXECUTABLE
AirDoplhinApp:

	gcc -Wall $(B_SG)/SmartGrid.o -o blobEngineApp $(B_GFX)/libisentlib.a $(B_GFX)/OutilsLib.o $(B_GFX)/BmpLib.o $(B_SUI)/SmartUI.o $(B_BE)/BlobEngine.o $(B_GEN)/maze.o $(B_GEN)/rock.o $(B_MENU)/menu.o -lm -lglut -lGL -lX11

# BLOB ENGINE
$(B_BE)/BlobEngine.o: $(S_BE)/BlobEngine.c $(S_GFX)/GfxLib.h $(S_GFX)/BmpLib.h $(S_GFX)/ESLib.h $(S_SUI)/SmartUI.h $(S_BE)/BlobEngine.h
	gcc -Wall -c $(S_BE)/BlobEngine.c

# SMART GRID
$(B_SG)/SmartGrid.o: $(S_SG)/SmartGrid.c $(S_GFX)/GfxLib.h $(S_GFX)/BmpLib.h $(S_GFX)/ESLib.h $(S_SUI)/SmartUI.h $(S_BE)/BlobEngine.h $(S_GEN)/rock.h $(S_GEN)/maze.h
	gcc -Wall  -c $(S_SG)/SmartGrid.c

# SMART UI
$(B_SUI)/SmartUI.o: $(S_SUI)/SmartUI.c $(S_GFX)/GfxLib.h $(S_GFX)/BmpLib.h $(S_GFX)/ESLib.h $(S_SUI)/SmartUI.h
	gcc -Wall  -c $(S_SUI)/SmartUI.c

# MAZE
$(B_GEN)/maze.o: $(S_GEN)/maze.c $(S_GEN)/maze.h
	gcc -Wall  -c $(S_GEN)/maze.c

# ROCK
$(B_GEN)/rock.o: $(S_GEN)/rock.c $(S_BE)/BlobEngine.h $(S_GEN)/rock.h
	gcc -Wall  -c $(S_GEN)/rock.c

# MENU
$(B_MENU)/menu.o: $(S_MENU)/menu.c $(S_GFX)/GfxLib.h $(S_GFX)/BmpLib.h $(S_BE)/BlobEngine.h $(S_SUI)/SmartUI.h $(S_MENU)/menu.h
	gcc -Wall  -c $(S_MENU)/menu.c

# BMP LIB
$(B_GFX)/libisentlib.a: $(B_GFX)/BmpLib.o $(B_GFX)/ErreurLib.o $(B_GFX)/ESLib.o $(B_GFX)/GfxLib.o $(B_GFX)/OutilsLib.o $(B_GFX)/VectorLib.o
	mv BmpLib.o $(B_GFX)/
	mv ESLib.o $(B_GFX)/
	mv ErreurLib.o $(B_GFX)/
	mv GfxLib.o $(B_GFX)/
	mv OutilsLib.o $(B_GFX)/
	mv VectorLib.o $(B_GFX)/
	ar r $(B_GFX)/libisentlib.a $(B_GFX)/BmpLib.o $(B_GFX)/ErreurLib.o $(B_GFX)/ESLib.o $(B_GFX)/GfxLib.o $(B_GFX)/OutilsLib.o $(B_GFX)/VectorLib.o
	ranlib $(B_GFX)/libisentlib.a

$(B_GFX)/BmpLib.o: $(S_GFX)/BmpLib.c $(S_GFX)/BmpLib.h $(S_GFX)/OutilsLib.h
	gcc -Wall -O2 -c $(S_GFX)/BmpLib.c

$(B_GFX)/ESLib.o: $(S_GFX)/ESLib.c $(S_GFX)/ESLib.h $(S_GFX)/ErreurLib.h
	gcc -Wall -O2 -c $(S_GFX)/ESLib.c

$(B_GFX)/ErreurLib.o: $(S_GFX)/ErreurLib.c $(S_GFX)/ErreurLib.h
	gcc -Wall -O2 -c $(S_GFX)/ErreurLib.c

$(B_GFX)/GfxLib.o: $(S_GFX)/GfxLib.c $(S_GFX)/GfxLib.h $(S_GFX)/ESLib.h
	gcc -Wall -O2 -c $(S_GFX)/GfxLib.c -I/usr/include/GL

$(B_GFX)/OutilsLib.o: $(S_GFX)/OutilsLib.c $(S_GFX)/OutilsLib.h
	gcc -Wall -O2 -c $(S_GFX)/OutilsLib.c

$(B_GFX)/VectorLib.o: $(S_GFX)/VectorLib.c $(S_GFX)/VectorLib.h
	gcc -Wall -O2 -c $(S_GFX)/VectorLib.c -msse3

# CELAN (DOIT ETRE APPLIQUER SYSTEMATIQUEMENT AVANT UN MAKE)
deepclean:
	rm -f blobEngineApp
	rm -f ./BlobEngine/*
	rm -f ./GfxLib/*
	rm -f ./SmartGrid/*
	rm -f ./SmartUI/*
	rm -f ./Menu/*
	rm -f ./Procedural/*