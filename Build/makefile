# SmartList
moduleList = AirManager bddManager Control Renderer SmarLit
moduleListC = $(moduleList:=/*.c)
moduleListPathC = $(addprefix ../Sources/,$(moduleListC))
moduleListH = $(moduleList:=/*.h)
moduleListPathH = $(addprefix ../Sources/,$(moduleListH))

subModuleList = AirManager cmdprint bddManager Control Renderer SmarLit

# EXECUTABLE
AirDoplhinApp:
	echo ../Sources/$(moduleListPathC)
	echo ../Sources/$(moduleListPathH)
	# gcc -Wall $(B_SG)/SmartGrid.o -o blobEngineApp $(B_GFX)/libisentlib.a $(B_GFX)/OutilsLib.o $(B_GFX)/BmpLib.o $(B_SUI)/SmartUI.o $(B_BE)/BlobEngine.o $(B_GEN)/maze.o $(B_GEN)/rock.o $(B_MENU)/menu.o -lm -lglut -lGL -lX11