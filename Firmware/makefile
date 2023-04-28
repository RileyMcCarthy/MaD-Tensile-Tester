#FlexProp
PORT = /dev/ttyUSB0
SPIN2CPP = C:\spin2cpp\spin2cpp.exe
FLEXCC = ~/flexprop/bin/flexcc
LOAD = ~/flexprop/bin/loadp2
FLEXCCFLAGS=-2 -O1 -Wall #-D_DEBUG_WARNING -D_DEBUG_ERROR -D_DEBUG_INFO#-D_DEBUG #-g#D_DEBUG_FATFS=1#-DFF_USE_LFN
BIN = bin
FIND := find #C:/cygwin64/bin/find.exe

TARGET = MAD.binary
SRC = MaD_Firmware # Root folders for src code
INCLUDE = $(sort $(dir $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.h" -o -name "*.spin2")))) MaD_Firmware/include MaD_Firmware
SOURCE = $(sort $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.c")))
SPIN = $(sort $(foreach dir,$(SRC),$(shell $(FIND) $(dir) -name "*.spin2")))

OBJECTS := $(addprefix $(BIN)/,$(SOURCE:.c=.o))
INC := $(addprefix -I ,$(INCLUDE))

flexpropc: clean clear flexprop
flexprop: $(BIN)/$(TARGET) run

# Make for FlexCC
$(BIN)/$(TARGET): $(OBJECTS)
	$(FLEXCC) $(FLEXCCFLAGS) $(INC) $(OBJECTS) -o $@

$(BIN)/%.o: %.c
	-mkdir -p $(subst \,/,$(@D))
	$(FLEXCC) $(FLEXCCFLAGS) $(INC) -c -o $@ $<
	
run: 
	$(LOAD)  -b230400 $(subst \,/,bin\$(TARGET)) -t
.PHONY: clean
.PHONY: clear
clear:
	clear
clean:
	sudo $(RM) $(subst /,/,$(BIN)) -Rf
	-mkdir -p $(BIN)
