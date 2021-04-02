EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SD Card Breakout"
Date "2021-02-23"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x09_Female J2
U 1 1 60219D11
P 3750 3450
F 0 "J2" H 3850 3400 50  0000 C CNN
F 1 "Conn_01x09_Female" H 4150 3300 50  0000 C CNN
F 2 "SD_Card:WM26501" H 3750 3450 50  0001 C CNN
F 3 "~" H 3750 3450 50  0001 C CNN
	1    3750 3450
	-1   0    0    -1  
$EndComp
$Comp
L Connector:SD_Card J1
U 1 1 60228D6E
P 6750 3800
F 0 "J1" H 6750 4465 50  0000 C CNN
F 1 "SD_Card" H 6750 4374 50  0000 C CNN
F 2 "SD_Card:609-3956-2-ND" H 6750 3800 50  0001 C CNN
F 3 "http://portal.fciconnect.com/Comergent//fci/drawing/10067847.pdf" H 6750 3800 50  0001 C CNN
	1    6750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3850 4750 3850
Text Label 4450 3850 0    50   ~ 0
SHIELD
Wire Wire Line
	7650 3900 7650 4000
Wire Wire Line
	7650 4000 8200 4000
Connection ~ 7650 4000
Text Label 7900 4000 0    50   ~ 0
SHIELD
NoConn ~ 7650 3600
NoConn ~ 7650 3700
Wire Wire Line
	5850 3400 5500 3400
Wire Wire Line
	5850 3500 5500 3500
Wire Wire Line
	5850 3600 5500 3600
Wire Wire Line
	5850 3700 5500 3700
Wire Wire Line
	5850 3800 5500 3800
Wire Wire Line
	5850 3900 5500 3900
Wire Wire Line
	5850 4000 5500 4000
Wire Wire Line
	5850 4100 5500 4100
Wire Wire Line
	5850 4200 5500 4200
Text Label 5550 3400 0    50   ~ 0
DAT2
Text Label 5550 3500 0    50   ~ 0
DAT3CD
Text Label 5550 3600 0    50   ~ 0
CMD
Text Label 5550 3700 0    50   ~ 0
VSS
Text Label 5550 3800 0    50   ~ 0
VDD
Text Label 5550 3900 0    50   ~ 0
CLK
Text Label 5550 4000 0    50   ~ 0
VSS
Text Label 5550 4100 0    50   ~ 0
DAT0
Text Label 5550 4200 0    50   ~ 0
DAT1
$Comp
L Connector:Conn_01x09_Female J3
U 1 1 60231CB1
P 3750 4600
F 0 "J3" H 3900 4700 50  0000 L CNN
F 1 "Conn_01x09_Female" H 3850 4600 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 3750 4600 50  0001 C CNN
F 3 "~" H 3750 4600 50  0001 C CNN
	1    3750 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3950 5000 4750 5000
Text Label 4450 5000 0    50   ~ 0
SHIELD
Wire Wire Line
	3950 4800 4750 4800
Wire Wire Line
	4750 4700 3950 4700
Wire Wire Line
	3950 4600 4750 4600
Wire Wire Line
	4750 4500 3950 4500
Wire Wire Line
	3950 4400 4750 4400
Wire Wire Line
	4750 4300 3950 4300
Wire Wire Line
	3950 4200 4750 4200
Text Label 4450 4200 0    50   ~ 0
DAT2
Text Label 4450 4300 0    50   ~ 0
DAT3CD
Text Label 4450 4400 0    50   ~ 0
CMD
Text Label 4450 4500 0    50   ~ 0
VDD
Text Label 4450 4600 0    50   ~ 0
CLK
Text Label 4450 4700 0    50   ~ 0
VSS
Text Label 4450 4800 0    50   ~ 0
DAT0
Text Label 4450 4900 0    50   ~ 0
DAT1
Wire Wire Line
	4750 4900 3950 4900
Text Label 4450 3750 0    50   ~ 0
DAT1
Text Label 4450 3650 0    50   ~ 0
DAT0
Text Label 4450 3550 0    50   ~ 0
VSS
Text Label 4450 3450 0    50   ~ 0
CLK
Text Label 4450 3350 0    50   ~ 0
VDD
Text Label 4450 3250 0    50   ~ 0
CMD
Text Label 4450 3150 0    50   ~ 0
DAT3CD
Text Label 4450 3050 0    50   ~ 0
DAT2
Wire Wire Line
	3950 3050 4750 3050
Wire Wire Line
	4750 3150 3950 3150
Wire Wire Line
	3950 3250 4750 3250
Wire Wire Line
	4750 3350 3950 3350
Wire Wire Line
	3950 3450 4750 3450
Wire Wire Line
	4750 3550 3950 3550
Wire Wire Line
	3950 3650 4750 3650
Wire Wire Line
	4750 3750 3950 3750
Text Notes 3800 2950 0    79   ~ 0
2.54mm header
Text Notes 3650 4100 0    79   ~ 0
8 pin molex connector
$EndSCHEMATC
