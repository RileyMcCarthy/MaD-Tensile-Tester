EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+3V3 #PWR?
U 1 1 6096C5A3
P 5400 1500
F 0 "#PWR?" H 5400 1350 50  0001 C CNN
F 1 "+3V3" H 5415 1673 50  0000 C CNN
F 2 "" H 5400 1500 50  0001 C CNN
F 3 "" H 5400 1500 50  0001 C CNN
	1    5400 1500
	1    0    0    -1  
$EndComp
$Comp
L MAD_Board:XL1509 U?
U 1 1 60979310
P 4300 1500
F 0 "U?" H 4000 1750 50  0000 L CNN
F 1 "XL1509-3.3" H 4400 1750 50  0000 L CNN
F 2 "" H 4150 1500 50  0001 C CNN
F 3 "" H 4150 1500 50  0001 C CNN
	1    4300 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60979316
P 3650 1750
F 0 "C?" H 3742 1796 50  0000 L CNN
F 1 "470uF" H 3742 1705 50  0000 L CNN
F 2 "" H 3650 1750 50  0001 C CNN
F 3 "~" H 3650 1750 50  0001 C CNN
	1    3650 1750
	1    0    0    -1  
$EndComp
Text GLabel 3450 1500 0    50   Input ~ 0
VIN
Wire Wire Line
	3450 1500 3650 1500
Wire Wire Line
	3650 1650 3650 1500
Connection ~ 3650 1500
Wire Wire Line
	3650 1500 3900 1500
$Comp
L power:GND #PWR?
U 1 1 60979321
P 4300 1850
F 0 "#PWR?" H 4300 1600 50  0001 C CNN
F 1 "GND" H 4305 1677 50  0000 C CNN
F 2 "" H 4300 1850 50  0001 C CNN
F 3 "" H 4300 1850 50  0001 C CNN
	1    4300 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60979327
P 3650 1850
F 0 "#PWR?" H 3650 1600 50  0001 C CNN
F 1 "GND" H 3655 1677 50  0000 C CNN
F 2 "" H 3650 1850 50  0001 C CNN
F 3 "" H 3650 1850 50  0001 C CNN
	1    3650 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1850 4200 1850
Connection ~ 4200 1850
Wire Wire Line
	4200 1850 4300 1850
Connection ~ 4300 1850
Wire Wire Line
	4300 1850 4400 1850
Connection ~ 4400 1850
Wire Wire Line
	4400 1850 4500 1850
$Comp
L Device:D_Schottky_Small D?
U 1 1 60979334
P 4800 1750
F 0 "D?" V 4754 1820 50  0000 L CNN
F 1 "SS36" V 4845 1820 50  0000 L CNN
F 2 "" V 4800 1750 50  0001 C CNN
F 3 "~" V 4800 1750 50  0001 C CNN
	1    4800 1750
	0    1    1    0   
$EndComp
$Comp
L Device:L_Small L?
U 1 1 6097933A
P 5000 1500
F 0 "L?" V 5185 1500 50  0000 C CNN
F 1 "47uH/3A" V 5094 1500 50  0000 C CNN
F 2 "" H 5000 1500 50  0001 C CNN
F 3 "~" H 5000 1500 50  0001 C CNN
	1    5000 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 1500 4800 1500
Connection ~ 4800 1500
Wire Wire Line
	4800 1500 4900 1500
$Comp
L power:GND #PWR?
U 1 1 60979343
P 4800 1850
F 0 "#PWR?" H 4800 1600 50  0001 C CNN
F 1 "GND" H 4805 1677 50  0000 C CNN
F 2 "" H 4800 1850 50  0001 C CNN
F 3 "" H 4800 1850 50  0001 C CNN
	1    4800 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1500 4800 1650
$Comp
L Device:C_Small C?
U 1 1 6097934A
P 5200 1750
F 0 "C?" H 5292 1796 50  0000 L CNN
F 1 "330uF" H 5292 1705 50  0000 L CNN
F 2 "" H 5200 1750 50  0001 C CNN
F 3 "~" H 5200 1750 50  0001 C CNN
	1    5200 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 1500 5100 1500
$Comp
L power:GND #PWR?
U 1 1 60979351
P 5200 1850
F 0 "#PWR?" H 5200 1600 50  0001 C CNN
F 1 "GND" H 5205 1677 50  0000 C CNN
F 2 "" H 5200 1850 50  0001 C CNN
F 3 "" H 5200 1850 50  0001 C CNN
	1    5200 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 1650 5200 1500
Wire Wire Line
	4300 1200 5200 1200
Wire Wire Line
	5200 1200 5200 1500
Connection ~ 5200 1500
Wire Wire Line
	5200 1500 5400 1500
Wire Wire Line
	2650 1550 2850 1550
$Comp
L power:+5V #PWR?
U 1 1 6096D224
P 2850 1550
F 0 "#PWR?" H 2850 1400 50  0001 C CNN
F 1 "+5V" H 2865 1723 50  0000 C CNN
F 2 "" H 2850 1550 50  0001 C CNN
F 3 "" H 2850 1550 50  0001 C CNN
	1    2850 1550
	1    0    0    -1  
$EndComp
Connection ~ 2650 1550
Wire Wire Line
	2650 1250 2650 1550
Wire Wire Line
	1750 1250 2650 1250
Wire Wire Line
	2650 1700 2650 1550
$Comp
L power:GND #PWR?
U 1 1 60968569
P 2650 1900
F 0 "#PWR?" H 2650 1650 50  0001 C CNN
F 1 "GND" H 2655 1727 50  0000 C CNN
F 2 "" H 2650 1900 50  0001 C CNN
F 3 "" H 2650 1900 50  0001 C CNN
	1    2650 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1550 2550 1550
$Comp
L Device:C_Small C?
U 1 1 60966D9E
P 2650 1800
F 0 "C?" H 2742 1846 50  0000 L CNN
F 1 "180uF" H 2742 1755 50  0000 L CNN
F 2 "" H 2650 1800 50  0001 C CNN
F 3 "~" H 2650 1800 50  0001 C CNN
	1    2650 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1550 2250 1700
$Comp
L power:GND #PWR?
U 1 1 6096645C
P 2250 1900
F 0 "#PWR?" H 2250 1650 50  0001 C CNN
F 1 "GND" H 2255 1727 50  0000 C CNN
F 2 "" H 2250 1900 50  0001 C CNN
F 3 "" H 2250 1900 50  0001 C CNN
	1    2250 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1550 2350 1550
Connection ~ 2250 1550
Wire Wire Line
	2150 1550 2250 1550
$Comp
L Device:L_Small L?
U 1 1 609644FE
P 2450 1550
F 0 "L?" V 2635 1550 50  0000 C CNN
F 1 "68uH/3A" V 2544 1550 50  0000 C CNN
F 2 "" H 2450 1550 50  0001 C CNN
F 3 "~" H 2450 1550 50  0001 C CNN
	1    2450 1550
	0    -1   -1   0   
$EndComp
$Comp
L Device:D_Schottky_Small D?
U 1 1 60962B10
P 2250 1800
F 0 "D?" V 2204 1870 50  0000 L CNN
F 1 "SS36" V 2295 1870 50  0000 L CNN
F 2 "" V 2250 1800 50  0001 C CNN
F 3 "~" V 2250 1800 50  0001 C CNN
	1    2250 1800
	0    1    1    0   
$EndComp
Connection ~ 1750 1900
$Comp
L power:GND #PWR?
U 1 1 609612C5
P 1100 1900
F 0 "#PWR?" H 1100 1650 50  0001 C CNN
F 1 "GND" H 1105 1727 50  0000 C CNN
F 2 "" H 1100 1900 50  0001 C CNN
F 3 "" H 1100 1900 50  0001 C CNN
	1    1100 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 609604AA
P 1750 1900
F 0 "#PWR?" H 1750 1650 50  0001 C CNN
F 1 "GND" H 1755 1727 50  0000 C CNN
F 2 "" H 1750 1900 50  0001 C CNN
F 3 "" H 1750 1900 50  0001 C CNN
	1    1750 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1550 1350 1550
Connection ~ 1100 1550
Wire Wire Line
	1100 1700 1100 1550
Wire Wire Line
	900  1550 1100 1550
Text GLabel 900  1550 0    50   Input ~ 0
VIN
$Comp
L Device:C_Small C?
U 1 1 609431B2
P 1100 1800
F 0 "C?" H 1192 1846 50  0000 L CNN
F 1 "470uF" H 1192 1755 50  0000 L CNN
F 2 "" H 1100 1800 50  0001 C CNN
F 3 "~" H 1100 1800 50  0001 C CNN
	1    1100 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 1900 1750 1900
Wire Wire Line
	1750 1900 1950 1900
$EndSCHEMATC
