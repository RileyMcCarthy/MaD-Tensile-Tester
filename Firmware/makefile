#FlexProp
PORT = /dev/ttyUSB0
SPIN2CPP = C:\spin2cpp\spin2cpp.exe
FLEXCC = ~/flexprop/bin/flexcc
LOAD = ~/flexprop/bin/loadp2
FLEXCCFLAGS=-2 -O1 -Wall -D_DEBUG_INFO -D_DEBUG_WARNING #-D_DEBUG_ERROR#-D_DEBUG #-g#D_DEBUG_FATFS=1#-DFF_USE_LFN
BIN = bin

CATALINA="C:/Program Files (x86)/Catalina_5.1/bin/catalina.exe"
CATALINAFLAGS= -O1 -p2 
CAT = cat

#GCC
GCC = g++
GCCFLAGS = -Wall -DSIMULATION -include MaD_Firmware/Librarys/simulation/propellerSim.h
WIN = win

FIND := find #C:/cygwin64/bin/find.exe

TARGET = MAD.binary
SRC = MaD_Firmware #flexprop/include # Root folders for src code
INCLUDE = $(sort $(dir $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.h" -o -name "*.spin2"))))
SOURCE = $(sort $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.c")))
SPIN = $(sort $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.spin2")))

$(info $$INCLUDE is [${INCLUDE}])
$(info $$SOURCE is [${SOURCE}])
$(info $$SPIN is [${SPIN}])

OBJECTS := $(addprefix $(BIN)/,$(SOURCE:.c=.o))
OBJECTSCAT := $(addprefix $(CAT)/,$(SOURCE:.c=.o))
OBJECTSWIN := $(addprefix $(WIN)/,$(SOURCE:.c=.o))
OBJECTSPIN := $(addprefix $(WIN)/,$(SPIN:.spin2=.h))
$(info $$OBJECTSPIN is [${OBJECTSPIN}])
INC := $(addprefix -I ,$(INCLUDE))

gui:
	cd MaD_Software && export FLASK_APP=app && export FLASK_ENV=development && flask run -h 0.0.0.0
guideps: 
	pipreqs MaD_Software --force
	pip install -r MaD_Software/requirements.txt
flexpropc: clean clear flexprop
flexprop: $(BIN)/$(TARGET) exportStructures run
sim: clean clear $(WIN)/$(TARGET)
catalina: $(CAT)/$(TARGET)

exportStructures:
	ctypesgen MaD_Firmware/Librarys/JSON/JSON.h -o MaD_Software/JSON.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/JSON.py
	sed -i -E "s/from ./from /g" MaD_Software/JSON.py
	#sed -i '1,31d' MaD_Software/JSON.py
	#sed -i '1s/^/from ctypes import *\n/' MaD_Software/JSON.py
	ctypesgen MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/MonitorDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/MonitorDefinition.py
	sed -i -E "s/from ./from /g" MaD_Software/MonitorDefinition.py
	#sed -i '1,31d' MaD_Software/MonitorDefinition.py
	#sed -i '1s/^/from ctypes import *\n/' MaD_Software/MonitorDefinition.py
	ctypesgen MaD_Firmware/include/Utility/StateMachineDefinition.h -o MaD_Software/StateMachineDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/StateMachineDefinition.py
	sed -i -E "s/from ./from /g" MaD_Software/StateMachineDefinition.py
	#sed -i '1,31d' MaD_Software/StateMachineDefinition.py
	#sed -i '1s/^/from ctypes import *\n/' MaD_Software/StateMachineDefinition.py
exportStructuresNew:
	clang2py MaD_Firmware/Librarys/JSON/JSON.h -o MaD_Software/JSON.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/JSON.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/JSON.py
	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/JSON.py 
	clang2py MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/MonitorDefinition.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/MonitorDefinition.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/MonitorDefinition.py 
#	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/MonitorDefinition.py
	clang2py MaD_Firmware/include/Utility/StateMachineDefinition.h -o MaD_Software/StateMachineDefinition.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/StateMachineDefinition.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/StateMachineDefinition.py 
#	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/StateMachineDefinition.py
# Make for FlexCC
$(BIN)/$(TARGET): $(OBJECTS)
	$(FLEXCC) $(FLEXCCFLAGS) $(INC) $(OBJECTS) -o $@

$(BIN)/%.o: %.c
	-mkdir -p $(subst \,/,$(@D))
	$(FLEXCC) $(FLEXCCFLAGS) $(INC) -c -o $@ $<

# Make for Catalina
$(CAT)/$(TARGET): $(OBJECTSCAT)
	$(CATALINA) $(CATALINAFLAGS) $(INC) $(OBJECTS) -o $@

$(CAT)/%.o: %.c
	-mkdir -p $(subst /,/,$(@D))
	$(CATALINA) $(CATALINAFLAGS) $(INC) -c -o $@ $<

#Make for windows
$(WIN)/$(TARGET): $(OBJECTSWIN)
	$(GCC) $(GCCFLAGS) $(INC) $(OBJECTSWIN) -o $@

#$(WIN)/%.h: %.spin2
#	-mkdir -p $(subst /,\,$(@D))
#	$(info $(SPIN2CPP) --p2 -o $@ $<)
#	$(SPIN2CPP) --p2 -o $@ $<

$(WIN)/%.o: %.c#$(WIN)/%.c
	$(info .o from win/.c is [$D $@ $<])
	-mkdir -p $(subst /,\,$(@D))
	$(GCC) $(GCCFLAGS) $(INC) -c $<

$(WIN)/%.c: %.c
	-mkdir -p $(subst /,\,$(@D))
	$(info $$SPIN is [$@ $<])
	-./a.exe $(subst /,\,$(<)) $(subst /,\,$(@))
	
run: 
	$(LOAD) -p $(PORT) -t -b230400 -ZERO -v $(subst \,/,bin\$(TARGET))
.PHONY: clean
.PHONY: clear
clear:
	clear
clean:
	$(RM) $(subst /,\,$(BIN)) -R
	-mkdir -p $(BIN)
	$(RM) $(subst /,\,$(WIN)) -R
	-mkdir -p $(WIN)