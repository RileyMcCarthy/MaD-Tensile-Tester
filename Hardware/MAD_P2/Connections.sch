EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2600 750  0    129  ~ 0
USB Serial
Text Notes 550  6850 0    129  ~ 0
IO Breakout
Text GLabel 7050 1100 1    50   Input ~ 0
VBUS
$Comp
L power:GND #PWR?
U 1 1 60B441E8
P 7050 1300
AR Path="/60B76FA1/60B441E8" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B441E8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7050 1050 50  0001 C CNN
F 1 "GND" H 7055 1127 50  0000 C CNN
F 2 "" H 7050 1300 50  0001 C CNN
F 3 "" H 7050 1300 50  0001 C CNN
	1    7050 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B441E2
P 7050 1200
AR Path="/60B76FA1/60B441E2" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B441E2" Ref="C?"  Part="1" 
F 0 "C?" H 7142 1246 50  0000 L CNN
F 1 "10uF" H 7142 1155 50  0000 L CNN
F 2 "" H 7050 1200 50  0001 C CNN
F 3 "~" H 7050 1200 50  0001 C CNN
	1    7050 1200
	1    0    0    -1  
$EndComp
Text GLabel 6550 1100 1    50   Input ~ 0
VBUS
$Comp
L power:GND #PWR?
U 1 1 60B441DB
P 6550 1300
AR Path="/60B76FA1/60B441DB" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B441DB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6550 1050 50  0001 C CNN
F 1 "GND" H 6555 1127 50  0000 C CNN
F 2 "" H 6550 1300 50  0001 C CNN
F 3 "" H 6550 1300 50  0001 C CNN
	1    6550 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B441D5
P 6550 1200
AR Path="/60B76FA1/60B441D5" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B441D5" Ref="C?"  Part="1" 
F 0 "C?" H 6642 1246 50  0000 L CNN
F 1 "0.1uF" H 6642 1155 50  0000 L CNN
F 2 "" H 6550 1200 50  0001 C CNN
F 3 "~" H 6550 1200 50  0001 C CNN
	1    6550 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 1900 7300 1900
Wire Wire Line
	7250 1800 7300 1800
Text Label 7300 1900 0    50   ~ 0
P62
Text Label 7300 1800 0    50   ~ 0
P63
$Comp
L Device:R_Small R?
U 1 1 60B441CB
P 3700 2650
AR Path="/60B76FA1/60B441CB" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B441CB" Ref="R?"  Part="1" 
F 0 "R?" V 3600 2650 50  0000 C CNN
F 1 "27R" V 3500 2650 50  0000 C CNN
F 2 "" H 3700 2650 50  0001 C CNN
F 3 "~" H 3700 2650 50  0001 C CNN
	1    3700 2650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B441C5
P 3700 2450
AR Path="/60B76FA1/60B441C5" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B441C5" Ref="R?"  Part="1" 
F 0 "R?" V 3504 2450 50  0000 C CNN
F 1 "27R" V 3595 2450 50  0000 C CNN
F 2 "" H 3700 2450 50  0001 C CNN
F 3 "~" H 3700 2450 50  0001 C CNN
	1    3700 2450
	0    1    1    0   
$EndComp
Connection ~ 8150 2450
Wire Wire Line
	8150 2450 8150 2400
Wire Wire Line
	7700 2450 8150 2450
Wire Wire Line
	7700 2400 7700 2450
Wire Wire Line
	8150 1950 8150 2000
Text Label 8150 1950 0    50   ~ 0
RESET
Wire Wire Line
	7350 2200 6950 2200
Connection ~ 7700 2200
Wire Wire Line
	7550 2200 7700 2200
Wire Wire Line
	7700 2200 7850 2200
$Comp
L power:GND #PWR?
U 1 1 60B441B5
P 8150 2450
AR Path="/60B76FA1/60B441B5" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B441B5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8150 2200 50  0001 C CNN
F 1 "GND" H 8155 2277 50  0000 C CNN
F 2 "" H 8150 2450 50  0001 C CNN
F 3 "" H 8150 2450 50  0001 C CNN
	1    8150 2450
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q?
U 1 1 60B441AF
P 8050 2200
AR Path="/60B76FA1/60B441AF" Ref="Q?"  Part="1" 
AR Path="/60BD9EA1/60B441AF" Ref="Q?"  Part="1" 
F 0 "Q?" H 8241 2246 50  0000 L CNN
F 1 "MMBT3904" H 8241 2155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8250 2125 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 8050 2200 50  0001 L CNN
	1    8050 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B441A9
P 7700 2300
AR Path="/60B76FA1/60B441A9" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B441A9" Ref="R?"  Part="1" 
F 0 "R?" H 7641 2254 50  0000 R CNN
F 1 "10K" H 7641 2345 50  0000 R CNN
F 2 "" H 7700 2300 50  0001 C CNN
F 3 "~" H 7700 2300 50  0001 C CNN
	1    7700 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B441A3
P 7450 2200
AR Path="/60B76FA1/60B441A3" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B441A3" Ref="C?"  Part="1" 
F 0 "C?" V 7221 2200 50  0000 C CNN
F 1 "0.01uF" V 7312 2200 50  0000 C CNN
F 2 "" H 7450 2200 50  0001 C CNN
F 3 "~" H 7450 2200 50  0001 C CNN
	1    7450 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 1900 6950 1900
Wire Wire Line
	7050 1800 6950 1800
$Comp
L Device:R_Small R?
U 1 1 60B4419B
P 7150 1900
AR Path="/60B76FA1/60B4419B" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B4419B" Ref="R?"  Part="1" 
F 0 "R?" V 6954 1900 50  0000 C CNN
F 1 "1K" V 7045 1900 50  0000 C CNN
F 2 "" H 7150 1900 50  0001 C CNN
F 3 "~" H 7150 1900 50  0001 C CNN
	1    7150 1900
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44195
P 7150 1800
AR Path="/60B76FA1/60B44195" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44195" Ref="R?"  Part="1" 
F 0 "R?" V 6954 1800 50  0000 C CNN
F 1 "1K" V 7045 1800 50  0000 C CNN
F 2 "" H 7150 1800 50  0001 C CNN
F 3 "~" H 7150 1800 50  0001 C CNN
	1    7150 1800
	0    -1   1    0   
$EndComp
Wire Wire Line
	7650 2800 7350 2800
Wire Wire Line
	7650 2900 7550 2900
Text GLabel 7850 2800 2    50   Input ~ 0
VBUS
Text GLabel 7850 2900 2    50   Input ~ 0
VBUS
Wire Wire Line
	7350 2900 6950 2900
Wire Wire Line
	7150 2800 6950 2800
$Comp
L Device:R_Small R?
U 1 1 60B44189
P 7750 2900
AR Path="/60B76FA1/60B44189" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44189" Ref="R?"  Part="1" 
F 0 "R?" V 7554 2900 50  0000 C CNN
F 1 "1K" V 7645 2900 50  0000 C CNN
F 2 "" H 7750 2900 50  0001 C CNN
F 3 "~" H 7750 2900 50  0001 C CNN
	1    7750 2900
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44183
P 7750 2800
AR Path="/60B76FA1/60B44183" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44183" Ref="R?"  Part="1" 
F 0 "R?" V 7554 2800 50  0000 C CNN
F 1 "1K" V 7645 2800 50  0000 C CNN
F 2 "" H 7750 2800 50  0001 C CNN
F 3 "~" H 7750 2800 50  0001 C CNN
	1    7750 2800
	0    1    1    0   
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60B4417D
P 7450 2900
AR Path="/60B76FA1/60B4417D" Ref="D?"  Part="1" 
AR Path="/60BD9EA1/60B4417D" Ref="D?"  Part="1" 
F 0 "D?" H 7450 2800 50  0000 C CNN
F 1 "GREEN" H 7450 2700 50  0000 C CNN
F 2 "" V 7450 2900 50  0001 C CNN
F 3 "~" V 7450 2900 50  0001 C CNN
	1    7450 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60B44177
P 7250 2800
AR Path="/60B76FA1/60B44177" Ref="D?"  Part="1" 
AR Path="/60BD9EA1/60B44177" Ref="D?"  Part="1" 
F 0 "D?" H 7250 3035 50  0000 C CNN
F 1 "BLUE" H 7250 2944 50  0000 C CNN
F 2 "" V 7250 2800 50  0001 C CNN
F 3 "~" V 7250 2800 50  0001 C CNN
	1    7250 2800
	1    0    0    -1  
$EndComp
NoConn ~ 5350 2500
NoConn ~ 5350 2700
NoConn ~ 5350 2900
Connection ~ 5950 3500
Wire Wire Line
	5350 3500 5950 3500
Wire Wire Line
	5350 3200 5350 3500
Wire Wire Line
	5950 3500 6150 3500
Wire Wire Line
	6250 3500 6350 3500
Connection ~ 6250 3500
Wire Wire Line
	6150 3500 6250 3500
Connection ~ 6150 3500
$Comp
L power:GND #PWR?
U 1 1 60B44166
P 6150 3500
AR Path="/60B76FA1/60B44166" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44166" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6150 3250 50  0001 C CNN
F 1 "GND" H 6155 3327 50  0000 C CNN
F 2 "" H 6150 3500 50  0001 C CNN
F 3 "" H 6150 3500 50  0001 C CNN
	1    6150 3500
	1    0    0    -1  
$EndComp
NoConn ~ 6950 3200
NoConn ~ 6950 3100
NoConn ~ 6950 3000
NoConn ~ 6950 2500
NoConn ~ 6950 2400
NoConn ~ 6950 2300
NoConn ~ 6950 2100
NoConn ~ 6950 2000
Text GLabel 4650 1450 1    50   Input ~ 0
VBUS
$Comp
L power:GND #PWR?
U 1 1 60B44157
P 4650 1650
AR Path="/60B76FA1/60B44157" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44157" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4650 1400 50  0001 C CNN
F 1 "GND" H 4655 1477 50  0000 C CNN
F 2 "" H 4650 1650 50  0001 C CNN
F 3 "" H 4650 1650 50  0001 C CNN
	1    4650 1650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B44151
P 4650 1550
AR Path="/60B76FA1/60B44151" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B44151" Ref="C?"  Part="1" 
F 0 "C?" H 4742 1596 50  0000 L CNN
F 1 "0.01uF" H 4742 1505 50  0000 L CNN
F 2 "" H 4650 1550 50  0001 C CNN
F 3 "~" H 4650 1550 50  0001 C CNN
	1    4650 1550
	1    0    0    -1  
$EndComp
Text GLabel 6250 1500 1    50   Input ~ 0
VBUS
Connection ~ 5350 1800
Wire Wire Line
	5350 1500 6050 1500
Wire Wire Line
	5350 1800 5350 1500
Text GLabel 5350 2100 0    50   Input ~ 0
D+
Text GLabel 5350 2200 0    50   Input ~ 0
D-
Wire Wire Line
	5050 1800 5350 1800
$Comp
L power:GND #PWR?
U 1 1 60B44144
P 5050 2000
AR Path="/60B76FA1/60B44144" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44144" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5050 1750 50  0001 C CNN
F 1 "GND" H 5055 1827 50  0000 C CNN
F 2 "" H 5050 2000 50  0001 C CNN
F 3 "" H 5050 2000 50  0001 C CNN
	1    5050 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B4413E
P 5050 1900
AR Path="/60B76FA1/60B4413E" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B4413E" Ref="C?"  Part="1" 
F 0 "C?" H 5142 1946 50  0000 L CNN
F 1 "0.1uF" H 5142 1855 50  0000 L CNN
F 2 "" H 5050 1900 50  0001 C CNN
F 3 "~" H 5050 1900 50  0001 C CNN
	1    5050 1900
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:FT232RL U?
U 1 1 60B44138
P 6150 2500
AR Path="/60B76FA1/60B44138" Ref="U?"  Part="1" 
AR Path="/60BD9EA1/60B44138" Ref="U?"  Part="1" 
F 0 "U?" H 6150 2750 50  0000 C CNN
F 1 "FT232RL" H 6150 2500 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 7250 1600 50  0001 C CNN
F 3 "https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232R.pdf" H 6150 2500 50  0001 C CNN
	1    6150 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1450 3800 1550
Text GLabel 3800 1450 1    50   Input ~ 0
VBUS
Text GLabel 3800 2450 2    50   Input ~ 0
D-
Wire Wire Line
	3550 2650 3550 2600
Connection ~ 3550 2650
Wire Wire Line
	3550 2650 3600 2650
Wire Wire Line
	3550 2450 3550 2400
Connection ~ 3550 2450
Wire Wire Line
	3550 2450 3600 2450
Text GLabel 3800 2650 2    50   Input ~ 0
D+
NoConn ~ 3550 3000
NoConn ~ 3550 3100
NoConn ~ 2650 3400
$Comp
L power:GND #PWR?
U 1 1 60B44125
P 2950 3400
AR Path="/60B76FA1/60B44125" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44125" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2950 3150 50  0001 C CNN
F 1 "GND" H 2955 3227 50  0000 C CNN
F 2 "" H 2950 3400 50  0001 C CNN
F 3 "" H 2950 3400 50  0001 C CNN
	1    2950 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2700 3550 2650
Wire Wire Line
	3550 2500 3550 2450
$Comp
L power:GND #PWR?
U 1 1 60B4411D
P 4200 2100
AR Path="/60B76FA1/60B4411D" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B4411D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4200 1850 50  0001 C CNN
F 1 "GND" V 4205 1972 50  0000 R CNN
F 2 "" H 4200 2100 50  0001 C CNN
F 3 "" H 4200 2100 50  0001 C CNN
	1    4200 2100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60B44117
P 4500 2200
AR Path="/60B76FA1/60B44117" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44117" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4500 1950 50  0001 C CNN
F 1 "GND" V 4505 2072 50  0000 R CNN
F 2 "" H 4500 2200 50  0001 C CNN
F 3 "" H 4500 2200 50  0001 C CNN
	1    4500 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3550 2100 4000 2100
Wire Wire Line
	3550 2200 4300 2200
$Comp
L Device:R_Small R?
U 1 1 60B4410F
P 4400 2200
AR Path="/60B76FA1/60B4410F" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B4410F" Ref="R?"  Part="1" 
F 0 "R?" V 4500 2200 50  0000 C CNN
F 1 "5.1K" V 4600 2200 50  0000 C CNN
F 2 "" H 4400 2200 50  0001 C CNN
F 3 "~" H 4400 2200 50  0001 C CNN
	1    4400 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44109
P 4100 2100
AR Path="/60B76FA1/60B44109" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44109" Ref="R?"  Part="1" 
F 0 "R?" V 3904 2100 50  0000 C CNN
F 1 "5.1K" V 3995 2100 50  0000 C CNN
F 2 "" H 4100 2100 50  0001 C CNN
F 3 "~" H 4100 2100 50  0001 C CNN
	1    4100 2100
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 1900 3800 1750
Wire Wire Line
	3550 1900 3800 1900
$Comp
L Device:Polyfuse_Small F?
U 1 1 60B44101
P 3800 1650
AR Path="/60B76FA1/60B44101" Ref="F?"  Part="1" 
AR Path="/60BD9EA1/60B44101" Ref="F?"  Part="1" 
F 0 "F?" H 3868 1696 50  0000 L CNN
F 1 "Polyfuse_Small" H 3868 1605 50  0000 L CNN
F 2 "" H 3850 1450 50  0001 L CNN
F 3 "~" H 3800 1650 50  0001 C CNN
	1    3800 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_C_Receptacle_USB2.0 J?
U 1 1 60B440FB
P 2950 2500
AR Path="/60B76FA1/60B440FB" Ref="J?"  Part="1" 
AR Path="/60BD9EA1/60B440FB" Ref="J?"  Part="1" 
F 0 "J?" H 3057 3367 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 3057 3276 50  0000 C CNN
F 2 "" H 3100 2500 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 3100 2500 50  0001 C CNN
	1    2950 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 609B7A7D
P 4800 7250
F 0 "J?" V 4850 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 5250 6950 50  0000 R CNN
F 2 "" H 4800 7250 50  0001 C CNN
F 3 "~" H 4800 7250 50  0001 C CNN
	1    4800 7250
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 609B7159
P 4050 7250
F 0 "J?" V 4100 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 4500 6950 50  0000 R CNN
F 2 "" H 4050 7250 50  0001 C CNN
F 3 "~" H 4050 7250 50  0001 C CNN
	1    4050 7250
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 609B6905
P 3250 7250
F 0 "J?" V 3300 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 3700 6950 50  0000 R CNN
F 2 "" H 3250 7250 50  0001 C CNN
F 3 "~" H 3250 7250 50  0001 C CNN
	1    3250 7250
	0    -1   -1   0   
$EndComp
Text Notes 600  750  0    129  ~ 0
Edge Connector
Text Notes 2500 4150 0    129  ~ 0
Tensile Board Connectors
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 609AE7F7
P 2450 7250
F 0 "J?" V 2500 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 2900 6950 50  0000 R CNN
F 2 "" H 2450 7250 50  0001 C CNN
F 3 "~" H 2450 7250 50  0001 C CNN
	1    2450 7250
	0    -1   -1   0   
$EndComp
Text Label 5100 5050 1    50   ~ 0
P29
Text Label 5000 5050 1    50   ~ 0
P27
Text Label 4900 5050 1    50   ~ 0
P25
Text Label 4800 5050 1    50   ~ 0
P23
Text Label 4700 5050 1    50   ~ 0
P21
Text Label 4450 5050 1    50   ~ 0
P19
Text Label 4350 5050 1    50   ~ 0
P17
Text Label 4250 5050 1    50   ~ 0
P15
Text Label 4150 5050 1    50   ~ 0
P13
Text Label 4050 5050 1    50   ~ 0
P11
Text Label 5100 5550 3    50   ~ 0
P28
Text Label 5000 5550 3    50   ~ 0
P26
Text Label 4900 5550 3    50   ~ 0
P24
Text Label 4800 5550 3    50   ~ 0
P22
Text Label 4700 5550 3    50   ~ 0
P20
Text Label 4450 5550 3    50   ~ 0
P18
Text Label 4350 5550 3    50   ~ 0
P16
Text Label 4250 5550 3    50   ~ 0
P14
Text Label 4150 5550 3    50   ~ 0
P12
Text Label 4050 5550 3    50   ~ 0
P10
Text Label 3800 5050 1    50   ~ 0
P9
Text Label 3700 5050 1    50   ~ 0
P7
Text Label 3600 5050 1    50   ~ 0
P5
Text Label 3800 5550 3    50   ~ 0
P8
Text Label 3700 5550 3    50   ~ 0
P6
Text Label 3600 5550 3    50   ~ 0
P4
Text Label 3500 5050 1    50   ~ 0
P3
Text Label 3500 5550 3    50   ~ 0
P2
Text Label 3400 5050 1    50   ~ 0
P1
Text Label 3400 5550 3    50   ~ 0
P0
Text Notes 8200 5050 2    50   ~ 0
Jumpers used for Tensile board v1 capatability
Wire Wire Line
	6200 4850 6200 5050
Wire Wire Line
	5800 4850 5800 5050
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 60991820
P 6200 4700
F 0 "JP?" V 6154 4768 50  0000 L CNN
F 1 "J_P30" V 6245 4768 50  0000 L CNN
F 2 "" H 6200 4700 50  0001 C CNN
F 3 "~" H 6200 4700 50  0001 C CNN
	1    6200 4700
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 60990D91
P 5800 4700
F 0 "JP?" V 5754 4612 50  0000 R CNN
F 1 "J_P31" V 5845 4612 50  0000 R CNN
F 2 "" H 5800 4700 50  0001 C CNN
F 3 "~" H 5800 4700 50  0001 C CNN
	1    5800 4700
	0    -1   1    0   
$EndComp
Wire Wire Line
	5800 5650 5800 5550
Wire Wire Line
	5300 5650 5800 5650
Wire Wire Line
	5300 5750 5300 5650
Wire Wire Line
	5900 5750 5900 5550
Wire Wire Line
	5650 5750 5900 5750
Wire Wire Line
	6000 5750 6000 5550
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 6098F74E
P 5300 5900
F 0 "JP?" V 5254 5968 50  0000 L CNN
F 1 "J_P30" V 5345 5968 50  0000 L CNN
F 2 "" H 5300 5900 50  0001 C CNN
F 3 "~" H 5300 5900 50  0001 C CNN
	1    5300 5900
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 6098D6A7
P 5650 5900
F 0 "JP?" V 5604 5968 50  0000 L CNN
F 1 "J_P32" V 5695 5968 50  0000 L CNN
F 2 "" H 5650 5900 50  0001 C CNN
F 3 "~" H 5650 5900 50  0001 C CNN
	1    5650 5900
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 6098BE34
P 6000 5900
F 0 "JP?" V 5954 5968 50  0000 L CNN
F 1 "J_P33" V 6045 5968 50  0000 L CNN
F 2 "" H 6000 5900 50  0001 C CNN
F 3 "~" H 6000 5900 50  0001 C CNN
	1    6000 5900
	0    1    1    0   
$EndComp
Text Label 6200 4550 1    50   ~ 0
P34
Text Label 6000 6050 3    50   ~ 0
P33
Text Label 5650 6050 3    50   ~ 0
P32
Text Label 5800 4550 1    50   ~ 0
P31
Text Label 5300 6050 3    50   ~ 0
P30
Wire Wire Line
	6000 5050 6100 5050
Connection ~ 6000 5050
Wire Wire Line
	5900 5050 6000 5050
Connection ~ 6200 5550
Wire Wire Line
	6100 5550 6200 5550
$Comp
L power:GND #PWR?
U 1 1 609867C6
P 6200 5550
F 0 "#PWR?" H 6200 5300 50  0001 C CNN
F 1 "GND" H 6205 5377 50  0000 C CNN
F 2 "" H 6200 5550 50  0001 C CNN
F 3 "" H 6200 5550 50  0001 C CNN
	1    6200 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60985CC6
P 6000 5050
F 0 "#PWR?" H 6000 4800 50  0001 C CNN
F 1 "GND" H 6005 4877 50  0000 C CNN
F 2 "" H 6000 5050 50  0001 C CNN
F 3 "" H 6000 5050 50  0001 C CNN
	1    6000 5050
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J4
U 1 1 609848F0
P 6000 5350
F 0 "J4" V 6050 5400 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" V 6005 5062 50  0000 R CNN
F 2 "" H 6000 5350 50  0001 C CNN
F 3 "~" H 6000 5350 50  0001 C CNN
	1    6000 5350
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J3
U 1 1 609801A5
P 4900 5350
F 0 "J3" V 4950 5400 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 5350 5050 50  0000 R CNN
F 2 "" H 4900 5350 50  0001 C CNN
F 3 "~" H 4900 5350 50  0001 C CNN
	1    4900 5350
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J2
U 1 1 6097E7FB
P 4250 5350
F 0 "J2" V 4300 5400 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 4700 5050 50  0000 R CNN
F 2 "" H 4250 5350 50  0001 C CNN
F 3 "~" H 4250 5350 50  0001 C CNN
	1    4250 5350
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J1
U 1 1 6097AD9A
P 3600 5350
F 0 "J1" V 3650 5400 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 4050 5050 50  0000 R CNN
F 2 "" H 3600 5350 50  0001 C CNN
F 3 "~" H 3600 5350 50  0001 C CNN
	1    3600 5350
	0    -1   -1   0   
$EndComp
Text Label 1300 1200 2    50   ~ 0
P38
Text Label 1300 1300 2    50   ~ 0
P39
Text Label 1300 1600 2    50   ~ 0
P40
Text Label 1300 1700 2    50   ~ 0
P41
Text Label 1300 1800 2    50   ~ 0
P42
Text Label 1300 1900 2    50   ~ 0
P43
Text Label 1300 2000 2    50   ~ 0
P44
Text Label 1300 2100 2    50   ~ 0
P45
Text Label 1300 2200 2    50   ~ 0
P46
Text Label 1300 2300 2    50   ~ 0
P47
Text Label 1300 2600 2    50   ~ 0
P48
Text Label 1300 2700 2    50   ~ 0
P49
Text Label 1300 2800 2    50   ~ 0
P50
Text Label 1300 2900 2    50   ~ 0
P51
Text Label 1300 3000 2    50   ~ 0
P52
Text Label 1300 3100 2    50   ~ 0
P53
Text Label 1300 3200 2    50   ~ 0
P54
Text Label 1300 3300 2    50   ~ 0
P55
Text Label 1300 3400 2    50   ~ 0
V_16_19
Text Label 1300 3500 2    50   ~ 0
V_56_59
Text Label 1300 3600 2    50   ~ 0
P56
Text Label 1300 3700 2    50   ~ 0
P57
Text Label 1300 3800 2    50   ~ 0
P58
Text Label 1300 3900 2    50   ~ 0
P59
Text Label 1300 4000 2    50   ~ 0
P60
Text Label 1300 4100 2    50   ~ 0
P61
Text Label 1300 4200 2    50   ~ 0
P62
Text Label 1300 4300 2    50   ~ 0
P63
Text Label 1300 2500 2    50   ~ 0
V_48_51
Text Label 1300 1500 2    50   ~ 0
V_40_43
Text Label 1300 1400 2    50   ~ 0
V_32_35
Text Label 1300 2400 2    50   ~ 0
V_24_27
Text Label 1300 4400 2    50   ~ 0
V_8_11
Text Label 1300 4500 2    50   ~ 0
V_0_3
Text GLabel 1300 4600 0    50   Input ~ 0
RESET
$Comp
L power:GND #PWR?
U 1 1 6096D345
P 1300 4700
F 0 "#PWR?" H 1300 4450 50  0001 C CNN
F 1 "GND" V 1305 4572 50  0000 R CNN
F 2 "" H 1300 4700 50  0001 C CNN
F 3 "" H 1300 4700 50  0001 C CNN
	1    1300 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6096CEA0
P 1300 4800
F 0 "#PWR?" H 1300 4550 50  0001 C CNN
F 1 "GND" V 1305 4672 50  0000 R CNN
F 2 "" H 1300 4800 50  0001 C CNN
F 3 "" H 1300 4800 50  0001 C CNN
	1    1300 4800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6096BF33
P 1300 4900
F 0 "#PWR?" H 1300 4650 50  0001 C CNN
F 1 "GND" V 1305 4772 50  0000 R CNN
F 2 "" H 1300 4900 50  0001 C CNN
F 3 "" H 1300 4900 50  0001 C CNN
	1    1300 4900
	0    1    1    0   
$EndComp
Text GLabel 1300 5000 0    50   Input ~ 0
VIN_MAIN
Text GLabel 1300 5100 0    50   Input ~ 0
VIN_MAIN
Text Label 1800 1400 0    50   ~ 0
P37
Text Label 1800 1500 0    50   ~ 0
P36
Text Label 1800 1600 0    50   ~ 0
P35
Text Label 1800 1700 0    50   ~ 0
P34
Text Label 1800 1800 0    50   ~ 0
P33
Text Label 1800 1900 0    50   ~ 0
P32
Text Label 1800 2000 0    50   ~ 0
P31
Text Label 1800 2100 0    50   ~ 0
P30
Text Label 1800 2200 0    50   ~ 0
P29
Text Label 1800 2300 0    50   ~ 0
P28
Text Label 1800 2400 0    50   ~ 0
P27
Text Label 1800 2500 0    50   ~ 0
P26
Text Label 1800 2600 0    50   ~ 0
P25
Text Label 1800 2700 0    50   ~ 0
P24
Text Label 1800 2800 0    50   ~ 0
P23
Text Label 1800 2900 0    50   ~ 0
P22
Text Label 1800 3000 0    50   ~ 0
P21
Text Label 1800 3100 0    50   ~ 0
P20
Text Label 1800 3200 0    50   ~ 0
P19
Text Label 1800 3300 0    50   ~ 0
P18
Text Label 1800 3400 0    50   ~ 0
P17
Text Label 1800 3500 0    50   ~ 0
P16
Text Label 1800 3600 0    50   ~ 0
P15
Text Label 1800 3700 0    50   ~ 0
P14
Text Label 1800 3800 0    50   ~ 0
P13
Text Label 1800 3900 0    50   ~ 0
P12
Text Label 1800 4000 0    50   ~ 0
P11
Text Label 1800 4100 0    50   ~ 0
P10
Text Label 1800 4200 0    50   ~ 0
P9
Text Label 1800 4300 0    50   ~ 0
P8
Text Label 1800 4400 0    50   ~ 0
P7
Text Label 1800 4500 0    50   ~ 0
P6
Text Label 1800 4600 0    50   ~ 0
P5
Text Label 1800 4700 0    50   ~ 0
P4
Text Label 1800 4800 0    50   ~ 0
P3
Text Label 1800 4900 0    50   ~ 0
P2
Text Label 1800 5000 0    50   ~ 0
P1
Text Label 1800 5100 0    50   ~ 0
P0
NoConn ~ 1800 1300
NoConn ~ 1800 1200
$Comp
L Connector_Generic:Conn_02x40_Counter_Clockwise J?
U 1 1 60BDCE17
P 1600 3100
F 0 "J?" H 1650 5217 50  0000 C CNN
F 1 "Edge_Conn" H 1650 5126 50  0000 C CNN
F 2 "" H 1600 3100 50  0001 C CNN
F 3 "~" H 1600 3100 50  0001 C CNN
	1    1600 3100
	-1   0    0    -1  
$EndComp
Wire Notes Line
	6950 6550 500  6550
Wire Notes Line
	2350 500  2350 6550
Wire Notes Line
	8750 500  8750 6500
Wire Notes Line
	2350 3800 8750 3800
$EndSCHEMATC
