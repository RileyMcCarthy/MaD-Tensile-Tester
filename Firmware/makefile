#FlexProp
SPIN2CPP = C:\spin2cpp\spin2cpp.exe
FLEXCC = ~/flexprop/bin/flexcc
LOAD = ~/flexprop/bin/loadp2
FLEXCCFLAGS=-2 -O2 -Wall #-D_DEBUG #-g#D_DEBUG_FATFS=1#-DFF_USE_LFN
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

flexpropc: clean clear $(BIN)/$(TARGET) run
flexprop: $(BIN)/$(TARGET) run
sim: clean clear $(WIN)/$(TARGET)
catalina: $(CAT)/$(TARGET)

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
	$(LOAD) -b230400 -t -k -ZERO -v $(subst \,/,bin\$(TARGET))
.PHONY: clean
.PHONY: clear
clear:
	clear
clean:
	$(RM) $(subst /,\,$(BIN)) -R
	-mkdir -p $(BIN)
	$(RM) $(subst /,\,$(WIN)) -R
	-mkdir -p $(WIN)