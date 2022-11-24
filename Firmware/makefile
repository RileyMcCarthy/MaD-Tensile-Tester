#FlexProp
PORT = /dev/ttyUSB1
SPIN2CPP = C:\spin2cpp\spin2cpp.exe
FLEXCC = ~/flexprop/bin/flexcc
LOAD = ~/flexprop/bin/loadp2
FLEXCCFLAGS=-2 -O1 -Wall -D_DEBUG_INFO -D_DEBUG_WARNING -D_DEBUG_ERROR#-D_DEBUG #-g#D_DEBUG_FATFS=1#-DFF_USE_LFN
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

OBJECTS := $(addprefix $(BIN)/,$(SOURCE:.c=.o))
OBJECTSCAT := $(addprefix $(CAT)/,$(SOURCE:.c=.o))
OBJECTSWIN := $(addprefix $(WIN)/,$(SOURCE:.c=.o))WWWD
OBJECTSPIN := $(addprefix $(WIN)/,$(SPIN:.spin2=.h))
INC := $(addprefix -I ,$(INCLUDE))
gui:
	sudo killall pigpiod || true
	sudo pigpiod
	cd MaD_Software && sudo ./venv/bin/python3 main.py
guideps:
	rm -Rf MaD_Software/venv
	python3 -m venv MaD_Software/venv && \
	. MaD_Software/venv/bin/activate && \
	pip install -r MaD_Software/requirements.txt
flexpropc: clean clear exportStructures flexprop
flexprop: $(BIN)/$(TARGET) run
sim: clean clear $(WIN)/$(TARGET)
catalina: $(CAT)/$(TARGET)

exportStructures:
	ctypesgen MaD_Firmware/Librarys/JSON/JSON.h -o MaD_Software/definitions/JSON.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/JSON.py

	ctypesgen MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/definitions/MonitorDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/MonitorDefinition.py

	ctypesgen MaD_Firmware/include/Utility/StateMachineDefinition.h -o MaD_Software/definitions/StateMachineDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/StateMachineDefinition.py

	ctypesgen MaD_Firmware/include/Main/Communication/CommunicationDefinition.h -o MaD_Software/definitions/CommuncationDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/CommuncationDefinition.py

	ctypesgen MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h -o MaD_Software/definitions/MotionPlanningDefinition.py --no-embed-preamble
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/MotionPlanningDefinition.py

exportStructuresNew:
	clang2py MaD_Firmware/Librarys/JSON/JSON.h -o MaD_Software/definitions/JSON.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/JSON.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/definitions/JSON.py
	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/definitions/JSON.py 
	clang2py MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/definitions/MonitorDefinition.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/MonitorDefinition.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/definitions/MonitorDefinition.py 
#	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/definitions/MonitorDefinition.py
	clang2py MaD_Firmware/include/Utility/StateMachineDefinition.h -o MaD_Software/definitions/StateMachineDefinition.py
	sed -i -E "s/c_double/c_float/g" MaD_Software/definitions/StateMachineDefinition.py
	sed -i -E "s/c_ubyte/c_char/g" MaD_Software/definitions/StateMachineDefinition.py 
#	sed -i -E "s/.*PADDING_[0-9].*//g" MaD_Software/definitions/StateMachineDefinition.py
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
	$(LOAD) -p $(PORT) -b230400 -ZERO -v $(subst \,/,bin\$(TARGET)) -t
.PHONY: clean
.PHONY: clear
clear:
	clear
clean:
	$(RM) $(subst /,\,$(BIN)) -R
	-mkdir -p $(BIN)
	$(RM) $(subst /,\,$(WIN)) -R
	-mkdir -p $(WIN)