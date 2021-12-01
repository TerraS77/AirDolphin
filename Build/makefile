# Module List & Files
moduleList = AirManager Control SmartList renderer
moduleListC = $(moduleList:=/*.c)
moduleListPathC = $(addprefix ../Source/,$(moduleListC))
moduleListH = $(moduleList:=/*.h)
moduleListPathH = $(addprefix ../Source/,$(moduleListH))

# files to compile
subModuleList = AirSim AirManager Control SmartList cmdPrint renderer
subListC = $(subModuleList:=.c)

# config
# flags = -lm
flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf

# EXECUTABLE
AirDolphinApp:
	cp $(moduleListPathC) $(moduleListPathH) ./
	gcc -Wall $(subListC) -o AirDolphinApp $(flags)
	rm *.c
	rm *.h

clear:
	rm AirDolphinApp
	rm *.c
	rm *.h