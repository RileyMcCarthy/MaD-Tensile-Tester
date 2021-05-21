EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 6
Title "MaD_P2"
Date "2021-05-06"
Rev "1"
Comp "University of Guelph"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 1100 1800 0    50   Input ~ 0
VIN_IO
$Comp
L Device:C_Small C?
U 1 1 60FA54F5
P 1200 2050
AR Path="/60FA54F5" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA54F5" Ref="C76"  Part="1" 
F 0 "C76" H 1292 2096 50  0000 L CNN
F 1 "470uF" H 1292 2005 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_10x10.2" H 1200 2050 50  0001 C CNN
F 3 "~" H 1200 2050 50  0001 C CNN
	1    1200 2050
	1    0    0    -1  
$EndComp
$Comp
L MAD_Board:XL1509 U?
U 1 1 60FA54FB
P 1850 1800
AR Path="/60FA54FB" Ref="U?"  Part="1" 
AR Path="/60F79100/60FA54FB" Ref="U13"  Part="1" 
F 0 "U13" H 1550 2050 50  0000 L CNN
F 1 "XL1509-5.0" H 1900 2050 50  0000 L CNN
F 2 "Package_SO:SOP-8_3.9x4.9mm_P1.27mm" H 1700 1800 50  0001 C CNN
F 3 "" H 1700 1800 50  0001 C CNN
	1    1850 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1800 1200 1800
Wire Wire Line
	1200 1950 1200 1800
Connection ~ 1200 1800
Wire Wire Line
	1200 1800 1450 1800
$Comp
L Device:D_Schottky_Small D?
U 1 1 60FA5505
P 2350 2050
AR Path="/60FA5505" Ref="D?"  Part="1" 
AR Path="/60F79100/60FA5505" Ref="D10"  Part="1" 
F 0 "D10" V 2304 2120 50  0000 L CNN
F 1 "SS36" V 2395 2120 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" V 2350 2050 50  0001 C CNN
F 3 "~" V 2350 2050 50  0001 C CNN
	1    2350 2050
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA550B
P 2750 2050
AR Path="/60FA550B" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA550B" Ref="C78"  Part="1" 
F 0 "C78" H 2842 2096 50  0000 L CNN
F 1 "180uF" H 2842 2005 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_6.3x7.7" H 2750 2050 50  0001 C CNN
F 3 "~" H 2750 2050 50  0001 C CNN
	1    2750 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Small L?
U 1 1 60FA5511
P 2550 1800
AR Path="/60FA5511" Ref="L?"  Part="1" 
AR Path="/60F79100/60FA5511" Ref="L10"  Part="1" 
F 0 "L10" V 2735 1800 50  0000 C CNN
F 1 "68uH/2A" V 2644 1800 50  0000 C CNN
F 2 "Inductor_SMD:L_10.4x10.4_H4.8" H 2550 1800 50  0001 C CNN
F 3 "~" H 2550 1800 50  0001 C CNN
	1    2550 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 1800 2350 1800
Wire Wire Line
	2350 1950 2350 1800
Connection ~ 2350 1800
Wire Wire Line
	2350 1800 2450 1800
Wire Wire Line
	2650 1800 2750 1800
Wire Wire Line
	2750 1950 2750 1800
Connection ~ 2750 1800
Wire Wire Line
	2750 1800 2750 1500
Wire Wire Line
	2750 1500 1850 1500
$Comp
L power:GND #PWR?
U 1 1 60FA5520
P 1200 2150
AR Path="/60FA5520" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5520" Ref="#PWR083"  Part="1" 
F 0 "#PWR083" H 1200 1900 50  0001 C CNN
F 1 "GND" H 1205 1977 50  0000 C CNN
F 2 "" H 1200 2150 50  0001 C CNN
F 3 "" H 1200 2150 50  0001 C CNN
	1    1200 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5526
P 1850 2150
AR Path="/60FA5526" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5526" Ref="#PWR085"  Part="1" 
F 0 "#PWR085" H 1850 1900 50  0001 C CNN
F 1 "GND" H 1855 1977 50  0000 C CNN
F 2 "" H 1850 2150 50  0001 C CNN
F 3 "" H 1850 2150 50  0001 C CNN
	1    1850 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2150 1750 2150
Connection ~ 1750 2150
Wire Wire Line
	1750 2150 1850 2150
Connection ~ 1850 2150
Wire Wire Line
	1850 2150 1950 2150
Connection ~ 1950 2150
Wire Wire Line
	1950 2150 2050 2150
$Comp
L power:GND #PWR?
U 1 1 60FA5533
P 2350 2150
AR Path="/60FA5533" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5533" Ref="#PWR087"  Part="1" 
F 0 "#PWR087" H 2350 1900 50  0001 C CNN
F 1 "GND" H 2355 1977 50  0000 C CNN
F 2 "" H 2350 2150 50  0001 C CNN
F 3 "" H 2350 2150 50  0001 C CNN
	1    2350 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5539
P 2750 2150
AR Path="/60FA5539" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5539" Ref="#PWR089"  Part="1" 
F 0 "#PWR089" H 2750 1900 50  0001 C CNN
F 1 "GND" H 2755 1977 50  0000 C CNN
F 2 "" H 2750 2150 50  0001 C CNN
F 3 "" H 2750 2150 50  0001 C CNN
	1    2750 2150
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 60FA553F
P 4550 1350
AR Path="/60FA553F" Ref="U?"  Part="1" 
AR Path="/60F79100/60FA553F" Ref="U15"  Part="1" 
F 0 "U15" H 4550 1592 50  0000 C CNN
F 1 "AMS1117-3.3" H 4550 1501 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4550 1550 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4650 1100 50  0001 C CNN
	1    4550 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA5545
P 3600 1550
AR Path="/60FA5545" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA5545" Ref="C80"  Part="1" 
F 0 "C80" H 3692 1596 50  0000 L CNN
F 1 "4.7uF" H 3692 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3600 1550 50  0001 C CNN
F 3 "~" H 3600 1550 50  0001 C CNN
	1    3600 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA554B
P 4050 1550
AR Path="/60FA554B" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA554B" Ref="C84"  Part="1" 
F 0 "C84" H 4142 1596 50  0000 L CNN
F 1 "1uF" H 4142 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4050 1550 50  0001 C CNN
F 3 "~" H 4050 1550 50  0001 C CNN
	1    4050 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5551
P 3600 1650
AR Path="/60FA5551" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5551" Ref="#PWR091"  Part="1" 
F 0 "#PWR091" H 3600 1400 50  0001 C CNN
F 1 "GND" H 3605 1477 50  0000 C CNN
F 2 "" H 3600 1650 50  0001 C CNN
F 3 "" H 3600 1650 50  0001 C CNN
	1    3600 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5557
P 4050 1650
AR Path="/60FA5557" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5557" Ref="#PWR095"  Part="1" 
F 0 "#PWR095" H 4050 1400 50  0001 C CNN
F 1 "GND" H 4055 1477 50  0000 C CNN
F 2 "" H 4050 1650 50  0001 C CNN
F 3 "" H 4050 1650 50  0001 C CNN
	1    4050 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA555D
P 4550 1650
AR Path="/60FA555D" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA555D" Ref="#PWR099"  Part="1" 
F 0 "#PWR099" H 4550 1400 50  0001 C CNN
F 1 "GND" H 4555 1477 50  0000 C CNN
F 2 "" H 4550 1650 50  0001 C CNN
F 3 "" H 4550 1650 50  0001 C CNN
	1    4550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 1450 4050 1350
Connection ~ 4050 1350
Wire Wire Line
	4050 1350 4250 1350
Wire Wire Line
	3600 1450 3600 1350
Connection ~ 3600 1350
Wire Wire Line
	3600 1350 4050 1350
Wire Wire Line
	5600 1350 5600 1200
Wire Wire Line
	5600 1200 5750 1200
Wire Wire Line
	5600 1350 5750 1350
Connection ~ 5600 1350
Wire Wire Line
	5600 1200 5600 1050
Wire Wire Line
	5600 1050 5750 1050
Connection ~ 5600 1200
Wire Wire Line
	5600 1050 5600 900 
Wire Wire Line
	5600 900  5750 900 
Connection ~ 5600 1050
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 60FA5592
P 4550 2300
AR Path="/60FA5592" Ref="U?"  Part="1" 
AR Path="/60F79100/60FA5592" Ref="U16"  Part="1" 
F 0 "U16" H 4550 2542 50  0000 C CNN
F 1 "AMS1117-3.3" H 4550 2451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4550 2500 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4650 2050 50  0001 C CNN
	1    4550 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA5598
P 3600 2500
AR Path="/60FA5598" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA5598" Ref="C81"  Part="1" 
F 0 "C81" H 3692 2546 50  0000 L CNN
F 1 "4.7uF" H 3692 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3600 2500 50  0001 C CNN
F 3 "~" H 3600 2500 50  0001 C CNN
	1    3600 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA559E
P 4050 2500
AR Path="/60FA559E" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA559E" Ref="C85"  Part="1" 
F 0 "C85" H 4142 2546 50  0000 L CNN
F 1 "1uF" H 4142 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4050 2500 50  0001 C CNN
F 3 "~" H 4050 2500 50  0001 C CNN
	1    4050 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA55A4
P 3600 2600
AR Path="/60FA55A4" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA55A4" Ref="#PWR092"  Part="1" 
F 0 "#PWR092" H 3600 2350 50  0001 C CNN
F 1 "GND" H 3605 2427 50  0000 C CNN
F 2 "" H 3600 2600 50  0001 C CNN
F 3 "" H 3600 2600 50  0001 C CNN
	1    3600 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA55AA
P 4050 2600
AR Path="/60FA55AA" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA55AA" Ref="#PWR096"  Part="1" 
F 0 "#PWR096" H 4050 2350 50  0001 C CNN
F 1 "GND" H 4055 2427 50  0000 C CNN
F 2 "" H 4050 2600 50  0001 C CNN
F 3 "" H 4050 2600 50  0001 C CNN
	1    4050 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA55B0
P 4550 2600
AR Path="/60FA55B0" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA55B0" Ref="#PWR0100"  Part="1" 
F 0 "#PWR0100" H 4550 2350 50  0001 C CNN
F 1 "GND" H 4555 2427 50  0000 C CNN
F 2 "" H 4550 2600 50  0001 C CNN
F 3 "" H 4550 2600 50  0001 C CNN
	1    4550 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2400 4050 2300
Connection ~ 4050 2300
Wire Wire Line
	4050 2300 4250 2300
Wire Wire Line
	3600 2400 3600 2300
Connection ~ 3600 2300
Wire Wire Line
	3600 2300 4050 2300
Wire Wire Line
	5600 2750 5600 2600
Wire Wire Line
	5600 2750 5750 2750
Connection ~ 5600 2750
Wire Wire Line
	5600 2600 5750 2600
Wire Wire Line
	5600 2600 5600 2450
Wire Wire Line
	5600 2450 5750 2450
Connection ~ 5600 2600
Wire Wire Line
	5600 2450 5600 2300
Wire Wire Line
	5600 2300 5750 2300
Connection ~ 5600 2450
Text Label 5750 2600 0    50   ~ 0
V_24_27
Text Label 5750 2750 0    50   ~ 0
V_28_31
Wire Wire Line
	3250 1800 3250 2300
Wire Wire Line
	2750 1800 3250 1800
Wire Wire Line
	3250 2300 3600 2300
Wire Wire Line
	3250 1800 3250 1350
Connection ~ 3250 1800
Wire Wire Line
	3250 1350 3600 1350
Wire Wire Line
	5600 1950 5600 1800
Wire Wire Line
	5600 1800 5750 1800
Wire Wire Line
	5600 1950 5750 1950
Wire Wire Line
	5600 1800 5600 1650
Wire Wire Line
	5600 1650 5750 1650
Connection ~ 5600 1800
Wire Wire Line
	5600 1650 5600 1500
Wire Wire Line
	5600 1500 5750 1500
Connection ~ 5600 1650
Wire Wire Line
	5600 3350 5600 3200
Wire Wire Line
	5600 3350 5750 3350
Wire Wire Line
	5600 3200 5750 3200
Wire Wire Line
	5600 3200 5600 3050
Wire Wire Line
	5600 3050 5750 3050
Connection ~ 5600 3200
Wire Wire Line
	5600 3050 5600 2900
Wire Wire Line
	5600 2900 5750 2900
Connection ~ 5600 3050
Text Label 5750 2450 0    50   ~ 0
V_20_23
Text Label 5750 2300 0    50   ~ 0
V_16_19
Text Label 5750 1350 0    50   ~ 0
V_12_15
Text Label 5750 1200 0    50   ~ 0
V_8_11
Text Label 5750 1050 0    50   ~ 0
V_4_7
Text Label 5750 900  0    50   ~ 0
V_0_3
Text Label 5750 1500 0    50   ~ 0
V_32_35
Text Label 5750 1650 0    50   ~ 0
V_36_39
Text Label 5750 1800 0    50   ~ 0
V_40_43
Text Label 5750 1950 0    50   ~ 0
V_44_47
Text Label 5750 2900 0    50   ~ 0
V_48_51
Text Label 5750 3050 0    50   ~ 0
V_52_55
Text Label 5750 3200 0    50   ~ 0
V_56_59
Text Label 5750 3350 0    50   ~ 0
V_60_63
Wire Wire Line
	4850 1350 5600 1350
$Comp
L Device:C_Small C?
U 1 1 60A3EBAB
P 1100 6100
AR Path="/60A3EBAB" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EBAB" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EBAB" Ref="C92"  Part="1" 
F 0 "C92" H 1192 6146 50  0000 L CNN
F 1 "470uF" H 1192 6055 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_10x10.2" H 1100 6100 50  0001 C CNN
F 3 "~" H 1100 6100 50  0001 C CNN
	1    1100 6100
	1    0    0    -1  
$EndComp
$Comp
L MAD_Board:XL1509 U?
U 1 1 60A3EBB1
P 1750 5850
AR Path="/60A3EBB1" Ref="U?"  Part="1" 
AR Path="/60E5FA00/60A3EBB1" Ref="U?"  Part="1" 
AR Path="/60F79100/60A3EBB1" Ref="U22"  Part="1" 
F 0 "U22" H 1450 6100 50  0000 L CNN
F 1 "XL1509-5.0" H 1800 6100 50  0000 L CNN
F 2 "Package_SO:SOP-8_3.9x4.9mm_P1.27mm" H 1600 5850 50  0001 C CNN
F 3 "" H 1600 5850 50  0001 C CNN
	1    1750 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 5850 1100 5850
Wire Wire Line
	1100 6000 1100 5850
Connection ~ 1100 5850
Wire Wire Line
	1100 5850 1350 5850
$Comp
L Device:D_Schottky_Small D?
U 1 1 60A3EBBB
P 2250 6100
AR Path="/60A3EBBB" Ref="D?"  Part="1" 
AR Path="/60E5FA00/60A3EBBB" Ref="D?"  Part="1" 
AR Path="/60F79100/60A3EBBB" Ref="D6"  Part="1" 
F 0 "D6" V 2204 6170 50  0000 L CNN
F 1 "SS36" V 2295 6170 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" V 2250 6100 50  0001 C CNN
F 3 "~" V 2250 6100 50  0001 C CNN
	1    2250 6100
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60A3EBC1
P 2650 6100
AR Path="/60A3EBC1" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EBC1" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EBC1" Ref="C93"  Part="1" 
F 0 "C93" H 2742 6146 50  0000 L CNN
F 1 "180uF" H 2742 6055 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_6.3x7.7" H 2650 6100 50  0001 C CNN
F 3 "~" H 2650 6100 50  0001 C CNN
	1    2650 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Small L?
U 1 1 60A3EBCD
P 2450 5850
AR Path="/60A3EBCD" Ref="L?"  Part="1" 
AR Path="/60E5FA00/60A3EBCD" Ref="L?"  Part="1" 
AR Path="/60F79100/60A3EBCD" Ref="L6"  Part="1" 
F 0 "L6" V 2635 5850 50  0000 C CNN
F 1 "68uH/2A" V 2544 5850 50  0000 C CNN
F 2 "Inductor_SMD:L_10.4x10.4_H4.8" H 2450 5850 50  0001 C CNN
F 3 "~" H 2450 5850 50  0001 C CNN
	1    2450 5850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2150 5850 2250 5850
Wire Wire Line
	2250 6000 2250 5850
Connection ~ 2250 5850
Wire Wire Line
	2250 5850 2350 5850
Wire Wire Line
	2550 5850 2650 5850
Wire Wire Line
	2650 6000 2650 5850
Connection ~ 2650 5850
Wire Wire Line
	2650 5850 2650 5550
Wire Wire Line
	2650 5550 1750 5550
$Comp
L power:GND #PWR?
U 1 1 60A3EBDC
P 1100 6200
AR Path="/60A3EBDC" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EBDC" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EBDC" Ref="#PWR0126"  Part="1" 
F 0 "#PWR0126" H 1100 5950 50  0001 C CNN
F 1 "GND" H 1105 6027 50  0000 C CNN
F 2 "" H 1100 6200 50  0001 C CNN
F 3 "" H 1100 6200 50  0001 C CNN
	1    1100 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EBE2
P 1750 6200
AR Path="/60A3EBE2" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EBE2" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EBE2" Ref="#PWR0127"  Part="1" 
F 0 "#PWR0127" H 1750 5950 50  0001 C CNN
F 1 "GND" H 1755 6027 50  0000 C CNN
F 2 "" H 1750 6200 50  0001 C CNN
F 3 "" H 1750 6200 50  0001 C CNN
	1    1750 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 6200 1650 6200
Connection ~ 1650 6200
Wire Wire Line
	1650 6200 1750 6200
Connection ~ 1750 6200
Wire Wire Line
	1750 6200 1850 6200
Connection ~ 1850 6200
Wire Wire Line
	1850 6200 1950 6200
$Comp
L power:GND #PWR?
U 1 1 60A3EBEF
P 2250 6200
AR Path="/60A3EBEF" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EBEF" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EBEF" Ref="#PWR0128"  Part="1" 
F 0 "#PWR0128" H 2250 5950 50  0001 C CNN
F 1 "GND" H 2255 6027 50  0000 C CNN
F 2 "" H 2250 6200 50  0001 C CNN
F 3 "" H 2250 6200 50  0001 C CNN
	1    2250 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EBF5
P 2650 6200
AR Path="/60A3EBF5" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EBF5" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EBF5" Ref="#PWR0129"  Part="1" 
F 0 "#PWR0129" H 2650 5950 50  0001 C CNN
F 1 "GND" H 2655 6027 50  0000 C CNN
F 2 "" H 2650 6200 50  0001 C CNN
F 3 "" H 2650 6200 50  0001 C CNN
	1    2650 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5850 2800 5850
Text Notes 650  5350 0    118  ~ 0
ADC/DAC Power
Text GLabel 1000 5850 0    50   Input ~ 0
VIN_ADC
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 60A3EBFE
P 4800 5850
AR Path="/60E5FA00/60A3EBFE" Ref="U?"  Part="1" 
AR Path="/60F79100/60A3EBFE" Ref="U24"  Part="1" 
F 0 "U24" H 4800 6092 50  0000 C CNN
F 1 "AMS1117-3.3" H 4800 6001 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4800 6050 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4900 5600 50  0001 C CNN
	1    4800 5850
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60A3EC04
P 4300 6050
AR Path="/60A3EC04" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EC04" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EC04" Ref="C97"  Part="1" 
F 0 "C97" H 4392 6096 50  0000 L CNN
F 1 "1uF" H 4392 6005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4300 6050 50  0001 C CNN
F 3 "~" H 4300 6050 50  0001 C CNN
	1    4300 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EC0A
P 4300 6150
AR Path="/60A3EC0A" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EC0A" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EC0A" Ref="#PWR0130"  Part="1" 
F 0 "#PWR0130" H 4300 5900 50  0001 C CNN
F 1 "GND" H 4305 5977 50  0000 C CNN
F 2 "" H 4300 6150 50  0001 C CNN
F 3 "" H 4300 6150 50  0001 C CNN
	1    4300 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EC10
P 4800 6150
AR Path="/60A3EC10" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EC10" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EC10" Ref="#PWR0131"  Part="1" 
F 0 "#PWR0131" H 4800 5900 50  0001 C CNN
F 1 "GND" H 4805 5977 50  0000 C CNN
F 2 "" H 4800 6150 50  0001 C CNN
F 3 "" H 4800 6150 50  0001 C CNN
	1    4800 6150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60A3EC16
P 3900 6050
AR Path="/60A3EC16" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EC16" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EC16" Ref="C95"  Part="1" 
F 0 "C95" H 3992 6096 50  0000 L CNN
F 1 "4.7uF" H 3992 6005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3900 6050 50  0001 C CNN
F 3 "~" H 3900 6050 50  0001 C CNN
	1    3900 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EC1C
P 3900 6150
AR Path="/60A3EC1C" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EC1C" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EC1C" Ref="#PWR0132"  Part="1" 
F 0 "#PWR0132" H 3900 5900 50  0001 C CNN
F 1 "GND" H 3905 5977 50  0000 C CNN
F 2 "" H 3900 6150 50  0001 C CNN
F 3 "" H 3900 6150 50  0001 C CNN
	1    3900 6150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60A3EC22
P 5200 6050
AR Path="/60A3EC22" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EC22" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EC22" Ref="C100"  Part="1" 
F 0 "C100" H 5292 6096 50  0000 L CNN
F 1 "1uF" H 5292 6005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5200 6050 50  0001 C CNN
F 3 "~" H 5200 6050 50  0001 C CNN
	1    5200 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3EC28
P 5200 6150
AR Path="/60A3EC28" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3EC28" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3EC28" Ref="#PWR0133"  Part="1" 
F 0 "#PWR0133" H 5200 5900 50  0001 C CNN
F 1 "GND" H 5205 5977 50  0000 C CNN
F 2 "" H 5200 6150 50  0001 C CNN
F 3 "" H 5200 6150 50  0001 C CNN
	1    5200 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 5850 4300 5850
Wire Wire Line
	4300 5850 4300 5950
Wire Wire Line
	4300 5850 3900 5850
Wire Wire Line
	3900 5850 3900 5950
Connection ~ 4300 5850
Wire Wire Line
	5100 5850 5200 5850
Wire Wire Line
	5200 5850 5200 5950
$Comp
L Device:C_Small C?
U 1 1 60A3EC97
P 5550 6050
AR Path="/60A3EC97" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60A3EC97" Ref="C?"  Part="1" 
AR Path="/60F79100/60A3EC97" Ref="C101"  Part="1" 
F 0 "C101" H 5642 6096 50  0000 L CNN
F 1 "4.7uF" H 5642 6005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5550 6050 50  0001 C CNN
F 3 "~" H 5550 6050 50  0001 C CNN
	1    5550 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60A3ECAB
P 5550 6150
AR Path="/60A3ECAB" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60A3ECAB" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60A3ECAB" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0141" H 5550 5900 50  0001 C CNN
F 1 "GND" H 5555 5977 50  0000 C CNN
F 2 "" H 5550 6150 50  0001 C CNN
F 3 "" H 5550 6150 50  0001 C CNN
	1    5550 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5850 5550 5850
Wire Wire Line
	5550 5850 5550 5950
Connection ~ 5200 5850
Text GLabel 2800 5850 2    50   Input ~ 0
5V_ADC
Text GLabel 3900 5850 0    50   Input ~ 0
5V_ADC
Text GLabel 5650 5850 2    50   Input ~ 0
3.3V_ADC
Wire Wire Line
	5550 5850 5650 5850
Connection ~ 5550 5850
Text Notes 700  800  0    129  ~ 0
IO Power
Wire Notes Line
	500  5050 6950 5050
Wire Notes Line
	6950 500  6950 7800
Wire Wire Line
	5600 2900 5600 2750
Connection ~ 5600 2900
Wire Wire Line
	5600 1500 5600 1350
Connection ~ 5600 1500
Wire Wire Line
	4850 2300 5600 2300
Connection ~ 5600 2300
Text GLabel 1100 3950 0    50   Input ~ 0
VIN_MAIN
Wire Wire Line
	2750 3950 3250 3950
Wire Wire Line
	3250 3950 3250 4450
Wire Wire Line
	3250 4450 3600 4450
Connection ~ 3600 4450
Wire Wire Line
	3600 4550 3600 4450
Wire Wire Line
	3600 4450 4050 4450
Wire Wire Line
	4050 4450 4250 4450
Connection ~ 4050 4450
Wire Wire Line
	4050 4550 4050 4450
$Comp
L power:GND #PWR?
U 1 1 60FA56A9
P 4550 4750
AR Path="/60FA56A9" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA56A9" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 4550 4500 50  0001 C CNN
F 1 "GND" H 4555 4577 50  0000 C CNN
F 2 "" H 4550 4750 50  0001 C CNN
F 3 "" H 4550 4750 50  0001 C CNN
	1    4550 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA56A3
P 4050 4750
AR Path="/60FA56A3" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA56A3" Ref="#PWR098"  Part="1" 
F 0 "#PWR098" H 4050 4500 50  0001 C CNN
F 1 "GND" H 4055 4577 50  0000 C CNN
F 2 "" H 4050 4750 50  0001 C CNN
F 3 "" H 4050 4750 50  0001 C CNN
	1    4050 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA569D
P 3600 4750
AR Path="/60FA569D" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA569D" Ref="#PWR094"  Part="1" 
F 0 "#PWR094" H 3600 4500 50  0001 C CNN
F 1 "GND" H 3605 4577 50  0000 C CNN
F 2 "" H 3600 4750 50  0001 C CNN
F 3 "" H 3600 4750 50  0001 C CNN
	1    3600 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA5697
P 4050 4650
AR Path="/60FA5697" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA5697" Ref="C87"  Part="1" 
F 0 "C87" H 4142 4696 50  0000 L CNN
F 1 "1uF" H 4142 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4050 4650 50  0001 C CNN
F 3 "~" H 4050 4650 50  0001 C CNN
	1    4050 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA5691
P 3600 4650
AR Path="/60FA5691" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA5691" Ref="C83"  Part="1" 
F 0 "C83" H 3692 4696 50  0000 L CNN
F 1 "4.7uF" H 3692 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3600 4650 50  0001 C CNN
F 3 "~" H 3600 4650 50  0001 C CNN
	1    3600 4650
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 60FA568B
P 4550 4450
AR Path="/60FA568B" Ref="U?"  Part="1" 
AR Path="/60F79100/60FA568B" Ref="U18"  Part="1" 
F 0 "U18" H 4550 4692 50  0000 C CNN
F 1 "AMS1117-3.3" H 4550 4601 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4550 4650 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4650 4200 50  0001 C CNN
	1    4550 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5632
P 2750 4300
AR Path="/60FA5632" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5632" Ref="#PWR090"  Part="1" 
F 0 "#PWR090" H 2750 4050 50  0001 C CNN
F 1 "GND" H 2755 4127 50  0000 C CNN
F 2 "" H 2750 4300 50  0001 C CNN
F 3 "" H 2750 4300 50  0001 C CNN
	1    2750 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA562C
P 2350 4300
AR Path="/60FA562C" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA562C" Ref="#PWR088"  Part="1" 
F 0 "#PWR088" H 2350 4050 50  0001 C CNN
F 1 "GND" H 2355 4127 50  0000 C CNN
F 2 "" H 2350 4300 50  0001 C CNN
F 3 "" H 2350 4300 50  0001 C CNN
	1    2350 4300
	1    0    0    -1  
$EndComp
Connection ~ 1850 4300
$Comp
L power:GND #PWR?
U 1 1 60FA561F
P 1850 4300
AR Path="/60FA561F" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA561F" Ref="#PWR086"  Part="1" 
F 0 "#PWR086" H 1850 4050 50  0001 C CNN
F 1 "GND" H 1855 4127 50  0000 C CNN
F 2 "" H 1850 4300 50  0001 C CNN
F 3 "" H 1850 4300 50  0001 C CNN
	1    1850 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA5619
P 1200 4300
AR Path="/60FA5619" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA5619" Ref="#PWR084"  Part="1" 
F 0 "#PWR084" H 1200 4050 50  0001 C CNN
F 1 "GND" H 1205 4127 50  0000 C CNN
F 2 "" H 1200 4300 50  0001 C CNN
F 3 "" H 1200 4300 50  0001 C CNN
	1    1200 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3650 1850 3650
Wire Wire Line
	2750 3950 2750 3650
Connection ~ 2750 3950
Wire Wire Line
	2750 4100 2750 3950
Wire Wire Line
	2650 3950 2750 3950
Wire Wire Line
	2350 3950 2450 3950
Wire Wire Line
	2350 4100 2350 3950
Connection ~ 2350 3950
Wire Wire Line
	2250 3950 2350 3950
$Comp
L Device:L_Small L?
U 1 1 60FA560A
P 2550 3950
AR Path="/60FA560A" Ref="L?"  Part="1" 
AR Path="/60F79100/60FA560A" Ref="L11"  Part="1" 
F 0 "L11" V 2735 3950 50  0000 C CNN
F 1 "68uH/2A" V 2644 3950 50  0000 C CNN
F 2 "Inductor_SMD:L_10.4x10.4_H4.8" H 2550 3950 50  0001 C CNN
F 3 "~" H 2550 3950 50  0001 C CNN
	1    2550 3950
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA5604
P 2750 4200
AR Path="/60FA5604" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA5604" Ref="C79"  Part="1" 
F 0 "C79" H 2842 4246 50  0000 L CNN
F 1 "180uF" H 2842 4155 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_6.3x7.7" H 2750 4200 50  0001 C CNN
F 3 "~" H 2750 4200 50  0001 C CNN
	1    2750 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky_Small D?
U 1 1 60FA55FE
P 2350 4200
AR Path="/60FA55FE" Ref="D?"  Part="1" 
AR Path="/60F79100/60FA55FE" Ref="D11"  Part="1" 
F 0 "D11" V 2304 4270 50  0000 L CNN
F 1 "SS36" V 2395 4270 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" V 2350 4200 50  0001 C CNN
F 3 "~" V 2350 4200 50  0001 C CNN
	1    2350 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 3950 1450 3950
Wire Wire Line
	1200 4100 1200 3950
Connection ~ 1200 3950
Wire Wire Line
	1100 3950 1200 3950
Wire Wire Line
	1950 4300 2050 4300
Wire Wire Line
	1850 4300 1950 4300
Connection ~ 1950 4300
Wire Wire Line
	1750 4300 1850 4300
Wire Wire Line
	1650 4300 1750 4300
Connection ~ 1750 4300
$Comp
L MAD_Board:XL1509 U?
U 1 1 60FA55F4
P 1850 3950
AR Path="/60FA55F4" Ref="U?"  Part="1" 
AR Path="/60F79100/60FA55F4" Ref="U14"  Part="1" 
F 0 "U14" H 1550 4200 50  0000 L CNN
F 1 "XL1509-5.0" H 1900 4200 50  0000 L CNN
F 2 "Package_SO:SOP-8_3.9x4.9mm_P1.27mm" H 1700 3950 50  0001 C CNN
F 3 "" H 1700 3950 50  0001 C CNN
	1    1850 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60FA55EE
P 1200 4200
AR Path="/60FA55EE" Ref="C?"  Part="1" 
AR Path="/60F79100/60FA55EE" Ref="C77"  Part="1" 
F 0 "C77" H 1292 4246 50  0000 L CNN
F 1 "470uF" H 1292 4155 50  0000 L CNN
F 2 "Capacitor_SMD:C_Elec_10x10.2" H 1200 4200 50  0001 C CNN
F 3 "~" H 1200 4200 50  0001 C CNN
	1    1200 4200
	1    0    0    -1  
$EndComp
Text GLabel 3250 3950 2    50   Input ~ 0
5V_MAIN1
Text GLabel 5400 4450 2    50   Input ~ 0
3.3V_MAIN1
$Comp
L Device:C_Small C?
U 1 1 60DEEE09
P 4950 4650
AR Path="/60DEEE09" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60DEEE09" Ref="C?"  Part="1" 
AR Path="/60F79100/60DEEE09" Ref="C?"  Part="1" 
F 0 "C?" H 5042 4696 50  0000 L CNN
F 1 "1uF" H 5042 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4950 4650 50  0001 C CNN
F 3 "~" H 4950 4650 50  0001 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60DEEE0F
P 4950 4750
AR Path="/60DEEE0F" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60DEEE0F" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60DEEE0F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4950 4500 50  0001 C CNN
F 1 "GND" H 4955 4577 50  0000 C CNN
F 2 "" H 4950 4750 50  0001 C CNN
F 3 "" H 4950 4750 50  0001 C CNN
	1    4950 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4450 4950 4450
Wire Wire Line
	4950 4450 4950 4550
$Comp
L Device:C_Small C?
U 1 1 60DEEE17
P 5300 4650
AR Path="/60DEEE17" Ref="C?"  Part="1" 
AR Path="/60E5FA00/60DEEE17" Ref="C?"  Part="1" 
AR Path="/60F79100/60DEEE17" Ref="C?"  Part="1" 
F 0 "C?" H 5392 4696 50  0000 L CNN
F 1 "4.7uF" H 5392 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5300 4650 50  0001 C CNN
F 3 "~" H 5300 4650 50  0001 C CNN
	1    5300 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60DEEE1D
P 5300 4750
AR Path="/60DEEE1D" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60DEEE1D" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60DEEE1D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5300 4500 50  0001 C CNN
F 1 "GND" H 5305 4577 50  0000 C CNN
F 2 "" H 5300 4750 50  0001 C CNN
F 3 "" H 5300 4750 50  0001 C CNN
	1    5300 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4450 5300 4450
Wire Wire Line
	5300 4450 5300 4550
Connection ~ 4950 4450
Wire Wire Line
	5300 4450 5400 4450
Connection ~ 5300 4450
Text Notes 650  3250 0    129  ~ 0
Secondary Main Power
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E21198
P 9900 850
AR Path="/60E21198" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E21198" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E21198" Ref="JP?"  Part="1" 
F 0 "JP?" H 9900 1085 50  0000 C CNN
F 1 "Jumper_1_3.3V" H 9900 994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9900 850 50  0001 C CNN
F 3 "~" H 9900 850 50  0001 C CNN
	1    9900 850 
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E2119E
P 9900 1200
AR Path="/60E2119E" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E2119E" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E2119E" Ref="JP?"  Part="1" 
F 0 "JP?" H 9900 1435 50  0000 C CNN
F 1 "Jumper_1_5V" H 9900 1344 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9900 1200 50  0001 C CNN
F 3 "~" H 9900 1200 50  0001 C CNN
	1    9900 1200
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E211A4
P 9900 1550
AR Path="/60E211A4" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E211A4" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E211A4" Ref="JP?"  Part="1" 
F 0 "JP?" H 9900 1785 50  0000 C CNN
F 1 "Jumper_1_VIN" H 9900 1694 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9900 1550 50  0001 C CNN
F 3 "~" H 9900 1550 50  0001 C CNN
	1    9900 1550
	1    0    0    -1  
$EndComp
Text GLabel 9700 1550 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E211AB
P 10650 1200
AR Path="/60E211AB" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E211AB" Ref="J?"  Part="1" 
AR Path="/60F79100/60E211AB" Ref="J?"  Part="1" 
F 0 "J?" H 10730 1192 50  0000 L CNN
F 1 "Line_1_1" H 10730 1101 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 10650 1200 50  0001 C CNN
F 3 "~" H 10650 1200 50  0001 C CNN
	1    10650 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E211B1
P 10450 1300
AR Path="/60E211B1" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E211B1" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E211B1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10450 1050 50  0001 C CNN
F 1 "GND" H 10455 1127 50  0000 C CNN
F 2 "" H 10450 1300 50  0001 C CNN
F 3 "" H 10450 1300 50  0001 C CNN
	1    10450 1300
	1    0    0    -1  
$EndComp
Text GLabel 9700 1200 0    50   Input ~ 0
+5V_1
Text GLabel 9700 850  0    50   Input ~ 0
+3.3V_1
Wire Wire Line
	10100 850  10300 850 
Wire Wire Line
	10300 850  10300 1200
Wire Wire Line
	10300 1200 10100 1200
Wire Wire Line
	10300 1200 10300 1550
Wire Wire Line
	10300 1550 10100 1550
Connection ~ 10300 1200
Wire Wire Line
	10300 1200 10450 1200
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E31F8C
P 9850 3150
AR Path="/60E31F8C" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E31F8C" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E31F8C" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 3385 50  0000 C CNN
F 1 "Jumper_2_3.3V" H 9850 3294 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 3150 50  0001 C CNN
F 3 "~" H 9850 3150 50  0001 C CNN
	1    9850 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E31F92
P 9850 3500
AR Path="/60E31F92" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E31F92" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E31F92" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 3735 50  0000 C CNN
F 1 "Jumper_2_5V" H 9850 3644 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 3500 50  0001 C CNN
F 3 "~" H 9850 3500 50  0001 C CNN
	1    9850 3500
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E31F98
P 9850 3850
AR Path="/60E31F98" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E31F98" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E31F98" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 4085 50  0000 C CNN
F 1 "Jumper_2_VIN" H 9850 3994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 3850 50  0001 C CNN
F 3 "~" H 9850 3850 50  0001 C CNN
	1    9850 3850
	1    0    0    -1  
$EndComp
Text GLabel 9650 3850 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E31F9F
P 10650 3500
AR Path="/60E31F9F" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E31F9F" Ref="J?"  Part="1" 
AR Path="/60F79100/60E31F9F" Ref="J?"  Part="1" 
F 0 "J?" H 10730 3492 50  0000 L CNN
F 1 "Line_2_1" H 10730 3401 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 10650 3500 50  0001 C CNN
F 3 "~" H 10650 3500 50  0001 C CNN
	1    10650 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E31FA5
P 10450 3600
AR Path="/60E31FA5" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E31FA5" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E31FA5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10450 3350 50  0001 C CNN
F 1 "GND" H 10455 3427 50  0000 C CNN
F 2 "" H 10450 3600 50  0001 C CNN
F 3 "" H 10450 3600 50  0001 C CNN
	1    10450 3600
	1    0    0    -1  
$EndComp
Text GLabel 9650 3500 0    50   Input ~ 0
+5V_2
Text GLabel 9650 3150 0    50   Input ~ 0
+3.3V_2
Wire Wire Line
	10050 3150 10250 3150
Wire Wire Line
	10250 3150 10250 3500
Wire Wire Line
	10250 3500 10050 3500
Wire Wire Line
	10250 3500 10250 3850
Wire Wire Line
	10250 3850 10050 3850
Connection ~ 10250 3500
Wire Wire Line
	10250 3500 10450 3500
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E41F7C
P 7800 4150
AR Path="/60E41F7C" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E41F7C" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E41F7C" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 4385 50  0000 C CNN
F 1 "Jumper_3_3.3V" H 7800 4294 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 4150 50  0001 C CNN
F 3 "~" H 7800 4150 50  0001 C CNN
	1    7800 4150
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E41F82
P 7800 4500
AR Path="/60E41F82" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E41F82" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E41F82" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 4735 50  0000 C CNN
F 1 "Jumper_3_5V" H 7800 4644 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 4500 50  0001 C CNN
F 3 "~" H 7800 4500 50  0001 C CNN
	1    7800 4500
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E41F88
P 7800 4850
AR Path="/60E41F88" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E41F88" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E41F88" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 5085 50  0000 C CNN
F 1 "Jumper_3_VIN" H 7800 4994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 4850 50  0001 C CNN
F 3 "~" H 7800 4850 50  0001 C CNN
	1    7800 4850
	1    0    0    -1  
$EndComp
Text GLabel 7600 4850 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E41F8F
P 8650 4500
AR Path="/60E41F8F" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E41F8F" Ref="J?"  Part="1" 
AR Path="/60F79100/60E41F8F" Ref="J?"  Part="1" 
F 0 "J?" H 8730 4492 50  0000 L CNN
F 1 "Line_3_1" H 8730 4401 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 8650 4500 50  0001 C CNN
F 3 "~" H 8650 4500 50  0001 C CNN
	1    8650 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E41F95
P 8450 4600
AR Path="/60E41F95" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E41F95" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E41F95" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8450 4350 50  0001 C CNN
F 1 "GND" H 8455 4427 50  0000 C CNN
F 2 "" H 8450 4600 50  0001 C CNN
F 3 "" H 8450 4600 50  0001 C CNN
	1    8450 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4150 8200 4150
Wire Wire Line
	8200 4150 8200 4500
Wire Wire Line
	8200 4500 8000 4500
Wire Wire Line
	8000 4850 8200 4850
Wire Wire Line
	8200 4850 8200 4500
Connection ~ 8200 4500
Wire Wire Line
	8200 4500 8450 4500
Text GLabel 7600 4150 0    50   Input ~ 0
+3.3V_3
Text GLabel 7600 4500 0    50   Input ~ 0
+5V_3
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E4A165
P 9850 1950
AR Path="/60E4A165" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E4A165" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E4A165" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 2185 50  0000 C CNN
F 1 "Jumper_1_3.3V" H 9850 2094 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 1950 50  0001 C CNN
F 3 "~" H 9850 1950 50  0001 C CNN
	1    9850 1950
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E4A16B
P 9850 2300
AR Path="/60E4A16B" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E4A16B" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E4A16B" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 2535 50  0000 C CNN
F 1 "Jumper_1_5V" H 9850 2444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 2300 50  0001 C CNN
F 3 "~" H 9850 2300 50  0001 C CNN
	1    9850 2300
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E4A171
P 9850 2650
AR Path="/60E4A171" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E4A171" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E4A171" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 2885 50  0000 C CNN
F 1 "Jumper_1_VIN" H 9850 2794 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 2650 50  0001 C CNN
F 3 "~" H 9850 2650 50  0001 C CNN
	1    9850 2650
	1    0    0    -1  
$EndComp
Text GLabel 9650 2650 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E4A178
P 10600 2300
AR Path="/60E4A178" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E4A178" Ref="J?"  Part="1" 
AR Path="/60F79100/60E4A178" Ref="J?"  Part="1" 
F 0 "J?" H 10680 2292 50  0000 L CNN
F 1 "Line_1_2" H 10680 2201 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 10600 2300 50  0001 C CNN
F 3 "~" H 10600 2300 50  0001 C CNN
	1    10600 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E4A17E
P 10400 2400
AR Path="/60E4A17E" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E4A17E" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E4A17E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10400 2150 50  0001 C CNN
F 1 "GND" H 10405 2227 50  0000 C CNN
F 2 "" H 10400 2400 50  0001 C CNN
F 3 "" H 10400 2400 50  0001 C CNN
	1    10400 2400
	1    0    0    -1  
$EndComp
Text GLabel 9650 2300 0    50   Input ~ 0
+5V_1
Text GLabel 9650 1950 0    50   Input ~ 0
+3.3V_1
Wire Wire Line
	10050 1950 10250 1950
Wire Wire Line
	10250 1950 10250 2300
Wire Wire Line
	10250 2300 10050 2300
Wire Wire Line
	10250 2300 10250 2650
Wire Wire Line
	10250 2650 10050 2650
Connection ~ 10250 2300
Wire Wire Line
	10250 2300 10400 2300
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E5AEDC
P 7850 1700
AR Path="/60E5AEDC" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E5AEDC" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E5AEDC" Ref="JP?"  Part="1" 
F 0 "JP?" H 7850 1935 50  0000 C CNN
F 1 "Jumper_1_3.3V" H 7850 1844 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7850 1700 50  0001 C CNN
F 3 "~" H 7850 1700 50  0001 C CNN
	1    7850 1700
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E5AEE2
P 7850 2050
AR Path="/60E5AEE2" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E5AEE2" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E5AEE2" Ref="JP?"  Part="1" 
F 0 "JP?" H 7850 2285 50  0000 C CNN
F 1 "Jumper_1_5V" H 7850 2194 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7850 2050 50  0001 C CNN
F 3 "~" H 7850 2050 50  0001 C CNN
	1    7850 2050
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E5AEE8
P 7850 2400
AR Path="/60E5AEE8" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E5AEE8" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E5AEE8" Ref="JP?"  Part="1" 
F 0 "JP?" H 7850 2635 50  0000 C CNN
F 1 "Jumper_1_VIN" H 7850 2544 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7850 2400 50  0001 C CNN
F 3 "~" H 7850 2400 50  0001 C CNN
	1    7850 2400
	1    0    0    -1  
$EndComp
Text GLabel 7650 2400 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E5AEEF
P 8600 2050
AR Path="/60E5AEEF" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E5AEEF" Ref="J?"  Part="1" 
AR Path="/60F79100/60E5AEEF" Ref="J?"  Part="1" 
F 0 "J?" H 8680 2042 50  0000 L CNN
F 1 "Line_1_3" H 8680 1951 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 8600 2050 50  0001 C CNN
F 3 "~" H 8600 2050 50  0001 C CNN
	1    8600 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E5AEF5
P 8400 2150
AR Path="/60E5AEF5" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E5AEF5" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E5AEF5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 1900 50  0001 C CNN
F 1 "GND" H 8405 1977 50  0000 C CNN
F 2 "" H 8400 2150 50  0001 C CNN
F 3 "" H 8400 2150 50  0001 C CNN
	1    8400 2150
	1    0    0    -1  
$EndComp
Text GLabel 7650 2050 0    50   Input ~ 0
+5V_1
Text GLabel 7650 1700 0    50   Input ~ 0
+3.3V_1
Wire Wire Line
	8050 1700 8250 1700
Wire Wire Line
	8250 1700 8250 2050
Wire Wire Line
	8250 2050 8050 2050
Wire Wire Line
	8250 2050 8250 2400
Wire Wire Line
	8250 2400 8050 2400
Connection ~ 8250 2050
Wire Wire Line
	8250 2050 8400 2050
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E6D8FA
P 7800 2950
AR Path="/60E6D8FA" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E6D8FA" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E6D8FA" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 3185 50  0000 C CNN
F 1 "Jumper_2_3.3V" H 7800 3094 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 2950 50  0001 C CNN
F 3 "~" H 7800 2950 50  0001 C CNN
	1    7800 2950
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E6D900
P 7800 3300
AR Path="/60E6D900" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E6D900" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E6D900" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 3535 50  0000 C CNN
F 1 "Jumper_2_5V" H 7800 3444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 3300 50  0001 C CNN
F 3 "~" H 7800 3300 50  0001 C CNN
	1    7800 3300
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E6D906
P 7800 3650
AR Path="/60E6D906" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E6D906" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E6D906" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 3885 50  0000 C CNN
F 1 "Jumper_2_VIN" H 7800 3794 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 3650 50  0001 C CNN
F 3 "~" H 7800 3650 50  0001 C CNN
	1    7800 3650
	1    0    0    -1  
$EndComp
Text GLabel 7600 3650 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E6D90D
P 8600 3300
AR Path="/60E6D90D" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E6D90D" Ref="J?"  Part="1" 
AR Path="/60F79100/60E6D90D" Ref="J?"  Part="1" 
F 0 "J?" H 8680 3292 50  0000 L CNN
F 1 "Line_2_2" H 8680 3201 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 8600 3300 50  0001 C CNN
F 3 "~" H 8600 3300 50  0001 C CNN
	1    8600 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E6D913
P 8400 3400
AR Path="/60E6D913" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E6D913" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E6D913" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 3150 50  0001 C CNN
F 1 "GND" H 8405 3227 50  0000 C CNN
F 2 "" H 8400 3400 50  0001 C CNN
F 3 "" H 8400 3400 50  0001 C CNN
	1    8400 3400
	1    0    0    -1  
$EndComp
Text GLabel 7600 3300 0    50   Input ~ 0
+5V_2
Text GLabel 7600 2950 0    50   Input ~ 0
+3.3V_2
Wire Wire Line
	8000 2950 8200 2950
Wire Wire Line
	8200 2950 8200 3300
Wire Wire Line
	8200 3300 8000 3300
Wire Wire Line
	8200 3300 8200 3650
Wire Wire Line
	8200 3650 8000 3650
Connection ~ 8200 3300
Wire Wire Line
	8200 3300 8400 3300
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E75D0E
P 9850 4350
AR Path="/60E75D0E" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E75D0E" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E75D0E" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 4585 50  0000 C CNN
F 1 "Jumper_2_3.3V" H 9850 4494 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 4350 50  0001 C CNN
F 3 "~" H 9850 4350 50  0001 C CNN
	1    9850 4350
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E75D14
P 9850 4700
AR Path="/60E75D14" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E75D14" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E75D14" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 4935 50  0000 C CNN
F 1 "Jumper_2_5V" H 9850 4844 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 4700 50  0001 C CNN
F 3 "~" H 9850 4700 50  0001 C CNN
	1    9850 4700
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E75D1A
P 9850 5050
AR Path="/60E75D1A" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E75D1A" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E75D1A" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 5285 50  0000 C CNN
F 1 "Jumper_2_VIN" H 9850 5194 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 5050 50  0001 C CNN
F 3 "~" H 9850 5050 50  0001 C CNN
	1    9850 5050
	1    0    0    -1  
$EndComp
Text GLabel 9650 5050 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E75D21
P 10650 4700
AR Path="/60E75D21" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E75D21" Ref="J?"  Part="1" 
AR Path="/60F79100/60E75D21" Ref="J?"  Part="1" 
F 0 "J?" H 10730 4692 50  0000 L CNN
F 1 "Line_2_3" H 10730 4601 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 10650 4700 50  0001 C CNN
F 3 "~" H 10650 4700 50  0001 C CNN
	1    10650 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E75D27
P 10450 4800
AR Path="/60E75D27" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E75D27" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E75D27" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10450 4550 50  0001 C CNN
F 1 "GND" H 10455 4627 50  0000 C CNN
F 2 "" H 10450 4800 50  0001 C CNN
F 3 "" H 10450 4800 50  0001 C CNN
	1    10450 4800
	1    0    0    -1  
$EndComp
Text GLabel 9650 4700 0    50   Input ~ 0
+5V_2
Text GLabel 9650 4350 0    50   Input ~ 0
+3.3V_2
Wire Wire Line
	10050 4350 10250 4350
Wire Wire Line
	10250 4350 10250 4700
Wire Wire Line
	10250 4700 10050 4700
Wire Wire Line
	10250 4700 10250 5050
Wire Wire Line
	10250 5050 10050 5050
Connection ~ 10250 4700
Wire Wire Line
	10250 4700 10450 4700
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E7E321
P 9850 5550
AR Path="/60E7E321" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E7E321" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E7E321" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 5785 50  0000 C CNN
F 1 "Jumper_3_3.3V" H 9850 5694 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 5550 50  0001 C CNN
F 3 "~" H 9850 5550 50  0001 C CNN
	1    9850 5550
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E7E327
P 9850 5900
AR Path="/60E7E327" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E7E327" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E7E327" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 6135 50  0000 C CNN
F 1 "Jumper_3_5V" H 9850 6044 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 5900 50  0001 C CNN
F 3 "~" H 9850 5900 50  0001 C CNN
	1    9850 5900
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E7E32D
P 9850 6250
AR Path="/60E7E32D" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E7E32D" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E7E32D" Ref="JP?"  Part="1" 
F 0 "JP?" H 9850 6485 50  0000 C CNN
F 1 "Jumper_3_VIN" H 9850 6394 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 6250 50  0001 C CNN
F 3 "~" H 9850 6250 50  0001 C CNN
	1    9850 6250
	1    0    0    -1  
$EndComp
Text GLabel 9650 6250 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E7E334
P 10700 5900
AR Path="/60E7E334" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E7E334" Ref="J?"  Part="1" 
AR Path="/60F79100/60E7E334" Ref="J?"  Part="1" 
F 0 "J?" H 10780 5892 50  0000 L CNN
F 1 "Line_3_2" H 10780 5801 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 10700 5900 50  0001 C CNN
F 3 "~" H 10700 5900 50  0001 C CNN
	1    10700 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E7E33A
P 10500 6000
AR Path="/60E7E33A" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E7E33A" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E7E33A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10500 5750 50  0001 C CNN
F 1 "GND" H 10505 5827 50  0000 C CNN
F 2 "" H 10500 6000 50  0001 C CNN
F 3 "" H 10500 6000 50  0001 C CNN
	1    10500 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5550 10250 5550
Wire Wire Line
	10250 5550 10250 5900
Wire Wire Line
	10250 5900 10050 5900
Wire Wire Line
	10050 6250 10250 6250
Wire Wire Line
	10250 6250 10250 5900
Connection ~ 10250 5900
Wire Wire Line
	10250 5900 10500 5900
Text GLabel 9650 5550 0    50   Input ~ 0
+3.3V_3
Text GLabel 9650 5900 0    50   Input ~ 0
+5V_3
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E91369
P 7800 5250
AR Path="/60E91369" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E91369" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E91369" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 5485 50  0000 C CNN
F 1 "Jumper_3_3.3V" H 7800 5394 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 5250 50  0001 C CNN
F 3 "~" H 7800 5250 50  0001 C CNN
	1    7800 5250
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E9136F
P 7800 5600
AR Path="/60E9136F" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E9136F" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E9136F" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 5835 50  0000 C CNN
F 1 "Jumper_3_5V" H 7800 5744 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 5600 50  0001 C CNN
F 3 "~" H 7800 5600 50  0001 C CNN
	1    7800 5600
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 60E91375
P 7800 5950
AR Path="/60E91375" Ref="JP?"  Part="1" 
AR Path="/60E5FA00/60E91375" Ref="JP?"  Part="1" 
AR Path="/60F79100/60E91375" Ref="JP?"  Part="1" 
F 0 "JP?" H 7800 6185 50  0000 C CNN
F 1 "Jumper_3_VIN" H 7800 6094 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7800 5950 50  0001 C CNN
F 3 "~" H 7800 5950 50  0001 C CNN
	1    7800 5950
	1    0    0    -1  
$EndComp
Text GLabel 7600 5950 0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 60E9137C
P 8650 5600
AR Path="/60E9137C" Ref="J?"  Part="1" 
AR Path="/60E5FA00/60E9137C" Ref="J?"  Part="1" 
AR Path="/60F79100/60E9137C" Ref="J?"  Part="1" 
F 0 "J?" H 8730 5592 50  0000 L CNN
F 1 "Line_3_3" H 8730 5501 50  0000 L CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 8650 5600 50  0001 C CNN
F 3 "~" H 8650 5600 50  0001 C CNN
	1    8650 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E91382
P 8450 5700
AR Path="/60E91382" Ref="#PWR?"  Part="1" 
AR Path="/60E5FA00/60E91382" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60E91382" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8450 5450 50  0001 C CNN
F 1 "GND" H 8455 5527 50  0000 C CNN
F 2 "" H 8450 5700 50  0001 C CNN
F 3 "" H 8450 5700 50  0001 C CNN
	1    8450 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 5250 8200 5250
Wire Wire Line
	8200 5250 8200 5600
Wire Wire Line
	8200 5600 8000 5600
Wire Wire Line
	8000 5950 8200 5950
Wire Wire Line
	8200 5950 8200 5600
Connection ~ 8200 5600
Wire Wire Line
	8200 5600 8450 5600
Text GLabel 7600 5250 0    50   Input ~ 0
+3.3V_3
Text GLabel 7600 5600 0    50   Input ~ 0
+5V_3
Text Notes 7050 800  0    129  ~ 0
Voltage Outputs
Text GLabel 2250 7100 1    50   Input ~ 0
3.3V_ADC
$Comp
L Device:LED_Small D?
U 1 1 60F6D06D
P 2250 7450
AR Path="/609BFBB9/60F6D06D" Ref="D?"  Part="1" 
AR Path="/60F79100/60F6D06D" Ref="D?"  Part="1" 
F 0 "D?" V 2296 7380 50  0000 R CNN
F 1 "BLUE" V 2205 7380 50  0000 R CNN
F 2 "" V 2250 7450 50  0001 C CNN
F 3 "~" V 2250 7450 50  0001 C CNN
	1    2250 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60F6D073
P 2250 7250
AR Path="/609BFBB9/60F6D073" Ref="R?"  Part="1" 
AR Path="/60F79100/60F6D073" Ref="R?"  Part="1" 
F 0 "R?" H 2309 7296 50  0000 L CNN
F 1 "620R" H 2309 7205 50  0000 L CNN
F 2 "" H 2250 7250 50  0001 C CNN
F 3 "~" H 2250 7250 50  0001 C CNN
	1    2250 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60F6D079
P 2250 7550
AR Path="/609BFBB9/60F6D079" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60F6D079" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60F6D079" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2250 7300 50  0001 C CNN
F 1 "GND" H 2255 7377 50  0000 C CNN
F 2 "" H 2250 7550 50  0001 C CNN
F 3 "" H 2250 7550 50  0001 C CNN
	1    2250 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 7100 2250 7150
Text GLabel 2600 7100 1    50   Input ~ 0
5V_ADC
$Comp
L Device:LED_Small D?
U 1 1 60F6D081
P 2600 7450
AR Path="/609BFBB9/60F6D081" Ref="D?"  Part="1" 
AR Path="/60F79100/60F6D081" Ref="D?"  Part="1" 
F 0 "D?" V 2646 7380 50  0000 R CNN
F 1 "BLUE" V 2555 7380 50  0000 R CNN
F 2 "" V 2600 7450 50  0001 C CNN
F 3 "~" V 2600 7450 50  0001 C CNN
	1    2600 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60F6D087
P 2600 7250
AR Path="/609BFBB9/60F6D087" Ref="R?"  Part="1" 
AR Path="/60F79100/60F6D087" Ref="R?"  Part="1" 
F 0 "R?" H 2659 7296 50  0000 L CNN
F 1 "1K" H 2659 7205 50  0000 L CNN
F 2 "" H 2600 7250 50  0001 C CNN
F 3 "~" H 2600 7250 50  0001 C CNN
	1    2600 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60F6D08D
P 2600 7550
AR Path="/609BFBB9/60F6D08D" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60F6D08D" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60F6D08D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2600 7300 50  0001 C CNN
F 1 "GND" H 2605 7377 50  0000 C CNN
F 2 "" H 2600 7550 50  0001 C CNN
F 3 "" H 2600 7550 50  0001 C CNN
	1    2600 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 7100 2600 7150
Text GLabel 2950 7100 1    50   Input ~ 0
3.3V_MAIN1
$Comp
L Device:LED_Small D?
U 1 1 60F6D095
P 2950 7450
AR Path="/609BFBB9/60F6D095" Ref="D?"  Part="1" 
AR Path="/60F79100/60F6D095" Ref="D?"  Part="1" 
F 0 "D?" V 2996 7380 50  0000 R CNN
F 1 "BLUE" V 2905 7380 50  0000 R CNN
F 2 "" V 2950 7450 50  0001 C CNN
F 3 "~" V 2950 7450 50  0001 C CNN
	1    2950 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60F6D09B
P 2950 7250
AR Path="/609BFBB9/60F6D09B" Ref="R?"  Part="1" 
AR Path="/60F79100/60F6D09B" Ref="R?"  Part="1" 
F 0 "R?" H 3009 7296 50  0000 L CNN
F 1 "620R" H 3009 7205 50  0000 L CNN
F 2 "" H 2950 7250 50  0001 C CNN
F 3 "~" H 2950 7250 50  0001 C CNN
	1    2950 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60F6D0A1
P 2950 7550
AR Path="/609BFBB9/60F6D0A1" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60F6D0A1" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60F6D0A1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2950 7300 50  0001 C CNN
F 1 "GND" H 2955 7377 50  0000 C CNN
F 2 "" H 2950 7550 50  0001 C CNN
F 3 "" H 2950 7550 50  0001 C CNN
	1    2950 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 7100 2950 7150
Wire Wire Line
	3300 7100 3300 7150
$Comp
L power:GND #PWR?
U 1 1 60F6D0A9
P 3300 7550
AR Path="/609BFBB9/60F6D0A9" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60F6D0A9" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60F6D0A9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3300 7300 50  0001 C CNN
F 1 "GND" H 3305 7377 50  0000 C CNN
F 2 "" H 3300 7550 50  0001 C CNN
F 3 "" H 3300 7550 50  0001 C CNN
	1    3300 7550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60F6D0AF
P 3300 7250
AR Path="/609BFBB9/60F6D0AF" Ref="R?"  Part="1" 
AR Path="/60F79100/60F6D0AF" Ref="R?"  Part="1" 
F 0 "R?" H 3359 7296 50  0000 L CNN
F 1 "1K" H 3359 7205 50  0000 L CNN
F 2 "" H 3300 7250 50  0001 C CNN
F 3 "~" H 3300 7250 50  0001 C CNN
	1    3300 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60F6D0B5
P 3300 7450
AR Path="/609BFBB9/60F6D0B5" Ref="D?"  Part="1" 
AR Path="/60F79100/60F6D0B5" Ref="D?"  Part="1" 
F 0 "D?" V 3346 7380 50  0000 R CNN
F 1 "BLUE" V 3255 7380 50  0000 R CNN
F 2 "" V 3300 7450 50  0001 C CNN
F 3 "~" V 3300 7450 50  0001 C CNN
	1    3300 7450
	0    -1   -1   0   
$EndComp
Text GLabel 3300 7100 1    50   Input ~ 0
5V_MAIN1
$Comp
L power:+5V #PWR?
U 1 1 60FA1E6B
P 3650 7150
F 0 "#PWR?" H 3650 7000 50  0001 C CNN
F 1 "+5V" H 3665 7323 50  0000 C CNN
F 2 "" H 3650 7150 50  0001 C CNN
F 3 "" H 3650 7150 50  0001 C CNN
	1    3650 7150
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 60FA35D7
P 4050 7150
F 0 "#PWR?" H 4050 7000 50  0001 C CNN
F 1 "+3.3V" H 4065 7323 50  0000 C CNN
F 2 "" H 4050 7150 50  0001 C CNN
F 3 "" H 4050 7150 50  0001 C CNN
	1    4050 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FA5FE5
P 3650 7250
AR Path="/609BFBB9/60FA5FE5" Ref="R?"  Part="1" 
AR Path="/60F79100/60FA5FE5" Ref="R?"  Part="1" 
F 0 "R?" H 3709 7296 50  0000 L CNN
F 1 "1K" H 3709 7205 50  0000 L CNN
F 2 "" H 3650 7250 50  0001 C CNN
F 3 "~" H 3650 7250 50  0001 C CNN
	1    3650 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FA684D
P 4050 7250
AR Path="/609BFBB9/60FA684D" Ref="R?"  Part="1" 
AR Path="/60F79100/60FA684D" Ref="R?"  Part="1" 
F 0 "R?" H 4109 7296 50  0000 L CNN
F 1 "620R" H 4109 7205 50  0000 L CNN
F 2 "" H 4050 7250 50  0001 C CNN
F 3 "~" H 4050 7250 50  0001 C CNN
	1    4050 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60FA6F7D
P 3650 7450
AR Path="/609BFBB9/60FA6F7D" Ref="D?"  Part="1" 
AR Path="/60F79100/60FA6F7D" Ref="D?"  Part="1" 
F 0 "D?" V 3696 7380 50  0000 R CNN
F 1 "BLUE" V 3605 7380 50  0000 R CNN
F 2 "" V 3650 7450 50  0001 C CNN
F 3 "~" V 3650 7450 50  0001 C CNN
	1    3650 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60FA7727
P 4050 7450
AR Path="/609BFBB9/60FA7727" Ref="D?"  Part="1" 
AR Path="/60F79100/60FA7727" Ref="D?"  Part="1" 
F 0 "D?" V 4096 7380 50  0000 R CNN
F 1 "BLUE" V 4005 7380 50  0000 R CNN
F 2 "" V 4050 7450 50  0001 C CNN
F 3 "~" V 4050 7450 50  0001 C CNN
	1    4050 7450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA7D54
P 3650 7550
AR Path="/609BFBB9/60FA7D54" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FA7D54" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA7D54" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3650 7300 50  0001 C CNN
F 1 "GND" H 3655 7377 50  0000 C CNN
F 2 "" H 3650 7550 50  0001 C CNN
F 3 "" H 3650 7550 50  0001 C CNN
	1    3650 7550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FA80B9
P 4050 7550
AR Path="/609BFBB9/60FA80B9" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FA80B9" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FA80B9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4050 7300 50  0001 C CNN
F 1 "GND" H 4055 7377 50  0000 C CNN
F 2 "" H 4050 7550 50  0001 C CNN
F 3 "" H 4050 7550 50  0001 C CNN
	1    4050 7550
	1    0    0    -1  
$EndComp
Text GLabel 4450 7100 1    50   Input ~ 0
+3.3V_1
$Comp
L Device:LED_Small D?
U 1 1 60FB509E
P 4450 7450
AR Path="/609BFBB9/60FB509E" Ref="D?"  Part="1" 
AR Path="/60F79100/60FB509E" Ref="D?"  Part="1" 
F 0 "D?" V 4496 7380 50  0000 R CNN
F 1 "BLUE" V 4405 7380 50  0000 R CNN
F 2 "" V 4450 7450 50  0001 C CNN
F 3 "~" V 4450 7450 50  0001 C CNN
	1    4450 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FB50A4
P 4450 7250
AR Path="/609BFBB9/60FB50A4" Ref="R?"  Part="1" 
AR Path="/60F79100/60FB50A4" Ref="R?"  Part="1" 
F 0 "R?" H 4509 7296 50  0000 L CNN
F 1 "620R" H 4509 7205 50  0000 L CNN
F 2 "" H 4450 7250 50  0001 C CNN
F 3 "~" H 4450 7250 50  0001 C CNN
	1    4450 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FB50AA
P 4450 7550
AR Path="/609BFBB9/60FB50AA" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FB50AA" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FB50AA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4450 7300 50  0001 C CNN
F 1 "GND" H 4455 7377 50  0000 C CNN
F 2 "" H 4450 7550 50  0001 C CNN
F 3 "" H 4450 7550 50  0001 C CNN
	1    4450 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7100 4450 7150
Wire Wire Line
	4800 7100 4800 7150
$Comp
L power:GND #PWR?
U 1 1 60FB50B2
P 4800 7550
AR Path="/609BFBB9/60FB50B2" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FB50B2" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FB50B2" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4800 7300 50  0001 C CNN
F 1 "GND" H 4805 7377 50  0000 C CNN
F 2 "" H 4800 7550 50  0001 C CNN
F 3 "" H 4800 7550 50  0001 C CNN
	1    4800 7550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FB50B8
P 4800 7250
AR Path="/609BFBB9/60FB50B8" Ref="R?"  Part="1" 
AR Path="/60F79100/60FB50B8" Ref="R?"  Part="1" 
F 0 "R?" H 4859 7296 50  0000 L CNN
F 1 "1K" H 4859 7205 50  0000 L CNN
F 2 "" H 4800 7250 50  0001 C CNN
F 3 "~" H 4800 7250 50  0001 C CNN
	1    4800 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60FB50BE
P 4800 7450
AR Path="/609BFBB9/60FB50BE" Ref="D?"  Part="1" 
AR Path="/60F79100/60FB50BE" Ref="D?"  Part="1" 
F 0 "D?" V 4846 7380 50  0000 R CNN
F 1 "BLUE" V 4755 7380 50  0000 R CNN
F 2 "" V 4800 7450 50  0001 C CNN
F 3 "~" V 4800 7450 50  0001 C CNN
	1    4800 7450
	0    -1   -1   0   
$EndComp
Text GLabel 4800 7100 1    50   Input ~ 0
+5V_1
Text GLabel 5150 7100 1    50   Input ~ 0
+3.3V_2
$Comp
L Device:LED_Small D?
U 1 1 60FBF248
P 5150 7450
AR Path="/609BFBB9/60FBF248" Ref="D?"  Part="1" 
AR Path="/60F79100/60FBF248" Ref="D?"  Part="1" 
F 0 "D?" V 5196 7380 50  0000 R CNN
F 1 "BLUE" V 5105 7380 50  0000 R CNN
F 2 "" V 5150 7450 50  0001 C CNN
F 3 "~" V 5150 7450 50  0001 C CNN
	1    5150 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FBF24E
P 5150 7250
AR Path="/609BFBB9/60FBF24E" Ref="R?"  Part="1" 
AR Path="/60F79100/60FBF24E" Ref="R?"  Part="1" 
F 0 "R?" H 5209 7296 50  0000 L CNN
F 1 "620R" H 5209 7205 50  0000 L CNN
F 2 "" H 5150 7250 50  0001 C CNN
F 3 "~" H 5150 7250 50  0001 C CNN
	1    5150 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FBF254
P 5150 7550
AR Path="/609BFBB9/60FBF254" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FBF254" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FBF254" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5150 7300 50  0001 C CNN
F 1 "GND" H 5155 7377 50  0000 C CNN
F 2 "" H 5150 7550 50  0001 C CNN
F 3 "" H 5150 7550 50  0001 C CNN
	1    5150 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 7100 5150 7150
Wire Wire Line
	5500 7100 5500 7150
$Comp
L power:GND #PWR?
U 1 1 60FBF25C
P 5500 7550
AR Path="/609BFBB9/60FBF25C" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FBF25C" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FBF25C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5500 7300 50  0001 C CNN
F 1 "GND" H 5505 7377 50  0000 C CNN
F 2 "" H 5500 7550 50  0001 C CNN
F 3 "" H 5500 7550 50  0001 C CNN
	1    5500 7550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FBF262
P 5500 7250
AR Path="/609BFBB9/60FBF262" Ref="R?"  Part="1" 
AR Path="/60F79100/60FBF262" Ref="R?"  Part="1" 
F 0 "R?" H 5559 7296 50  0000 L CNN
F 1 "1K" H 5559 7205 50  0000 L CNN
F 2 "" H 5500 7250 50  0001 C CNN
F 3 "~" H 5500 7250 50  0001 C CNN
	1    5500 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60FBF268
P 5500 7450
AR Path="/609BFBB9/60FBF268" Ref="D?"  Part="1" 
AR Path="/60F79100/60FBF268" Ref="D?"  Part="1" 
F 0 "D?" V 5546 7380 50  0000 R CNN
F 1 "BLUE" V 5455 7380 50  0000 R CNN
F 2 "" V 5500 7450 50  0001 C CNN
F 3 "~" V 5500 7450 50  0001 C CNN
	1    5500 7450
	0    -1   -1   0   
$EndComp
Text GLabel 5500 7100 1    50   Input ~ 0
+5V_2
Text GLabel 5850 7100 1    50   Input ~ 0
+3.3V_2
$Comp
L Device:LED_Small D?
U 1 1 60FC8E6E
P 5850 7450
AR Path="/609BFBB9/60FC8E6E" Ref="D?"  Part="1" 
AR Path="/60F79100/60FC8E6E" Ref="D?"  Part="1" 
F 0 "D?" V 5896 7380 50  0000 R CNN
F 1 "BLUE" V 5805 7380 50  0000 R CNN
F 2 "" V 5850 7450 50  0001 C CNN
F 3 "~" V 5850 7450 50  0001 C CNN
	1    5850 7450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FC8E74
P 5850 7250
AR Path="/609BFBB9/60FC8E74" Ref="R?"  Part="1" 
AR Path="/60F79100/60FC8E74" Ref="R?"  Part="1" 
F 0 "R?" H 5909 7296 50  0000 L CNN
F 1 "620R" H 5909 7205 50  0000 L CNN
F 2 "" H 5850 7250 50  0001 C CNN
F 3 "~" H 5850 7250 50  0001 C CNN
	1    5850 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FC8E7A
P 5850 7550
AR Path="/609BFBB9/60FC8E7A" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FC8E7A" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FC8E7A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5850 7300 50  0001 C CNN
F 1 "GND" H 5855 7377 50  0000 C CNN
F 2 "" H 5850 7550 50  0001 C CNN
F 3 "" H 5850 7550 50  0001 C CNN
	1    5850 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 7100 5850 7150
Wire Wire Line
	6200 7100 6200 7150
$Comp
L power:GND #PWR?
U 1 1 60FC8E82
P 6200 7550
AR Path="/609BFBB9/60FC8E82" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60FC8E82" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/60FC8E82" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6200 7300 50  0001 C CNN
F 1 "GND" H 6205 7377 50  0000 C CNN
F 2 "" H 6200 7550 50  0001 C CNN
F 3 "" H 6200 7550 50  0001 C CNN
	1    6200 7550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60FC8E88
P 6200 7250
AR Path="/609BFBB9/60FC8E88" Ref="R?"  Part="1" 
AR Path="/60F79100/60FC8E88" Ref="R?"  Part="1" 
F 0 "R?" H 6259 7296 50  0000 L CNN
F 1 "1K" H 6259 7205 50  0000 L CNN
F 2 "" H 6200 7250 50  0001 C CNN
F 3 "~" H 6200 7250 50  0001 C CNN
	1    6200 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60FC8E8E
P 6200 7450
AR Path="/609BFBB9/60FC8E8E" Ref="D?"  Part="1" 
AR Path="/60F79100/60FC8E8E" Ref="D?"  Part="1" 
F 0 "D?" V 6246 7380 50  0000 R CNN
F 1 "BLUE" V 6155 7380 50  0000 R CNN
F 2 "" V 6200 7450 50  0001 C CNN
F 3 "~" V 6200 7450 50  0001 C CNN
	1    6200 7450
	0    -1   -1   0   
$EndComp
Text GLabel 6200 7100 1    50   Input ~ 0
+5V_2
Wire Wire Line
	6650 7100 6650 7150
$Comp
L power:GND #PWR?
U 1 1 6124418A
P 6650 7550
AR Path="/609BFBB9/6124418A" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/6124418A" Ref="#PWR?"  Part="1" 
AR Path="/60F79100/6124418A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6650 7300 50  0001 C CNN
F 1 "GND" H 6655 7377 50  0000 C CNN
F 2 "" H 6650 7550 50  0001 C CNN
F 3 "" H 6650 7550 50  0001 C CNN
	1    6650 7550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61244190
P 6650 7250
AR Path="/609BFBB9/61244190" Ref="R?"  Part="1" 
AR Path="/60F79100/61244190" Ref="R?"  Part="1" 
F 0 "R?" H 6709 7296 50  0000 L CNN
F 1 "1K" H 6709 7205 50  0000 L CNN
F 2 "" H 6650 7250 50  0001 C CNN
F 3 "~" H 6650 7250 50  0001 C CNN
	1    6650 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 61244196
P 6650 7450
AR Path="/609BFBB9/61244196" Ref="D?"  Part="1" 
AR Path="/60F79100/61244196" Ref="D?"  Part="1" 
F 0 "D?" V 6696 7380 50  0000 R CNN
F 1 "BLUE" V 6605 7380 50  0000 R CNN
F 2 "" V 6650 7450 50  0001 C CNN
F 3 "~" V 6650 7450 50  0001 C CNN
	1    6650 7450
	0    -1   -1   0   
$EndComp
Text Label 6650 7100 1    50   ~ 0
P55
Text Notes 550  6900 0    118  ~ 0
Power Indication\n LEDs
Wire Notes Line
	500  6450 6950 6450
$EndSCHEMATC
