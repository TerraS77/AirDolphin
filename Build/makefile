# Module List & Files
moduleList = AirManager bddManager Control Renderer SmartList
moduleListC = $(moduleList:=/*.c)
moduleListPathC = $(addprefix ../Source/,$(moduleListC))
moduleListH = $(moduleList:=/*.h)
moduleListPathH = $(addprefix ../Source/,$(moduleListH))

# files to compile
subModuleList = AirManager cmdprint bddManager Control Renderer SmartList
subListC = $(subModuleList:=.c)

# config
flags = -lm $(shell pkg-config --cflags --libs sdl2)

# EXECUTABLE
AirDoplhinApp:
	cp $(moduleListPathC) $(moduleListPathH) ./
	gcc -Wall $(subListC) -o $a $(flags)
	rm *.c
	rm *.h

clear:
	rm *.c
	rm *.h