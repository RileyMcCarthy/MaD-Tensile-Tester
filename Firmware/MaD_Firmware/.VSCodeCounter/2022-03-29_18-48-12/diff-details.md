# Diff Details

Date : 2022-03-29 18:48:12

Directory c:\Users\riley\Documents\GitHub\Tensile-Testing-Machine\Firmware\MaD_Firmware\Librarys

Total : 60 files,  6626 codes, 1614 comments, 1142 blanks, all 9382 lines

[summary](results.md) / [details](details.md) / [diff summary](diff.md) / diff details

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [Librarys/Communication/list.h](/Librarys/Communication/list.h) | C++ | 11 | 0 | 2 | 13 |
| [Librarys/Communication/queue.h](/Librarys/Communication/queue.h) | C++ | 7 | 0 | 1 | 8 |
| [Librarys/DS3231/DS3231.c](/Librarys/DS3231/DS3231.c) | C | 111 | 48 | 17 | 176 |
| [Librarys/DS3231/DS3231.h](/Librarys/DS3231/DS3231.h) | C++ | 31 | 4 | 7 | 42 |
| [Librarys/DYN4/DYN4.c](/Librarys/DYN4/DYN4.c) | C | 111 | 39 | 16 | 166 |
| [Librarys/DYN4/DYN4.h](/Librarys/DYN4/DYN4.h) | C++ | 80 | 16 | 16 | 112 |
| [Librarys/ESP32/esp32_core.spin2](/Librarys/ESP32/esp32_core.spin2) | spin2 | 224 | 117 | 35 | 376 |
| [Librarys/ESP32/esp32_tcpip.spin2](/Librarys/ESP32/esp32_tcpip.spin2) | spin2 | 277 | 140 | 38 | 455 |
| [Librarys/ESP32/esp32_wifi.spin2](/Librarys/ESP32/esp32_wifi.spin2) | spin2 | 183 | 82 | 28 | 293 |
| [Librarys/Encoder/jm_quadrature.spin2](/Librarys/Encoder/jm_quadrature.spin2) | spin2 | 55 | 57 | 43 | 155 |
| [Librarys/ForceGauge/ForceGauge.c](/Librarys/ForceGauge/ForceGauge.c) | C | 88 | 18 | 16 | 122 |
| [Librarys/ForceGauge/ForceGauge.h](/Librarys/ForceGauge/ForceGauge.h) | C++ | 23 | 0 | 10 | 33 |
| [Librarys/I2C/jm_i2c.spin2](/Librarys/I2C/jm_i2c.spin2) | spin2 | 180 | 59 | 78 | 317 |
| [Librarys/JSON/JSON.c](/Librarys/JSON/JSON.c) | C | 987 | 49 | 141 | 1,177 |
| [Librarys/JSON/JSON.h](/Librarys/JSON/JSON.h) | C++ | 122 | 2 | 19 | 143 |
| [Librarys/MCP23017/MCP23017.c](/Librarys/MCP23017/MCP23017.c) | C | 132 | 10 | 13 | 155 |
| [Librarys/MCP23017/MCP23017.h](/Librarys/MCP23017/MCP23017.h) | C++ | 31 | 7 | 12 | 50 |
| [Librarys/MotionPlanningOld/MotionPlanningold.h](/Librarys/MotionPlanningOld/MotionPlanningold.h) | C++ | 10 | 0 | 2 | 12 |
| [Librarys/MotionPlanningOld/bbpr.c](/Librarys/MotionPlanningOld/bbpr.c) | C | 253 | 20 | 24 | 297 |
| [Librarys/MotionPlanningOld/bbpr.h](/Librarys/MotionPlanningOld/bbpr.h) | C++ | 11 | 46 | 6 | 63 |
| [Librarys/MotionPlanningOld/machineProfile.m](/Librarys/MotionPlanningOld/machineProfile.m) | Objective-C | 125 | 0 | 21 | 146 |
| [Librarys/MotionPlanningOld/motionPlanning.c](/Librarys/MotionPlanningOld/motionPlanning.c) | C | 143 | 12 | 18 | 173 |
| [Librarys/MotionPlanningOld/motionPlanning.h](/Librarys/MotionPlanningOld/motionPlanning.h) | C++ | 4 | 0 | 2 | 6 |
| [Librarys/MotionPlanning/MotionPlanning.c](/Librarys/MotionPlanning/MotionPlanning.c) | C | 178 | 4 | 23 | 205 |
| [Librarys/MotionPlanning/MotionPlanning.h](/Librarys/MotionPlanning/MotionPlanning.h) | C++ | 49 | 0 | 7 | 56 |
| [Librarys/NavKey/i2cNavKey.c](/Librarys/NavKey/i2cNavKey.c) | C | 442 | 141 | 74 | 657 |
| [Librarys/NavKey/i2cNavKey.h](/Librarys/NavKey/i2cNavKey.h) | C++ | 104 | 27 | 32 | 163 |
| [Librarys/RA8876/Button.c](/Librarys/RA8876/Button.c) | C | 58 | 1 | 5 | 64 |
| [Librarys/RA8876/Button.h](/Librarys/RA8876/Button.h) | C++ | 19 | 0 | 2 | 21 |
| [Librarys/RA8876/Module.c](/Librarys/RA8876/Module.c) | C | 308 | 2 | 34 | 344 |
| [Librarys/RA8876/Module.h](/Librarys/RA8876/Module.h) | C++ | 58 | 0 | 13 | 71 |
| [Librarys/RA8876/RA8876.c](/Librarys/RA8876/RA8876.c) | C | 1,205 | 270 | 78 | 1,553 |
| [Librarys/RA8876/RA8876.h](/Librarys/RA8876/RA8876.h) | C++ | 812 | 33 | 95 | 940 |
| [Librarys/Serial/jm_fullduplexserial.spin2](/Librarys/Serial/jm_fullduplexserial.spin2) | spin2 | 298 | 176 | 238 | 712 |
| [Librarys/Serial/jm_nstr.spin2](/Librarys/Serial/jm_nstr.spin2) | spin2 | 122 | 59 | 59 | 240 |
| [Librarys/tiny-json/tiny-json.c](/Librarys/tiny-json/tiny-json.c) | C | 391 | 137 | 34 | 562 |
| [Librarys/tiny-json/tiny-json.h](/Librarys/tiny-json/tiny-json.h) | C | 83 | 85 | 25 | 193 |
| [include/Elements/Clock.h](/include/Elements/Clock.h) | C++ | -25 | -4 | -5 | -34 |
| [include/Elements/Explorer.h](/include/Elements/Explorer.h) | C++ | -29 | 0 | -6 | -35 |
| [include/Elements/FunctionWindow.h](/include/Elements/FunctionWindow.h) | C++ | -18 | 0 | -3 | -21 |
| [include/Elements/Graph.h](/include/Elements/Graph.h) | C++ | -25 | 0 | -3 | -28 |
| [include/Elements/Keyboard.h](/include/Elements/Keyboard.h) | C++ | -18 | -4 | -5 | -27 |
| [include/Elements/MotionStateWindow.h](/include/Elements/MotionStateWindow.h) | C++ | -18 | 0 | -3 | -21 |
| [include/Main/MaD.h](/include/Main/MaD.h) | C++ | -45 | -4 | -7 | -56 |
| [include/Pages/AutomaticPage.h](/include/Pages/AutomaticPage.h) | C++ | -28 | -4 | -4 | -36 |
| [include/Pages/CalibrateForcePage.h](/include/Pages/CalibrateForcePage.h) | C++ | -27 | 0 | -4 | -31 |
| [include/Pages/ManualPage.h](/include/Pages/ManualPage.h) | C++ | -24 | -4 | -4 | -32 |
| [include/Pages/NavigationPage.h](/include/Pages/NavigationPage.h) | C++ | -29 | -4 | -8 | -41 |
| [include/Pages/SettingsPage.h](/include/Pages/SettingsPage.h) | C++ | -23 | 0 | -5 | -28 |
| [include/Pages/StatusPage.h](/include/Pages/StatusPage.h) | C++ | -26 | -4 | -6 | -36 |
| [include/Pages/TestProfile.h](/include/Pages/TestProfile.h) | C++ | -28 | 0 | -3 | -31 |
| [include/Utility/Control.h](/include/Utility/Control.h) | C++ | -30 | 0 | -5 | -35 |
| [include/Utility/Error.h](/include/Utility/Error.h) | C++ | -30 | -8 | -8 | -46 |
| [include/Utility/IOBoard.h](/include/Utility/IOBoard.h) | C++ | -80 | -1 | -22 | -103 |
| [include/Utility/Images.h](/include/Utility/Images.h) | C++ | -22 | -4 | -5 | -31 |
| [include/Utility/LoadingOverlay.h](/include/Utility/LoadingOverlay.h) | C++ | -10 | 0 | -2 | -12 |
| [include/Utility/Monitor.h](/include/Utility/Monitor.h) | C++ | -30 | 0 | -5 | -35 |
| [include/Utility/Selection.h](/include/Utility/Selection.h) | C++ | -7 | 0 | -2 | -9 |
| [include/Utility/StateMachine.h](/include/Utility/StateMachine.h) | C++ | -106 | -5 | -15 | -126 |
| [include/Utility/Style.h](/include/Utility/Style.h) | C++ | -22 | -1 | -12 | -35 |

[summary](results.md) / [details](details.md) / [diff summary](diff.md) / diff details