Tosot (Gree) Hombask 200Liter

The signal is custom rs485
![alt text](<scope.png>)

All message send
```
12-07 12:47:05 frame=7e 7e 00 f0 00 04 90 31 01 64 47 00 (poll slave 0x00)
12-07 12:47:06 frame=7e 7e 01 f0 00 04 90 31 01 74 87 00 (poll slave 0x01)
12-07 12:47:06 frame=7e 7e f0 01 02 2d 90 31 01 15 21 9a 40 4a 02 c0 02 05 01 c7 01 0c 02 0e 02 c4 00 24 01 17 b4 00 00 00 00 55 8e 5d a7 15 b0 8e 00 aa ff ff ff ff ff 73 4a 57 55 (response slave 0x01)
12-07 12:47:06 frame=7e 7e 02 f0 00 04 90 31 01 47 87 00 (poll slave 0x02)
12-07 12:47:06 frame=7e 7e f0 f0 04 2d 90 31 01 15 21 9a 40 4a 02 c0 02 05 01 c7 01 0c 02 0e 02 c4 00 24 01 17 b4 00 00 00 00 55 8e 5d a7 15 b0 8e 00 aa ff ff ff ff ff 73 73 f6 00 (broadcast master)
12-07 12:47:06 frame=7e 7e f0 f0 06 1c 90 31 01 15 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00 99 1f 00 (broadcast master)
12-07 12:47:07 frame=7e 7e f0 f0 02 3c 90 31 01 77 98 00 7d 00 00 00 00 76 00 74 75 00 00 10 01 01 90 00 00 00 00 f8 00 00 00 00 00 00 00 11 8c 0f 9f c6 1d 98 15 00 00 00 00 70 9a 00 00 00 00 02 00 00 00 00 02 00 00 5a f0 00 (broadcast master)
12-07 12:47:07 frame=7e 7e f0 f0 03 25 90 31 06 00 00 00 14 14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00 09 98 44 00 (broadcast master)
12-07 12:47:07 frame=7e 7e f0 f0 08 40 90 31 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ae 60 00 (broadcast master)
```
Slave can send after a poll frame.
Slave is ready sending when the last padding byte is 0x55
To write byte 0x08 must be 0x02


Aansluiting Rs485 signal for wired controller on H1(pos) and H2(neg) of the XT3 connector
![alt text](<Rs485Connector.jpeg>)


Parameters
00 21 protocol
01 E6 last error
02 53 top
03 53 bottom
04 21 Environment
05 25 discharge gas temp
06 23 suction gas temp
07 21/20 (defrost) (10 when on)
08 --
09 --
10 400b (1) E Expansion valve
11 00b (2) E
12 02
13 000 Fan speed (790 on)
14 00 (comp) F
15 (clock) 034c/350 (comp) 
16 0143 
17 0000
18 034c
19 0000
20 0000
21 015A
22 --
23 --
24 000 (19 als die aanstaat)
25 --


E00 4F:00 (off) defrost
E01 y:01 memory
E02 00 Celcius / fahrenheid
E03 01 Electric heating
E04 Gr:67
E05 00
E07 01
E08 u7:01
E09 HE:42
E10 HE:07
E12 00
E13 80
E14 00
E15 EL:00
E16 24:00 (clock)(2)
E17 03
E18 00
E19 10
E20 10
E21 Ho:15
E22 ][:01
E23 200L
E24 58 (comp)
E25 750
E26 dP:01
E27 750

F00 70 Legionella
F01 -7 (comp + vacation)
F02 -15 (elec heating + vacation)
F03 05 (comp)(on)
F04 01 (on/off)
F05 00
F06 Gn:00
F12 CE:00
F17 95 (comp)
F18 900 (fan)
F19 CL:00
