EESchema Schematic File Version 4
LIBS:sdbox-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 3750 5200 0    50   BiDi ~ 0
D0
$Comp
L Connector:DB25_Male_MountingHoles J1
U 1 1 5EA7B824
P 4050 4200
F 0 "J1" H 4230 4202 50  0000 L CNN
F 1 "DB25_Male_MountingHoles" H 3600 2700 50  0000 L CNN
F 2 "Connector_Dsub:DSUB-25_Male_Horizontal_P2.77x2.84mm_EdgePinOffset4.94mm_Housed_MountingHolesOffset7.48mm" H 4050 4200 50  0001 C CNN
F 3 " ~" H 4050 4200 50  0001 C CNN
	1    4050 4200
	1    0    0    -1  
$EndComp
Text GLabel 3750 5000 0    50   BiDi ~ 0
D1
Text GLabel 3750 4800 0    50   BiDi ~ 0
D2
Text GLabel 3750 4600 0    50   BiDi ~ 0
D3
Text GLabel 3750 4400 0    50   BiDi ~ 0
D4
Text GLabel 3750 4200 0    50   BiDi ~ 0
D5
Text GLabel 3750 4000 0    50   BiDi ~ 0
D6
Text GLabel 3750 3800 0    50   BiDi ~ 0
D7
Text GLabel 3750 3400 0    50   BiDi ~ 0
BUSY
Text GLabel 3750 3200 0    50   BiDi ~ 0
POUT
Text GLabel 3750 3000 0    50   BiDi ~ 0
SELECT
Text GLabel 3750 4700 0    50   Input ~ 0
GND
Text GLabel 3750 4500 0    50   Input ~ 0
GND
Text GLabel 3750 4300 0    50   Input ~ 0
GND
Text GLabel 3750 4100 0    50   Input ~ 0
GND
Text GLabel 3750 3900 0    50   Input ~ 0
GND
Text GLabel 3750 3700 0    50   Input ~ 0
GND
Text GLabel 3750 4900 0    50   Output ~ 0
RESET
Text GLabel 4800 3750 0    50   Input ~ 0
GND
Text GLabel 6150 3550 2    50   Input ~ 0
GND
Text GLabel 4800 4050 0    50   BiDi ~ 0
BUSY
Text GLabel 4800 4150 0    50   BiDi ~ 0
POUT
Text GLabel 4800 4250 0    50   BiDi ~ 0
D6
Text GLabel 4800 4350 0    50   BiDi ~ 0
D7
Text GLabel 6150 4550 2    50   BiDi ~ 0
D0
Text GLabel 6150 4450 2    50   BiDi ~ 0
D1
Text GLabel 6150 4350 2    50   BiDi ~ 0
D2
Text GLabel 6150 4250 2    50   BiDi ~ 0
D3
Text GLabel 6150 4150 2    50   BiDi ~ 0
D4
Text GLabel 6150 4050 2    50   BiDi ~ 0
D5
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 5EB238A1
P 6600 3650
F 0 "JP1" H 6600 3550 50  0000 C CNN
F 1 "SolderJumper_Open" H 6600 3450 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 6600 3650 50  0001 C CNN
F 3 "~" H 6600 3650 50  0001 C CNN
	1    6600 3650
	1    0    0    -1  
$EndComp
Text GLabel 6750 3650 2    50   Input ~ 0
RESET
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5EB341E8
P 3800 2500
F 0 "#FLG0101" H 3800 2575 50  0001 C CNN
F 1 "PWR_FLAG" H 3800 2673 50  0000 C CNN
F 2 "" H 3800 2500 50  0001 C CNN
F 3 "~" H 3800 2500 50  0001 C CNN
	1    3800 2500
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0101
U 1 1 5EB34559
P 3800 2400
F 0 "#PWR0101" H 3800 2250 50  0001 C CNN
F 1 "+5V" H 3815 2573 50  0000 C CNN
F 2 "" H 3800 2400 50  0001 C CNN
F 3 "" H 3800 2400 50  0001 C CNN
	1    3800 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2400 3800 2500
$Comp
L power:GND #PWR0102
U 1 1 5EB362B9
P 4100 2500
F 0 "#PWR0102" H 4100 2250 50  0001 C CNN
F 1 "GND" H 4105 2327 50  0000 C CNN
F 2 "" H 4100 2500 50  0001 C CNN
F 3 "" H 4100 2500 50  0001 C CNN
	1    4100 2500
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5EB368E6
P 4100 2400
F 0 "#FLG0102" H 4100 2475 50  0001 C CNN
F 1 "PWR_FLAG" H 4100 2573 50  0000 C CNN
F 2 "" H 4100 2400 50  0001 C CNN
F 3 "~" H 4100 2400 50  0001 C CNN
	1    4100 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2400 4100 2500
Text GLabel 4100 2450 0    50   Input ~ 0
GND
$Comp
L Connector:Micro_SD_Card_Module U2
U 1 1 5EB40C39
P 7050 4900
F 0 "U2" H 7000 4950 50  0000 L CNN
F 1 "Micro_SD_Card_Module" H 6650 4250 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 8200 5200 50  0001 C CNN
F 3 "" V 7000 4950 50  0001 C CNN
	1    7050 4900
	1    0    0    -1  
$EndComp
Text GLabel 7300 4250 1    50   Input ~ 0
GND
Text GLabel 6150 3750 2    50   Output ~ 0
5V
Text GLabel 7200 4250 1    50   Input ~ 0
5V
Text GLabel 7100 4250 1    50   Output ~ 0
MISO
Text GLabel 7000 4250 1    50   Input ~ 0
MOSI
Text GLabel 6700 4000 0    50   Input ~ 0
SELECT
Text GLabel 6950 2700 0    50   Input ~ 0
GND
Wire Wire Line
	6250 3450 6150 3450
Text GLabel 6650 2500 2    50   Input ~ 0
VCC_7-12V
$Comp
L Device:C_Small C1
U 1 1 5EB53531
P 6350 3350
F 0 "C1" V 6213 3350 50  0000 C CNN
F 1 "C_Small" V 6212 3350 50  0001 C CNN
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 6350 3350 50  0001 C CNN
F 3 "~" H 6350 3350 50  0001 C CNN
	1    6350 3350
	0    1    1    0   
$EndComp
Text GLabel 6450 3350 2    50   Input ~ 0
GND
Text GLabel 6900 4250 1    50   Input ~ 0
SCK
$Comp
L MCU_Microchip_AVR:Arduino_Nano_V3 U1
U 1 1 5EB5B4AA
P 5450 4100
F 0 "U1" H 5450 4250 50  0000 C CNN
F 1 "Arduino_Nano_V3" H 5950 3050 50  0000 C CNN
F 2 "Arduino:Arduino_Nano_V3_Schield" H 5450 4100 50  0001 C CNN
F 3 "" H 5450 4100 50  0001 C CNN
	1    5450 4100
	1    0    0    -1  
$EndComp
NoConn ~ 6150 3850
NoConn ~ 6150 3950
NoConn ~ 6150 4650
NoConn ~ 3750 3300
NoConn ~ 3750 3500
NoConn ~ 3750 5100
NoConn ~ 4800 3450
NoConn ~ 4800 3550
NoConn ~ 4800 3650
NoConn ~ 4050 5600
Text GLabel 6150 4850 2    50   Output ~ 0
SCK
Text GLabel 4800 4850 0    50   Input ~ 0
MISO
Text GLabel 4800 4750 0    50   Output ~ 0
MOSI
$Comp
L Relay_SolidState:PVG612S U3
U 1 1 5EB77D01
P 6050 2650
F 0 "U3" H 6050 2450 50  0000 C CNN
F 1 "PVG612S" H 6050 2950 50  0000 C CNN
F 2 "Relay_SMD:SOIC254P978X443-6N" H 6120 3225 50  0001 C CNN
F 3 "" H 6120 3225 50  0001 C CNN
	1    6050 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2650 6950 2750
Wire Wire Line
	6250 3350 6250 3450
Wire Wire Line
	6250 3100 6250 3350
Connection ~ 6250 3350
Wire Wire Line
	6950 2850 6750 2850
Wire Wire Line
	6650 2850 6650 2500
Wire Wire Line
	6650 2500 6350 2500
Text GLabel 3750 5300 0    50   Output ~ 0
Amiga5V
Text GLabel 5750 2650 0    50   Input ~ 0
GND
NoConn ~ 6350 2800
NoConn ~ 5750 2800
Wire Wire Line
	6450 2650 6350 2650
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 5EB95AF7
P 6600 3100
F 0 "JP2" H 6600 3000 50  0000 C CNN
F 1 "SolderJumper_Open" H 6600 3214 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 6600 3100 50  0001 C CNN
F 3 "~" H 6600 3100 50  0001 C CNN
	1    6600 3100
	1    0    0    -1  
$EndComp
Connection ~ 6750 2850
Wire Wire Line
	6750 2850 6650 2850
Wire Wire Line
	6250 3100 6450 3100
Wire Wire Line
	6250 3100 6250 3000
Wire Wire Line
	6250 3000 6450 3000
Wire Wire Line
	6450 3000 6450 2650
Connection ~ 6250 3100
Wire Wire Line
	6150 3650 6450 3650
Text Notes 5200 2250 0    50   ~ 0
Optional for power down when \nAmiga is shutdown
NoConn ~ 4800 3950
NoConn ~ 4800 4450
NoConn ~ 4800 4550
NoConn ~ 4800 3850
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5EBD4F96
P 3400 2500
F 0 "#FLG0103" H 3400 2575 50  0001 C CNN
F 1 "PWR_FLAG" H 3400 2673 50  0000 C CNN
F 2 "" H 3400 2500 50  0001 C CNN
F 3 "~" H 3400 2500 50  0001 C CNN
	1    3400 2500
	-1   0    0    1   
$EndComp
$Comp
L power:+9V #PWR0103
U 1 1 5EBD74C3
P 3400 2400
F 0 "#PWR0103" H 3400 2250 50  0001 C CNN
F 1 "+9V" H 3415 2573 50  0000 C CNN
F 2 "" H 3400 2400 50  0001 C CNN
F 3 "" H 3400 2400 50  0001 C CNN
	1    3400 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2500 3400 2400
Text GLabel 3400 2450 0    50   Input ~ 0
VCC_7-12V
Wire Wire Line
	6750 2850 6750 3100
$Comp
L Connector:Barrel_Jack_Switch J2
U 1 1 5EB558AE
P 7250 2750
F 0 "J2" H 7100 2750 50  0000 L CNN
F 1 "Barrel_Jack" H 7050 2950 50  0000 L CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 7300 2710 50  0001 C CNN
F 3 "~" H 7300 2710 50  0001 C CNN
	1    7250 2750
	-1   0    0    1   
$EndComp
Wire Notes Line
	6600 2300 6600 3000
Text Notes 7200 2400 0    50   ~ 0
Optional Power Source
Text Notes 7200 2500 0    50   ~ 0
7-12V recommended
Wire Notes Line
	8100 2300 8100 3000
Wire Notes Line
	6600 2300 8100 2300
Wire Notes Line
	6600 3000 8100 3000
Text Notes 5390 5030 1    50   ~ 0
VCC\n
Text Notes 5590 5030 1    50   ~ 0
GND\n
NoConn ~ 4800 4650
NoConn ~ 6150 4750
Text GLabel 3800 2450 0    50   Input ~ 0
VCC
Text GLabel 5350 5100 3    50   Input ~ 0
VCC
Text GLabel 5550 5100 3    50   Input ~ 0
GND
NoConn ~ 3750 3100
Connection ~ 6800 4000
Text GLabel 8000 4600 0    50   Input ~ 0
5V
$Comp
L Device:R_Small R4
U 1 1 5EBAF827
P 8250 4700
F 0 "R4" H 8309 4700 50  0000 L CNN
F 1 "R_Small" H 8309 4700 50  0001 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8250 4700 50  0001 C CNN
F 3 "~" H 8250 4700 50  0001 C CNN
	1    8250 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5EBAFFCE
P 8250 5050
F 0 "D2" V 8289 4933 50  0000 R CNN
F 1 "Power LED Indicator" V 8198 4933 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 8250 5050 50  0001 C CNN
F 3 "~" H 8250 5050 50  0001 C CNN
	1    8250 5050
	0    -1   -1   0   
$EndComp
Text GLabel 8250 5300 3    50   Input ~ 0
GND
Wire Wire Line
	8000 4600 8250 4600
Wire Wire Line
	8250 4800 8250 4900
Wire Wire Line
	8250 5200 8250 5300
Text GLabel 3750 3600 0    50   Input ~ 0
ACK
Text GLabel 3750 5400 0    50   Output ~ 0
STROBE
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5EBC446E
P 4850 5250
F 0 "J3" H 4930 5246 50  0001 L CNN
F 1 "Conn_01x01" H 4930 5201 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4850 5250 50  0001 C CNN
F 3 "~" H 4850 5250 50  0001 C CNN
	1    4850 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5EBC5D5C
P 4850 5400
F 0 "J4" H 4930 5396 50  0001 L CNN
F 1 "Conn_01x01" H 4930 5351 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4850 5400 50  0001 C CNN
F 3 "~" H 4850 5400 50  0001 C CNN
	1    4850 5400
	1    0    0    -1  
$EndComp
Text GLabel 4650 5250 0    50   Input ~ 0
STROBE
Text GLabel 4650 5400 0    50   Output ~ 0
ACK
Text GLabel 5550 2500 0    50   Input ~ 0
Amiga5V
Wire Notes Line
	6550 2050 6550 2950
Wire Notes Line
	5100 2050 5100 2950
$Comp
L Device:R_Small R5
U 1 1 5EC32818
P 5650 2500
F 0 "R5" V 5546 2500 50  0000 C CNN
F 1 "R_Small" V 5545 2500 50  0001 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5650 2500 50  0001 C CNN
F 3 "~" H 5650 2500 50  0001 C CNN
	1    5650 2500
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5EC49DEA
P 5350 5550
F 0 "J5" H 5430 5592 50  0001 L CNN
F 1 "Conn_01x01" H 5430 5501 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 5350 5550 50  0001 C CNN
F 3 "~" H 5350 5550 50  0001 C CNN
	1    5350 5550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J6
U 1 1 5EC4A7EF
P 5750 5550
F 0 "J6" H 5830 5592 50  0001 L CNN
F 1 "Conn_01x01" H 5830 5501 50  0001 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 5750 5550 50  0001 C CNN
F 3 "~" H 5750 5550 50  0001 C CNN
	1    5750 5550
	1    0    0    -1  
$EndComp
NoConn ~ 5550 5550
NoConn ~ 5150 5550
Wire Notes Line
	6550 2950 5100 2950
Wire Notes Line
	6550 2050 5100 2050
Text Notes 5050 5700 0    50   ~ 0
Leg support for sd-module
Wire Wire Line
	7500 4000 6800 4000
Wire Wire Line
	6700 4000 6800 4000
$Comp
L Transistor_BJT:BC847W Q1
U 1 1 5EB97D03
P 7900 4000
F 0 "Q1" H 8090 4046 50  0000 L CNN
F 1 "BC847BW" H 8090 3955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-323_SC-70" H 8100 3925 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 7900 4000 50  0001 L CNN
	1    7900 4000
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC847W Q2
U 1 1 5EB9885D
P 8400 3750
F 0 "Q2" H 8590 3796 50  0000 L CNN
F 1 "BC847BW" H 8590 3705 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-323_SC-70" H 8600 3675 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 8400 3750 50  0001 L CNN
	1    8400 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R2
U 1 1 5EB9BBF1
P 8000 3450
F 0 "R2" H 8059 3450 50  0000 L CNN
F 1 "R_Small" H 8059 3405 50  0001 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8000 3450 50  0001 C CNN
F 3 "~" H 8000 3450 50  0001 C CNN
	1    8000 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R3
U 1 1 5EB9C4B4
P 8250 3150
F 0 "R3" V 8350 3150 50  0000 C CNN
F 1 "R_Small" H 8309 3105 50  0001 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8250 3150 50  0001 C CNN
F 3 "~" H 8250 3150 50  0001 C CNN
	1    8250 3150
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 5EB9CA00
P 7600 4000
F 0 "R1" V 7496 4000 50  0000 C CNN
F 1 "R_Small" V 7495 4000 50  0001 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7600 4000 50  0001 C CNN
F 3 "~" H 7600 4000 50  0001 C CNN
	1    7600 4000
	0    1    1    0   
$EndComp
Text GLabel 8500 4200 3    50   Input ~ 0
GND
Wire Wire Line
	8500 3950 8500 4200
Wire Wire Line
	8500 4200 8000 4200
Text GLabel 8000 3150 0    50   Input ~ 0
5V
$Comp
L Device:LED D1
U 1 1 5EBA7223
P 8500 3300
F 0 "D1" V 8539 3183 50  0000 R CNN
F 1 "Activity LED Indicator" V 8448 3183 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 8500 3300 50  0001 C CNN
F 3 "~" H 8500 3300 50  0001 C CNN
	1    8500 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8000 3150 8150 3150
Wire Wire Line
	8500 3450 8500 3550
Wire Wire Line
	8000 3550 8000 3750
Wire Wire Line
	8000 3350 8000 3150
Wire Wire Line
	8350 3150 8500 3150
Wire Wire Line
	8200 3750 8000 3750
Connection ~ 8000 3750
Wire Wire Line
	8000 3750 8000 3800
Wire Wire Line
	6800 4000 6800 4250
Wire Notes Line
	7500 3050 9450 3050
Wire Notes Line
	9450 4450 7500 4450
Text Notes 8650 3150 0    50   ~ 0
Optional Activity LED
Wire Notes Line
	7800 4500 7800 5550
Wire Notes Line
	7800 5550 9200 5550
Wire Notes Line
	9200 5550 9200 4500
Wire Notes Line
	9200 4500 7800 4500
Text Notes 8400 4600 0    50   ~ 0
Optional Power LED
Wire Notes Line
	9450 3050 9450 4450
Wire Notes Line
	7500 3050 7500 4450
$EndSCHEMATC
