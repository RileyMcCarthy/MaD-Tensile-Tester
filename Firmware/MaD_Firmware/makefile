 #CC = C:\flexprop\bin\flexcc.exe
 CC = g++
 LOAD = C:\flexprop\bin\loadp2.exe
 SIM = C:\flexprop\bin\spinsim.exe 
 SIMFLAGS = -t2 -b115200 -B serial.txt -v0
 #CFLAGS=-2 -O1 -Wall #-D_DEBUG -g#D_DEBUG_FATFS=1#-DFF_USE_LFN
 CFLAGS = -Wall -include include/Main/propellerSim.h

#-I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Main" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Elements" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Pages" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/DYN4" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/Encoder" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/ESP32" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/ForceGauge" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/I2C" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/JSON" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MCP23017" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/NavKey" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/RA8876" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/Serial" -I "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/tiny-json"  "C:/Users/riley.m/Tensile-Testing-Machine/Firmware/MaD_Firmware/src/Main/main.c"

 TARGET = MAD.binary
 BIN = bin
 INCLUDE = C:\flexprop\include Librarys/SDCard include/Utility include/Elements include/Pages include/Main Librarys/json-maker Librarys/tiny-json Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/I2C Librarys/Encoder Librarys/JSON Librarys/Serial Librarys/MotionPlanning Librarys/leak_detector_c Librarys/SimpleI2CSlow 
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
simulate:
	$(SIM) $(SIMFLAGS) bin\$(TARGET)

.PHONY: clean
.PHONY: clear

clear:
	clear

clean:
	$(RM) $(subst /,\,$(BIN)) -R
	-mkdir -p $(BIN)
