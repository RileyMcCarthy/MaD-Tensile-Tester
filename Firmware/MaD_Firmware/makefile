 CC = C:\flexprop\bin\flexspin.exe
 LOAD = C:\flexprop\bin\loadp2.exe
 
 CFLAGS=-2 -O1 -Wall -g -D_DEBUG_FATFS=1#-DFF_USE_LFN

 TARGET = MAD.binary
 BIN = bin
 INCLUDE = Librarys/SDCard include/Utility include/Elements include/Pages include/Main Librarys/json-maker Librarys/tiny-json Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/I2C Librarys/Encoder Librarys/JSON Librarys/Serial Librarys/MotionPlanning Librarys/leak_detector_c Librarys/SimpleI2CSlow 
 SOURCEDIR = Librarys/SDCard Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 Librarys/json-maker  Librarys/tiny-json Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/Encoder Librarys/JSON Librarys/Serial Librarys/I2C src/Elements src/Main src/Pages src/Utility Librarys/MotionPlanning Librarys/leak_detector_c Librarys/SimpleI2CSlow 
 BIN_DIR := $(addprefix $(BIN)/, $(INCLUDE))
 SOURCE = $(foreach dir,$(SOURCEDIR),$(wildcard $(dir)/*.c))
 SOURCE_CPP = $(foreach dir,$(SOURCEDIR),$(wildcard $(dir)/*.cpp))

 OBJECTS := $(addprefix $(BIN)/,$(SOURCE:.c=.o)) $(addprefix $(BIN)/,$(SOURCE_CPP:.cpp=.o))
 INC := $(addprefix -I ,$(INCLUDE))
 $(info VAR="$(OBJECTS)")
 $(info VAR="$(INC)")
 $(info VAR="$(SOURCE)")

 all: clean clear $(BIN)/$(TARGET) run


$(BIN)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) $(OBJECTS) -o $@

$(BIN)/%.o: %.c
	-mkdir -p $(subst /,\,$(@D))
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BIN)/%.o: %.cpp
	-mkdir -p $(subst /,\,$(@D))
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

run: 
	$(LOAD) -b230400 -t -k -ZERO -v bin\$(TARGET)

.PHONY: clean
.PHONY: clear

clear:
	clear

clean:
	$(RM) $(subst /,\,$(BIN)) -R
	-mkdir -p $(BIN)
