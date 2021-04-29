cmake_minimum_required(VERSION 3.12)
set(PROPWARE_PATH C:/PropWare/share/PropWare)
find_package(PropWare REQUIRED)
project(MaD_Board C CXX SPIN2DAT)
set(BOARD activityboard)
set(MODEL cmm)
set(COMMON_FLAGS)
INCLUDE_DIRECTORIES(include include/Elements include/Pages Librarys/NavKey Librarys/RA8876 Librarys/IOBoard Librarys/MCP23017 
Librarys/SPI Librarys/DS3231 Librarys/ForceGauge Librarys/DYN4 Librarys/Encoder Librarys/JSON Librarys/Serial)


create_simple_executable(${PROJECT_NAME}
    Librarys/Encoder/Encoder.spin
    Librarys/Encoder/EncoderAPI.c
    Librarys/JSON/JSON.c
    Librarys/JSON/JSONHelper.c
    Librarys/DYN4/DYN4.c
    src/main.cpp
    src/MaD.cpp
    src/Motion.cpp
    Librarys/Serial/BufferlessSerial.c
    src/Elements/Keyboard.cpp
    src/Elements/Clock.cpp
    src/Pages/StatusPage.cpp
    src/Pages/NavigationPage.cpp
    src/Pages/ManualPage.cpp
    src/Pages/AutomaticPage.cpp
    Librarys/NavKey/i2cNavKey.cpp
    Librarys/RA8876/RA8876_Lite.cpp
    Librarys/MCP23017/MCP23017.cpp
    Librarys/SPI/SPI_Fast.c
    Librarys/DS3231/DS3231.cpp
    Librarys/ForceGauge/ForceGauge.cpp) 