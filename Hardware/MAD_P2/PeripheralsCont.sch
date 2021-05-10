EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
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
L MAD_P2-rescue:MAX5134-7 U?
U 1 1 60C13D6D
P 1900 1900
F 0 "U?" H 1900 2350 50  0000 C CNN
F 1 "MAX5134-7" H 1850 1450 50  0000 C CNN
F 2 "" H 1950 1900 50  0001 C CNN
F 3 "" H 1950 1900 50  0001 C CNN
	1    1900 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1550 1500 1350
Wire Wire Line
	1500 1350 1900 1350
Wire Wire Line
	2250 1350 2250 1550
$Comp
L Device:C_Small C?
U 1 1 60C1568C
P 1900 1250
F 0 "C?" H 1992 1296 50  0000 L CNN
F 1 "47pF" H 1992 1205 50  0000 L CNN
F 2 "" H 1900 1250 50  0001 C CNN
F 3 "~" H 1900 1250 50  0001 C CNN
	1    1900 1250
	1    0    0    -1  
$EndComp
Connection ~ 1900 1350
Wire Wire Line
	1900 1350 2250 1350
$Comp
L power:GND #PWR?
U 1 1 60C16203
P 1900 1150
F 0 "#PWR?" H 1900 900 50  0001 C CNN
F 1 "GND" H 1905 977 50  0000 C CNN
F 2 "" H 1900 1150 50  0001 C CNN
F 3 "" H 1900 1150 50  0001 C CNN
	1    1900 1150
	-1   0    0    1   
$EndComp
Text Label 2250 2250 0    50   ~ 0
P48
Text Label 2250 2150 0    50   ~ 0
P49
Text Label 1500 2250 2    50   ~ 0
P50
$Comp
L power:GND #PWR?
U 1 1 60C3F571
P 1200 2250
AR Path="/609BFBB9/60C3F571" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60C3F571" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1200 2000 50  0001 C CNN
F 1 "GND" H 1205 2077 50  0000 C CNN
F 2 "" H 1200 2250 50  0001 C CNN
F 3 "" H 1200 2250 50  0001 C CNN
	1    1200 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2250 1200 2150
Wire Wire Line
	1200 2150 1500 2150
NoConn ~ 1500 1950
$Comp
L power:GND #PWR?
U 1 1 60C4089F
P 2400 1650
AR Path="/609BFBB9/60C4089F" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60C4089F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2400 1400 50  0001 C CNN
F 1 "GND" V 2405 1522 50  0000 R CNN
F 2 "" H 2400 1650 50  0001 C CNN
F 3 "" H 2400 1650 50  0001 C CNN
	1    2400 1650
	0    -1   -1   0   
$EndComp
Text GLabel 1500 1850 0    50   Input ~ 0
3.3V_ADC
Text GLabel 1500 1650 0    50   Input ~ 0
3.3V_ADC
Text Label 1500 2050 2    50   ~ 0
A03
Text Label 2250 2050 0    50   ~ 0
A02
Text Label 2250 1750 0    50   ~ 0
A01
$Comp
L power:GND #PWR?
U 1 1 60C47B17
P 2400 1850
AR Path="/609BFBB9/60C47B17" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60C47B17" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2400 1600 50  0001 C CNN
F 1 "GND" V 2405 1722 50  0000 R CNN
F 2 "" H 2400 1850 50  0001 C CNN
F 3 "" H 2400 1850 50  0001 C CNN
	1    2400 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 1650 2400 1650
Wire Wire Line
	2250 1850 2400 1850
Text GLabel 2700 1050 1    50   Input ~ 0
3.3V_ADC
$Comp
L Device:C_Small C?
U 1 1 60C48DE5
P 2450 1250
F 0 "C?" H 2542 1296 50  0000 L CNN
F 1 "0.1uF" H 2542 1205 50  0000 L CNN
F 2 "" H 2450 1250 50  0001 C CNN
F 3 "~" H 2450 1250 50  0001 C CNN
	1    2450 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60C497BA
P 2900 1250
F 0 "C?" H 2992 1296 50  0000 L CNN
F 1 "4.7uF" H 2992 1205 50  0000 L CNN
F 2 "" H 2900 1250 50  0001 C CNN
F 3 "~" H 2900 1250 50  0001 C CNN
	1    2900 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1050 2450 1150
Wire Wire Line
	2900 1050 2900 1150
Wire Wire Line
	2450 1050 2900 1050
$Comp
L power:GND #PWR?
U 1 1 60C4A300
P 2700 1400
AR Path="/609BFBB9/60C4A300" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60C4A300" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2700 1150 50  0001 C CNN
F 1 "GND" H 2705 1227 50  0000 C CNN
F 2 "" H 2700 1400 50  0001 C CNN
F 3 "" H 2700 1400 50  0001 C CNN
	1    2700 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1350 2450 1400
Wire Wire Line
	2450 1400 2700 1400
Wire Wire Line
	2900 1350 2900 1400
Wire Wire Line
	2900 1400 2700 1400
Connection ~ 2700 1400
Text GLabel 3550 1050 1    50   Input ~ 0
3.3V_ADC
$Comp
L Device:C_Small C?
U 1 1 60C4E119
P 3300 1250
F 0 "C?" H 3392 1296 50  0000 L CNN
F 1 "0.1uF" H 3392 1205 50  0000 L CNN
F 2 "" H 3300 1250 50  0001 C CNN
F 3 "~" H 3300 1250 50  0001 C CNN
	1    3300 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60C4E11F
P 3750 1250
F 0 "C?" H 3842 1296 50  0000 L CNN
F 1 "4.7uF" H 3842 1205 50  0000 L CNN
F 2 "" H 3750 1250 50  0001 C CNN
F 3 "~" H 3750 1250 50  0001 C CNN
	1    3750 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1050 3300 1150
Wire Wire Line
	3750 1050 3750 1150
Wire Wire Line
	3300 1050 3750 1050
$Comp
L power:GND #PWR?
U 1 1 60C4E128
P 3550 1400
AR Path="/609BFBB9/60C4E128" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60C4E128" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3550 1150 50  0001 C CNN
F 1 "GND" H 3555 1227 50  0000 C CNN
F 2 "" H 3550 1400 50  0001 C CNN
F 3 "" H 3550 1400 50  0001 C CNN
	1    3550 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1350 3300 1400
Wire Wire Line
	3300 1400 3550 1400
Wire Wire Line
	3750 1350 3750 1400
Wire Wire Line
	3750 1400 3550 1400
Connection ~ 3550 1400
Text Notes 1650 750  2    129  ~ 0
16 Bit DAC
Text GLabel 2450 1950 2    50   Input ~ 0
3.3V_ADC
Wire Wire Line
	2450 1950 2250 1950
$EndSCHEMATC
