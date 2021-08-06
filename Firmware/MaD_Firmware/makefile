 CC = flexcc
 CFLAGS=-2 -O2 -Wall 

 TARGET = MAD.binary
 BIN = bin
 INCLUDE = include/Utility include/Elements include/Pages include/Main Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/Encoder Librarys/JSON Librarys/Serial
 SOURCEDIR = Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/Encoder Librarys/JSON Librarys/Serial src/Elements src/Main
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
	-mkdir $(subst /,\,$(@D))
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BIN)/%.o: %.cpp
	-mkdir $(subst /,\,$(@D))
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

run: 
	loadp2 -p COM5 -b230400 -t -k -v bin\$(TARGET)

.PHONY: clean
.PHONY: clear

clear:
	cls

clean:
	-mkdir $(BIN)
	del /s /q $(subst /,\,$(BIN))\*
