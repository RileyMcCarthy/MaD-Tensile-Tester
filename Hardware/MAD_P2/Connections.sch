EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
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
AR Path="/60BD9EA1/60B441E8" Ref="#PWR0149"  Part="1" 
F 0 "#PWR0149" H 7050 1050 50  0001 C CNN
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
AR Path="/60BD9EA1/60B441E2" Ref="C106"  Part="1" 
F 0 "C106" H 7142 1246 50  0000 L CNN
F 1 "10uF" H 7142 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7050 1200 50  0001 C CNN
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
AR Path="/60BD9EA1/60B441DB" Ref="#PWR0150"  Part="1" 
F 0 "#PWR0150" H 6550 1050 50  0001 C CNN
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
AR Path="/60BD9EA1/60B441D5" Ref="C105"  Part="1" 
F 0 "C105" H 6642 1246 50  0000 L CNN
F 1 "0.1uF" H 6642 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6550 1200 50  0001 C CNN
F 3 "~" H 6550 1200 50  0001 C CNN
	1    6550 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 1900 7300 1900
Wire Wire Line
	7250 1800 7300 1800
$Comp
L Device:R_Small R?
U 1 1 60B441CB
P 3700 2650
AR Path="/60B76FA1/60B441CB" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B441CB" Ref="R15"  Part="1" 
F 0 "R15" V 3600 2650 50  0000 C CNN
F 1 "27R" V 3500 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 3700 2650 50  0001 C CNN
F 3 "~" H 3700 2650 50  0001 C CNN
	1    3700 2650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B441C5
P 3700 2450
AR Path="/60B76FA1/60B441C5" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B441C5" Ref="R14"  Part="1" 
F 0 "R14" V 3504 2450 50  0000 C CNN
F 1 "27R" V 3595 2450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 3700 2450 50  0001 C CNN
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
AR Path="/60BD9EA1/60B441B5" Ref="#PWR0151"  Part="1" 
F 0 "#PWR0151" H 8150 2200 50  0001 C CNN
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
AR Path="/60BD9EA1/60B441AF" Ref="Q1"  Part="1" 
F 0 "Q1" H 8241 2246 50  0000 L CNN
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
AR Path="/60BD9EA1/60B441A9" Ref="R20"  Part="1" 
F 0 "R20" H 7641 2254 50  0000 R CNN
F 1 "10K" H 7641 2345 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7700 2300 50  0001 C CNN
F 3 "~" H 7700 2300 50  0001 C CNN
	1    7700 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 60B441A3
P 7450 2200
AR Path="/60B76FA1/60B441A3" Ref="C?"  Part="1" 
AR Path="/60BD9EA1/60B441A3" Ref="C107"  Part="1" 
F 0 "C107" V 7221 2200 50  0000 C CNN
F 1 "0.01uF" V 7312 2200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7450 2200 50  0001 C CNN
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
AR Path="/60BD9EA1/60B4419B" Ref="R19"  Part="1" 
F 0 "R19" V 6954 1900 50  0000 C CNN
F 1 "240R" V 7045 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7150 1900 50  0001 C CNN
F 3 "~" H 7150 1900 50  0001 C CNN
	1    7150 1900
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44195
P 7150 1800
AR Path="/60B76FA1/60B44195" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44195" Ref="R18"  Part="1" 
F 0 "R18" V 6954 1800 50  0000 C CNN
F 1 "240R" V 7045 1800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7150 1800 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44189" Ref="R22"  Part="1" 
F 0 "R22" V 7554 2900 50  0000 C CNN
F 1 "1K" V 7645 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7750 2900 50  0001 C CNN
F 3 "~" H 7750 2900 50  0001 C CNN
	1    7750 2900
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44183
P 7750 2800
AR Path="/60B76FA1/60B44183" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44183" Ref="R21"  Part="1" 
F 0 "R21" V 7554 2800 50  0000 C CNN
F 1 "1K" V 7645 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7750 2800 50  0001 C CNN
F 3 "~" H 7750 2800 50  0001 C CNN
	1    7750 2800
	0    1    1    0   
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60B4417D
P 7450 2900
AR Path="/60B76FA1/60B4417D" Ref="D?"  Part="1" 
AR Path="/60BD9EA1/60B4417D" Ref="D12"  Part="1" 
F 0 "D12" H 7450 2800 50  0000 C CNN
F 1 "GREEN" H 7450 2700 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" V 7450 2900 50  0001 C CNN
F 3 "~" V 7450 2900 50  0001 C CNN
	1    7450 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D?
U 1 1 60B44177
P 7250 2800
AR Path="/60B76FA1/60B44177" Ref="D?"  Part="1" 
AR Path="/60BD9EA1/60B44177" Ref="D9"  Part="1" 
F 0 "D9" H 7250 3035 50  0000 C CNN
F 1 "BLUE" H 7250 2944 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" V 7250 2800 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44166" Ref="#PWR0152"  Part="1" 
F 0 "#PWR0152" H 6150 3250 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44157" Ref="#PWR0153"  Part="1" 
F 0 "#PWR0153" H 4650 1400 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44151" Ref="C103"  Part="1" 
F 0 "C103" H 4742 1596 50  0000 L CNN
F 1 "0.01uF" H 4742 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4650 1550 50  0001 C CNN
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
Wire Wire Line
	5050 1800 5350 1800
$Comp
L power:GND #PWR?
U 1 1 60B44144
P 5050 2000
AR Path="/60B76FA1/60B44144" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44144" Ref="#PWR0154"  Part="1" 
F 0 "#PWR0154" H 5050 1750 50  0001 C CNN
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
AR Path="/60BD9EA1/60B4413E" Ref="C104"  Part="1" 
F 0 "C104" H 5142 1946 50  0000 L CNN
F 1 "0.1uF" H 5142 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5050 1900 50  0001 C CNN
F 3 "~" H 5050 1900 50  0001 C CNN
	1    5050 1900
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:FT232RL U?
U 1 1 60B44138
P 6150 2500
AR Path="/60B76FA1/60B44138" Ref="U?"  Part="1" 
AR Path="/60BD9EA1/60B44138" Ref="U27"  Part="1" 
F 0 "U27" H 6150 2750 50  0000 C CNN
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
NoConn ~ 3550 3000
NoConn ~ 3550 3100
NoConn ~ 2650 3400
$Comp
L power:GND #PWR?
U 1 1 60B44125
P 2950 3400
AR Path="/60B76FA1/60B44125" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60B44125" Ref="#PWR0155"  Part="1" 
F 0 "#PWR0155" H 2950 3150 50  0001 C CNN
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
AR Path="/60BD9EA1/60B4411D" Ref="#PWR0156"  Part="1" 
F 0 "#PWR0156" H 4200 1850 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44117" Ref="#PWR0157"  Part="1" 
F 0 "#PWR0157" H 4500 1950 50  0001 C CNN
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
AR Path="/60BD9EA1/60B4410F" Ref="R17"  Part="1" 
F 0 "R17" V 4500 2200 50  0000 C CNN
F 1 "5.1K" V 4600 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4400 2200 50  0001 C CNN
F 3 "~" H 4400 2200 50  0001 C CNN
	1    4400 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 60B44109
P 4100 2100
AR Path="/60B76FA1/60B44109" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60B44109" Ref="R16"  Part="1" 
F 0 "R16" V 3904 2100 50  0000 C CNN
F 1 "5.1K" V 3995 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4100 2100 50  0001 C CNN
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
AR Path="/60BD9EA1/60B44101" Ref="F4"  Part="1" 
F 0 "F4" H 3868 1696 50  0000 L CNN
F 1 "Polyfuse_Small" H 3868 1605 50  0000 L CNN
F 2 "Fuse:Fuse_0603_1608Metric" H 3850 1450 50  0001 L CNN
F 3 "~" H 3800 1650 50  0001 C CNN
	1    3800 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_C_Receptacle_USB2.0 J?
U 1 1 60B440FB
P 2950 2500
AR Path="/60B76FA1/60B440FB" Ref="J?"  Part="1" 
AR Path="/60BD9EA1/60B440FB" Ref="J8"  Part="1" 
F 0 "J8" H 3057 3367 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 3057 3276 50  0000 C CNN
F 2 "Connector_USB:USB_C_Receptacle_Palconn_UTC16-G" H 3100 2500 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 3100 2500 50  0001 C CNN
	1    2950 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J11
U 1 1 609B7A7D
P 4800 7250
F 0 "J11" V 4850 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 5250 6950 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 4800 7250 50  0001 C CNN
F 3 "~" H 4800 7250 50  0001 C CNN
	1    4800 7250
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J10
U 1 1 609B7159
P 4050 7250
F 0 "J10" V 4100 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 4500 6950 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 4050 7250 50  0001 C CNN
F 3 "~" H 4050 7250 50  0001 C CNN
	1    4050 7250
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J9
U 1 1 609B6905
P 3250 7250
F 0 "J9" V 3300 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 3700 6950 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 3250 7250 50  0001 C CNN
F 3 "~" H 3250 7250 50  0001 C CNN
	1    3250 7250
	0    -1   -1   0   
$EndComp
Text Notes 600  750  0    129  ~ 0
Edge Connector
Text Notes 2500 4150 0    129  ~ 0
Tensile Board Connectors
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J7
U 1 1 609AE7F7
P 2450 7250
F 0 "J7" V 2500 7300 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 2900 6950 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 2450 7250 50  0001 C CNN
F 3 "~" H 2450 7250 50  0001 C CNN
	1    2450 7250
	0    -1   -1   0   
$EndComp
Text Notes 4450 4650 2    50   ~ 0
Jumpers used for Tensile board v1 capatability
Wire Wire Line
	5100 5050 5100 5250
Wire Wire Line
	4700 5050 4700 5250
$Comp
L Jumper:SolderJumper_2_Open JP17
U 1 1 60991820
P 5100 4900
F 0 "JP17" V 5054 4968 50  0000 L CNN
F 1 "J_P30" V 5145 4968 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 5100 4900 50  0001 C CNN
F 3 "~" H 5100 4900 50  0001 C CNN
	1    5100 4900
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP15
U 1 1 60990D91
P 4700 4900
F 0 "JP15" V 4654 4812 50  0000 R CNN
F 1 "J_P31" V 4745 4812 50  0000 R CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 4700 4900 50  0001 C CNN
F 3 "~" H 4700 4900 50  0001 C CNN
	1    4700 4900
	0    -1   1    0   
$EndComp
Wire Wire Line
	4700 5850 4700 5750
Wire Wire Line
	4200 5850 4700 5850
Wire Wire Line
	4200 5950 4200 5850
Wire Wire Line
	4800 5950 4800 5750
Wire Wire Line
	4550 5950 4800 5950
Wire Wire Line
	4900 5950 4900 5750
$Comp
L Jumper:SolderJumper_2_Open JP14
U 1 1 6098D6A7
P 4550 6100
F 0 "JP14" V 4504 6168 50  0000 L CNN
F 1 "J_P32" V 4595 6168 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 4550 6100 50  0001 C CNN
F 3 "~" H 4550 6100 50  0001 C CNN
	1    4550 6100
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP16
U 1 1 6098BE34
P 4900 6100
F 0 "JP16" V 4854 6168 50  0000 L CNN
F 1 "J_P33" V 4945 6168 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 4900 6100 50  0001 C CNN
F 3 "~" H 4900 6100 50  0001 C CNN
	1    4900 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 5250 5000 5250
Connection ~ 4900 5250
Wire Wire Line
	4800 5250 4900 5250
Connection ~ 5100 5750
Wire Wire Line
	5000 5750 5100 5750
$Comp
L power:GND #PWR0158
U 1 1 609867C6
P 5100 5750
F 0 "#PWR0158" H 5100 5500 50  0001 C CNN
F 1 "GND" H 5105 5577 50  0000 C CNN
F 2 "" H 5100 5750 50  0001 C CNN
F 3 "" H 5100 5750 50  0001 C CNN
	1    5100 5750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0159
U 1 1 60985CC6
P 4900 5250
F 0 "#PWR0159" H 4900 5000 50  0001 C CNN
F 1 "GND" H 4905 5077 50  0000 C CNN
F 2 "" H 4900 5250 50  0001 C CNN
F 3 "" H 4900 5250 50  0001 C CNN
	1    4900 5250
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J14
U 1 1 609848F0
P 4900 5550
F 0 "J14" V 4950 5600 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 5250 5150 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 4900 5550 50  0001 C CNN
F 3 "~" H 4900 5550 50  0001 C CNN
	1    4900 5550
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J13
U 1 1 609801A5
P 4050 5400
F 0 "J13" V 4100 5450 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 4500 5100 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 4050 5400 50  0001 C CNN
F 3 "~" H 4050 5400 50  0001 C CNN
	1    4050 5400
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J22
U 1 1 6097E7FB
P 3400 5400
F 0 "J22" V 3450 5450 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 3850 5100 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 3400 5400 50  0001 C CNN
F 3 "~" H 3400 5400 50  0001 C CNN
	1    3400 5400
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J21
U 1 1 6097AD9A
P 2750 5400
F 0 "J21" V 2800 5450 50  0000 R CNN
F 1 "Conn_02x05_Odd_Even" H 3200 5100 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 2750 5400 50  0001 C CNN
F 3 "~" H 2750 5400 50  0001 C CNN
	1    2750 5400
	0    -1   -1   0   
$EndComp
Text GLabel 1300 4600 0    50   Input ~ 0
RESET
$Comp
L power:GND #PWR0160
U 1 1 6096D345
P 1300 4700
F 0 "#PWR0160" H 1300 4450 50  0001 C CNN
F 1 "GND" V 1305 4572 50  0000 R CNN
F 2 "" H 1300 4700 50  0001 C CNN
F 3 "" H 1300 4700 50  0001 C CNN
	1    1300 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0161
U 1 1 6096CEA0
P 1300 4800
F 0 "#PWR0161" H 1300 4550 50  0001 C CNN
F 1 "GND" V 1305 4672 50  0000 R CNN
F 2 "" H 1300 4800 50  0001 C CNN
F 3 "" H 1300 4800 50  0001 C CNN
	1    1300 4800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0162
U 1 1 6096BF33
P 1300 4900
F 0 "#PWR0162" H 1300 4650 50  0001 C CNN
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
NoConn ~ 1800 1300
NoConn ~ 1800 1200
$Comp
L Connector_Generic:Conn_02x40_Counter_Clockwise J6
U 1 1 60BDCE17
P 1600 3100
F 0 "J6" H 1650 5217 50  0000 C CNN
F 1 "Edge_Conn" H 1650 5126 50  0000 C CNN
F 2 "MAD_P2:P2_Edge_Conn" H 1600 3100 50  0001 C CNN
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
$Comp
L power:GND #PWR0217
U 1 1 60DCCC5E
P 5050 7700
F 0 "#PWR0217" H 5050 7450 50  0001 C CNN
F 1 "GND" H 5055 7527 50  0000 C CNN
F 2 "" H 5050 7700 50  0001 C CNN
F 3 "" H 5050 7700 50  0001 C CNN
	1    5050 7700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0218
U 1 1 60DD69AB
P 5050 6700
F 0 "#PWR0218" H 5050 6450 50  0001 C CNN
F 1 "GND" H 5055 6527 50  0000 C CNN
F 2 "" H 5050 6700 50  0001 C CNN
F 3 "" H 5050 6700 50  0001 C CNN
	1    5050 6700
	0    -1   -1   0   
$EndComp
Text GLabel 4800 6950 1    50   Input ~ 0
VIN_MAIN
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J16
U 1 1 60DE4CD2
P 9800 1450
AR Path="/60BD9EA1/60DE4CD2" Ref="J16"  Part="1" 
AR Path="/60C11A52/60DE4CD2" Ref="J?"  Part="1" 
F 0 "J16" V 9850 1500 50  0000 R CNN
F 1 "Conn_DAC" V 9805 1162 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 9800 1450 50  0001 C CNN
F 3 "~" H 9800 1450 50  0001 C CNN
	1    9800 1450
	0    -1   -1   0   
$EndComp
Text GLabel 9600 1650 3    50   Input ~ 0
3.3V_ADC
$Comp
L power:GND #PWR?
U 1 1 60DE4CD9
P 10000 1650
AR Path="/609BFBB9/60DE4CD9" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60DE4CD9" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60DE4CD9" Ref="#PWR0219"  Part="1" 
F 0 "#PWR0219" H 10000 1400 50  0001 C CNN
F 1 "GND" H 10005 1477 50  0000 C CNN
F 2 "" H 10000 1650 50  0001 C CNN
F 3 "" H 10000 1650 50  0001 C CNN
	1    10000 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 1650 10000 1650
Connection ~ 10000 1650
$Comp
L power:GND #PWR?
U 1 1 60DE4CE5
P 9800 1150
AR Path="/609BFBB9/60DE4CE5" Ref="#PWR?"  Part="1" 
AR Path="/60C11A52/60DE4CE5" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60DE4CE5" Ref="#PWR0220"  Part="1" 
F 0 "#PWR0220" H 9800 900 50  0001 C CNN
F 1 "GND" H 9805 977 50  0000 C CNN
F 2 "" H 9800 1150 50  0001 C CNN
F 3 "" H 9800 1150 50  0001 C CNN
	1    9800 1150
	-1   0    0    1   
$EndComp
Wire Wire Line
	9700 1150 9800 1150
Connection ~ 9800 1150
Wire Wire Line
	9800 1150 9900 1150
Text Notes 8850 750  0    129  ~ 0
DAC
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J15
U 1 1 60E06871
P 9950 3100
AR Path="/60BD9EA1/60E06871" Ref="J15"  Part="1" 
AR Path="/60C11A52/60E06871" Ref="J?"  Part="1" 
AR Path="/609BFBB9/60E06871" Ref="J?"  Part="1" 
F 0 "J15" V 10000 3150 50  0000 R CNN
F 1 "Conn_ADC1" V 9955 2812 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical" H 9950 3100 50  0001 C CNN
F 3 "~" H 9950 3100 50  0001 C CNN
	1    9950 3100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60E0688D
P 10150 3450
AR Path="/609BFBB9/60E0688D" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/60E0688D" Ref="#PWR0221"  Part="1" 
F 0 "#PWR0221" H 10150 3200 50  0001 C CNN
F 1 "GND" H 10155 3277 50  0000 C CNN
F 2 "" H 10150 3450 50  0001 C CNN
F 3 "" H 10150 3450 50  0001 C CNN
	1    10150 3450
	1    0    0    -1  
$EndComp
Text GLabel 10150 2800 1    50   Input ~ 0
3.3V_ADC
Text Notes 8850 2500 0    129  ~ 0
ADC
Wire Notes Line
	8750 2200 11200 2200
Text GLabel 9750 3300 3    50   Input ~ 0
A1+
Text GLabel 9750 2800 1    50   Input ~ 0
A1-
Text GLabel 9850 3300 3    50   Input ~ 0
A2+
Text GLabel 9950 3300 3    50   Input ~ 0
A3+
Text GLabel 10050 3300 3    50   Input ~ 0
A4+
Text GLabel 9850 2800 1    50   Input ~ 0
A2-
Text GLabel 9950 2800 1    50   Input ~ 0
A3-
Text GLabel 10050 2800 1    50   Input ~ 0
A4-
$Comp
L Connector_Generic:Conn_01x08 J20
U 1 1 60AAE656
P 9450 4200
F 0 "J20" H 9530 4192 50  0000 L CNN
F 1 "P2" H 9530 4101 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 9450 4200 50  0001 C CNN
F 3 "~" H 9450 4200 50  0001 C CNN
	1    9450 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J18
U 1 1 60AAEBA7
P 9100 5100
F 0 "J18" H 9180 5092 50  0000 L CNN
F 1 "P3" H 9180 5001 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 9100 5100 50  0001 C CNN
F 3 "~" H 9100 5100 50  0001 C CNN
	1    9100 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J19
U 1 1 60AB01A9
P 9100 6000
F 0 "J19" H 9180 5992 50  0000 L CNN
F 1 "P4" H 9180 5901 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 9100 6000 50  0001 C CNN
F 3 "~" H 9100 6000 50  0001 C CNN
	1    9100 6000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J26
U 1 1 60AB0F6B
P 10400 6050
F 0 "J26" H 10500 5950 50  0000 C CNN
F 1 "P7" H 10500 6050 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 10400 6050 50  0001 C CNN
F 3 "~" H 10400 6050 50  0001 C CNN
	1    10400 6050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J24
U 1 1 60AB2A80
P 10400 4250
F 0 "J24" H 10550 4000 50  0000 C CNN
F 1 "P5" H 10550 4150 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x10_P2.54mm_Vertical" H 10400 4250 50  0001 C CNN
F 3 "~" H 10400 4250 50  0001 C CNN
	1    10400 4250
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J23
U 1 1 60AB4D8A
P 9700 5800
F 0 "J23" H 9750 6117 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 9750 6026 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x03_P2.54mm_Vertical" H 9700 5800 50  0001 C CNN
F 3 "~" H 9700 5800 50  0001 C CNN
	1    9700 5800
	-1   0    0    -1  
$EndComp
Wire Notes Line
	2350 3750 11200 3750
NoConn ~ 9250 3900
$Comp
L power:GND #PWR0236
U 1 1 60ACF8EA
P 9100 4450
F 0 "#PWR0236" H 9100 4200 50  0001 C CNN
F 1 "GND" H 9105 4277 50  0000 C CNN
F 2 "" H 9100 4450 50  0001 C CNN
F 3 "" H 9100 4450 50  0001 C CNN
	1    9100 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	9250 4500 9150 4500
Wire Wire Line
	9150 4500 9150 4450
Wire Wire Line
	9150 4450 9100 4450
Wire Wire Line
	9250 4400 9150 4400
Wire Wire Line
	9150 4400 9150 4450
Connection ~ 9150 4450
Text GLabel 9250 4600 0    50   Input ~ 0
VIN_MAIN
NoConn ~ 8900 4800
NoConn ~ 8900 4900
NoConn ~ 8900 5000
NoConn ~ 8900 5100
NoConn ~ 8900 5200
NoConn ~ 8900 5300
NoConn ~ 8900 5400
NoConn ~ 8900 5500
NoConn ~ 8900 5700
NoConn ~ 8900 5800
NoConn ~ 8900 5900
NoConn ~ 8900 6000
NoConn ~ 8900 6100
NoConn ~ 8900 6200
NoConn ~ 8900 6300
NoConn ~ 8900 6400
NoConn ~ 10600 5750
NoConn ~ 10600 5850
NoConn ~ 10600 5950
NoConn ~ 10600 6050
NoConn ~ 10600 6150
NoConn ~ 10600 6250
NoConn ~ 10600 5600
NoConn ~ 10600 5500
NoConn ~ 10600 5400
NoConn ~ 10600 5300
NoConn ~ 10600 5200
NoConn ~ 10600 5100
NoConn ~ 10600 5000
$Comp
L Device:R_Small R?
U 1 1 60AF985C
P 10900 4750
AR Path="/60B76FA1/60AF985C" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/60AF985C" Ref="R1"  Part="1" 
F 0 "R1" V 10704 4750 50  0000 C CNN
F 1 "10K" V 10795 4750 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 10900 4750 50  0001 C CNN
F 3 "~" H 10900 4750 50  0001 C CNN
	1    10900 4750
	0    1    -1   0   
$EndComp
Wire Wire Line
	11000 4750 11100 4750
$Comp
L Connector_Generic:Conn_01x08 J25
U 1 1 60AB1BA6
P 10400 5200
F 0 "J25" H 10550 5050 50  0000 C CNN
F 1 "P6" H 10550 5200 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 10400 5200 50  0001 C CNN
F 3 "~" H 10400 5200 50  0001 C CNN
	1    10400 5200
	-1   0    0    -1  
$EndComp
NoConn ~ 10600 3850
NoConn ~ 10600 3950
NoConn ~ 10600 4050
NoConn ~ 10600 4250
NoConn ~ 10600 4350
NoConn ~ 10600 4450
Wire Wire Line
	10600 4750 10800 4750
NoConn ~ 10600 4900
$Comp
L power:GND #PWR0237
U 1 1 60B1AC64
P 10600 4150
F 0 "#PWR0237" H 10600 3900 50  0001 C CNN
F 1 "GND" H 10605 3977 50  0000 C CNN
F 2 "" H 10600 4150 50  0001 C CNN
F 3 "" H 10600 4150 50  0001 C CNN
	1    10600 4150
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0238
U 1 1 60B28B51
P 9900 5900
F 0 "#PWR0238" H 9900 5650 50  0001 C CNN
F 1 "GND" H 9905 5727 50  0000 C CNN
F 2 "" H 9900 5900 50  0001 C CNN
F 3 "" H 9900 5900 50  0001 C CNN
	1    9900 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 7700 5000 7700
Wire Wire Line
	5000 7700 5000 7450
Wire Wire Line
	5050 6700 5000 6700
Wire Wire Line
	5000 6700 5000 6950
Wire Wire Line
	10150 3450 10150 3300
Text GLabel 4900 6950 1    50   Input ~ 0
5V_MAIN1
Text GLabel 4900 7450 3    50   Input ~ 0
3.3V_MAIN1
Text GLabel 9250 4300 0    50   Input ~ 0
5V_MAIN1
Text GLabel 11100 4750 1    50   Input ~ 0
5V_MAIN1
Text GLabel 9250 3950 0    50   Input ~ 0
3.3V_MAIN1
Text GLabel 9250 4200 0    50   Input ~ 0
3.3V_MAIN1
Wire Wire Line
	9250 3950 9250 4000
Text GLabel 9900 5650 2    50   Input ~ 0
5V_MAIN1
Wire Wire Line
	9900 5650 9900 5700
$Comp
L Connector:Micro_SD_Card J?
U 1 1 612055B5
P 7800 4550
AR Path="/609BFBB9/612055B5" Ref="J?"  Part="1" 
AR Path="/60BD9EA1/612055B5" Ref="J31"  Part="1" 
F 0 "J31" H 8100 4600 50  0000 C CNN
F 1 "Micro_SD_Card_Connector" H 8100 4400 50  0000 C CNN
F 2 "Tensile_Board:WM26501" H 8950 4850 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 7800 4550 50  0001 C CNN
	1    7800 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 612055BB
P 6900 4550
AR Path="/609BFBB9/612055BB" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/612055BB" Ref="#PWR0194"  Part="1" 
F 0 "#PWR0194" H 6900 4400 50  0001 C CNN
F 1 "+3.3V" V 6915 4678 50  0000 L CNN
F 2 "" H 6900 4550 50  0001 C CNN
F 3 "" H 6900 4550 50  0001 C CNN
	1    6900 4550
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 612055C1
P 6900 4750
AR Path="/609BFBB9/612055C1" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/612055C1" Ref="#PWR0195"  Part="1" 
F 0 "#PWR0195" H 6900 4500 50  0001 C CNN
F 1 "GND" V 6905 4622 50  0000 R CNN
F 2 "" H 6900 4750 50  0001 C CNN
F 3 "" H 6900 4750 50  0001 C CNN
	1    6900 4750
	0    1    1    0   
$EndComp
NoConn ~ 8600 5150
NoConn ~ 6900 4950
NoConn ~ 6900 4250
$Comp
L Device:R_Small R?
U 1 1 612055CA
P 6400 4850
AR Path="/609BFBB9/612055CA" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/612055CA" Ref="R35"  Part="1" 
F 0 "R35" V 6500 4850 50  0000 C CNN
F 1 "240R" V 6300 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6400 4850 50  0001 C CNN
F 3 "~" H 6400 4850 50  0001 C CNN
	1    6400 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6200 4850 6300 4850
Wire Wire Line
	6500 4850 6900 4850
Wire Wire Line
	6200 4650 6900 4650
Wire Wire Line
	6200 4450 6900 4450
Wire Wire Line
	6200 4350 6900 4350
Text Notes 5550 4000 0    129  ~ 0
Data SD Card
$Comp
L Connector:Micro_SD_Card J?
U 1 1 6120B790
P 7800 5800
AR Path="/609BFBB9/6120B790" Ref="J?"  Part="1" 
AR Path="/60BD9EA1/6120B790" Ref="J32"  Part="1" 
F 0 "J32" H 8100 5850 50  0000 C CNN
F 1 "Micro_SD_Card_Socket" H 8100 5650 50  0000 C CNN
F 2 "Connector_Card:microSD_HC_Hirose_DM3D-SF" H 8950 6100 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/693072010801.pdf" H 7800 5800 50  0001 C CNN
	1    7800 5800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6120B796
P 6900 5800
AR Path="/609BFBB9/6120B796" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/6120B796" Ref="#PWR0196"  Part="1" 
F 0 "#PWR0196" H 6900 5650 50  0001 C CNN
F 1 "+3.3V" V 6915 5928 50  0000 L CNN
F 2 "" H 6900 5800 50  0001 C CNN
F 3 "" H 6900 5800 50  0001 C CNN
	1    6900 5800
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6120B79C
P 6900 6000
AR Path="/609BFBB9/6120B79C" Ref="#PWR?"  Part="1" 
AR Path="/60BD9EA1/6120B79C" Ref="#PWR0197"  Part="1" 
F 0 "#PWR0197" H 6900 5750 50  0001 C CNN
F 1 "GND" V 6905 5872 50  0000 R CNN
F 2 "" H 6900 6000 50  0001 C CNN
F 3 "" H 6900 6000 50  0001 C CNN
	1    6900 6000
	0    1    1    0   
$EndComp
NoConn ~ 8600 6400
NoConn ~ 6900 6200
NoConn ~ 6900 5500
$Comp
L Device:R_Small R?
U 1 1 6120B7A5
P 6400 6100
AR Path="/609BFBB9/6120B7A5" Ref="R?"  Part="1" 
AR Path="/60BD9EA1/6120B7A5" Ref="R36"  Part="1" 
F 0 "R36" V 6500 6100 50  0000 C CNN
F 1 "240R" V 6300 6100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6400 6100 50  0001 C CNN
F 3 "~" H 6400 6100 50  0001 C CNN
	1    6400 6100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6200 6100 6300 6100
Wire Wire Line
	6500 6100 6900 6100
Wire Wire Line
	6200 5900 6900 5900
Wire Wire Line
	6200 5700 6900 5700
Wire Wire Line
	6200 5600 6900 5600
Wire Notes Line
	5500 3750 5500 6550
Text GLabel 1800 5100 2    50   Input ~ 0
P0
Text GLabel 1800 5000 2    50   Input ~ 0
P1
Text GLabel 1800 4900 2    50   Input ~ 0
P2
Text GLabel 1800 4800 2    50   Input ~ 0
P3
Text GLabel 1800 4700 2    50   Input ~ 0
P4
Text GLabel 1800 4600 2    50   Input ~ 0
P5
Text GLabel 1800 4500 2    50   Input ~ 0
P6
Text GLabel 1800 4400 2    50   Input ~ 0
P7
Text GLabel 1800 4300 2    50   Input ~ 0
P8
Text GLabel 1800 4200 2    50   Input ~ 0
P9
Text GLabel 1800 4100 2    50   Input ~ 0
P10
Text GLabel 1800 4000 2    50   Input ~ 0
P11
Text GLabel 1800 3900 2    50   Input ~ 0
P12
Text GLabel 1800 3800 2    50   Input ~ 0
P13
Text GLabel 1800 3700 2    50   Input ~ 0
P14
Text GLabel 1800 3600 2    50   Input ~ 0
P15
Text GLabel 1800 3500 2    50   Input ~ 0
P16
Text GLabel 1800 3400 2    50   Input ~ 0
P17
Text GLabel 1800 3300 2    50   Input ~ 0
P18
Text GLabel 1800 3200 2    50   Input ~ 0
P19
Text GLabel 1800 3100 2    50   Input ~ 0
P20
Text GLabel 1800 3000 2    50   Input ~ 0
P21
Text GLabel 1800 2900 2    50   Input ~ 0
P22
Text GLabel 1800 2800 2    50   Input ~ 0
P23
Text GLabel 1800 2700 2    50   Input ~ 0
P24
Text GLabel 1800 2600 2    50   Input ~ 0
P25
Text GLabel 1800 2500 2    50   Input ~ 0
P26
Text GLabel 1800 2400 2    50   Input ~ 0
P27
Text GLabel 1800 2300 2    50   Input ~ 0
P28
Text GLabel 1800 2200 2    50   Input ~ 0
P29
Text GLabel 1800 2100 2    50   Input ~ 0
P30
Text GLabel 1800 2000 2    50   Input ~ 0
P31
Text GLabel 1800 1900 2    50   Input ~ 0
P32
Text GLabel 1800 1800 2    50   Input ~ 0
P33
Text GLabel 1800 1700 2    50   Input ~ 0
P34
Text GLabel 1800 1600 2    50   Input ~ 0
P35
Text GLabel 1800 1500 2    50   Input ~ 0
P36
Text GLabel 1800 1400 2    50   Input ~ 0
P37
Text GLabel 1300 1200 0    50   Input ~ 0
P38
Text GLabel 1300 1300 0    50   Input ~ 0
P39
Text GLabel 1300 1600 0    50   Input ~ 0
P40
Text GLabel 1300 1700 0    50   Input ~ 0
P41
Text GLabel 1300 1800 0    50   Input ~ 0
P42
Text GLabel 1300 1900 0    50   Input ~ 0
P43
Text GLabel 1300 2000 0    50   Input ~ 0
P44
Text GLabel 1300 2100 0    50   Input ~ 0
P45
Text GLabel 1300 2200 0    50   Input ~ 0
P46
Text GLabel 1300 2300 0    50   Input ~ 0
P47
Text GLabel 1300 2600 0    50   Input ~ 0
P48
Text GLabel 1300 2700 0    50   Input ~ 0
P49
Text GLabel 1300 2800 0    50   Input ~ 0
P50
Text GLabel 1300 2900 0    50   Input ~ 0
P51
Text GLabel 1300 3000 0    50   Input ~ 0
P52
Text GLabel 1300 3100 0    50   Input ~ 0
P53
Text GLabel 1300 3200 0    50   Input ~ 0
P54
Text GLabel 1300 3300 0    50   Input ~ 0
P55
Text GLabel 1300 3600 0    50   Input ~ 0
P56
Text GLabel 1300 3700 0    50   Input ~ 0
P57
Text GLabel 1300 3800 0    50   Input ~ 0
P58
Text GLabel 1300 3900 0    50   Input ~ 0
P59
Text GLabel 1300 4000 0    50   Input ~ 0
P60
Text GLabel 1300 4100 0    50   Input ~ 0
P61
Text GLabel 1300 4200 0    50   Input ~ 0
P62
Text GLabel 1300 4300 0    50   Input ~ 0
P63
Text GLabel 1300 4500 0    50   Input ~ 0
V_0_3
Text GLabel 1300 4400 0    50   Input ~ 0
V_8_11
Text GLabel 1300 2400 0    50   Input ~ 0
V_24_27
Text GLabel 1300 1400 0    50   Input ~ 0
V_32_35
Text GLabel 1300 2500 0    50   Input ~ 0
V_48_51
Text GLabel 1300 3500 0    50   Input ~ 0
V_56_59
Text GLabel 1300 3400 0    50   Input ~ 0
V_16_19
Text GLabel 1300 1500 0    50   Input ~ 0
V_40_43
Text GLabel 2550 5600 3    50   Input ~ 0
P0
Text GLabel 2550 5100 1    50   Input ~ 0
P1
Text GLabel 2650 5600 3    50   Input ~ 0
P2
Text GLabel 2650 5100 1    50   Input ~ 0
P3
Text GLabel 2750 5600 3    50   Input ~ 0
P4
Text GLabel 2750 5100 1    50   Input ~ 0
P5
Text GLabel 2850 5600 3    50   Input ~ 0
P6
Text GLabel 2850 5100 1    50   Input ~ 0
P7
Text GLabel 2950 5600 3    50   Input ~ 0
P8
Text GLabel 2950 5100 1    50   Input ~ 0
P9
Text GLabel 3200 5600 3    50   Input ~ 0
P10
Text GLabel 3200 5100 1    50   Input ~ 0
P11
Text GLabel 3300 5600 3    50   Input ~ 0
P12
Text GLabel 3300 5100 1    50   Input ~ 0
P13
Text GLabel 3400 5600 3    50   Input ~ 0
P14
Text GLabel 3400 5100 1    50   Input ~ 0
P15
Text GLabel 3500 5600 3    50   Input ~ 0
P16
Text GLabel 3500 5100 1    50   Input ~ 0
P17
Text GLabel 3600 5600 3    50   Input ~ 0
P18
Text GLabel 3600 5100 1    50   Input ~ 0
P19
Text GLabel 3850 5600 3    50   Input ~ 0
P20
Text GLabel 3850 5100 1    50   Input ~ 0
P21
Text GLabel 3950 5600 3    50   Input ~ 0
P22
Text GLabel 3950 5100 1    50   Input ~ 0
P23
Text GLabel 4050 5600 3    50   Input ~ 0
P24
Text GLabel 4050 5100 1    50   Input ~ 0
P25
Text GLabel 4150 5600 3    50   Input ~ 0
P26
Text GLabel 4150 5100 1    50   Input ~ 0
P27
Text GLabel 4250 5600 3    50   Input ~ 0
P28
Text GLabel 4250 5100 1    50   Input ~ 0
P29
Text GLabel 4200 6250 3    50   Input ~ 0
P30
Text GLabel 4700 4750 1    50   Input ~ 0
P31
Text GLabel 4550 6250 3    50   Input ~ 0
P32
Text GLabel 4900 6250 3    50   Input ~ 0
P33
Text GLabel 2250 7450 3    50   Input ~ 0
P34
Text GLabel 2250 6950 1    50   Input ~ 0
P35
Text GLabel 2350 7450 3    50   Input ~ 0
P36
Text GLabel 2350 6950 1    50   Input ~ 0
P37
Text GLabel 2450 7450 3    50   Input ~ 0
P38
Text GLabel 2450 6950 1    50   Input ~ 0
P39
Text GLabel 2550 7450 3    50   Input ~ 0
P40
Text GLabel 2550 6950 1    50   Input ~ 0
P41
Text GLabel 2650 7450 3    50   Input ~ 0
P42
Text GLabel 2650 6950 1    50   Input ~ 0
P43
Text GLabel 3050 7450 3    50   Input ~ 0
P44
Text GLabel 3050 6950 1    50   Input ~ 0
P45
Text GLabel 3150 7450 3    50   Input ~ 0
P46
Text GLabel 3150 6950 1    50   Input ~ 0
P47
Text GLabel 3250 7450 3    50   Input ~ 0
P48
Text GLabel 3250 6950 1    50   Input ~ 0
P49
Text GLabel 3350 7450 3    50   Input ~ 0
P50
Text GLabel 3350 6950 1    50   Input ~ 0
P51
Text GLabel 3450 7450 3    50   Input ~ 0
P52
Text GLabel 3450 6950 1    50   Input ~ 0
P53
Text GLabel 3850 7450 3    50   Input ~ 0
P54
Text GLabel 3850 6950 1    50   Input ~ 0
P55
Text GLabel 3950 7450 3    50   Input ~ 0
P56
Text GLabel 3950 6950 1    50   Input ~ 0
P57
Text GLabel 4050 7450 3    50   Input ~ 0
P58
Text GLabel 4050 6950 1    50   Input ~ 0
P59
Text GLabel 4150 7450 3    50   Input ~ 0
P60
Text GLabel 4150 6950 1    50   Input ~ 0
P61
Text GLabel 4250 7450 3    50   Input ~ 0
P62
Text GLabel 4250 6950 1    50   Input ~ 0
P63
Text GLabel 4600 6950 1    50   Input ~ 0
V_0_3
Text GLabel 2950 8850 3    50   Input ~ 0
V_4_7
Text GLabel 3050 8850 3    50   Input ~ 0
V_8_11
Text GLabel 3150 8850 3    50   Input ~ 0
V_12_15
Text GLabel 3350 8850 3    50   Input ~ 0
V_20_23
Text GLabel 4700 7450 3    50   Input ~ 0
V_16_19
Text GLabel 3450 8850 3    50   Input ~ 0
V_24_27
Text GLabel 3550 8850 3    50   Input ~ 0
V_28_31
Text GLabel 4700 6950 1    50   Input ~ 0
V_32_35
Text GLabel 3750 8850 3    50   Input ~ 0
V_36_39
Text GLabel 3850 8850 3    50   Input ~ 0
V_40_43
Text GLabel 3950 8850 3    50   Input ~ 0
V_44_47
Text GLabel 4800 7450 3    50   Input ~ 0
V_48_51
Text GLabel 4150 8850 3    50   Input ~ 0
V_52_55
Text GLabel 4250 8850 3    50   Input ~ 0
V_56_59
Text GLabel 4350 8850 3    50   Input ~ 0
V_60_63
NoConn ~ 4600 7450
Text GLabel 5100 4750 1    50   Input ~ 0
P34
$Comp
L Jumper:SolderJumper_2_Open JP13
U 1 1 6098F74E
P 4200 6100
F 0 "JP13" V 4154 6168 50  0000 L CNN
F 1 "J_P30" V 4245 6168 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 4200 6100 50  0001 C CNN
F 3 "~" H 4200 6100 50  0001 C CNN
	1    4200 6100
	0    1    1    0   
$EndComp
Text Label 3800 2450 0    50   ~ 0
D-
Text Label 3800 2650 0    50   ~ 0
D+
Text Label 5350 2100 2    50   ~ 0
D+
Text Label 5350 2200 2    50   ~ 0
D-
Text GLabel 7300 1800 2    50   Input ~ 0
P63
Text GLabel 7300 1900 2    50   Input ~ 0
P62
Text GLabel 8150 1950 1    50   Input ~ 0
RESET
Text GLabel 9600 1150 1    50   Input ~ 0
AO1
Text GLabel 10000 1150 1    50   Input ~ 0
AO3
Text GLabel 9800 1650 3    50   Input ~ 0
AO2
Text GLabel 9250 4100 0    50   Input ~ 0
RESET
Text GLabel 10600 4550 2    50   Input ~ 0
P55
Text GLabel 10600 4650 2    50   Input ~ 0
P54
Text GLabel 10600 6350 2    50   Input ~ 0
P57
Text GLabel 10600 6450 2    50   Input ~ 0
P56
Text GLabel 9900 5800 2    50   Input ~ 0
P52
Text GLabel 9400 5900 3    50   Input ~ 0
RESET
Text GLabel 9400 5800 0    50   Input ~ 0
P51
Text GLabel 9400 5700 0    50   Input ~ 0
P53
Text GLabel 6200 4350 0    50   Input ~ 0
P42
Text GLabel 6200 4450 0    50   Input ~ 0
P41
Text GLabel 6200 4650 0    50   Input ~ 0
P40
Text GLabel 6200 4850 0    50   Input ~ 0
P39
Text GLabel 6200 5600 0    50   Input ~ 0
P42
Text GLabel 6200 5700 0    50   Input ~ 0
P41
Text GLabel 6200 5900 0    50   Input ~ 0
P40
Text GLabel 6200 6100 0    50   Input ~ 0
P39
Text GLabel 9700 1650 3    50   Input ~ 0
5V_ADC
$EndSCHEMATC
