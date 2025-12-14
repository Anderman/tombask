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
```
7e7e start frame
dst
src
code?
length
payload
crc (modbus16)
00 for  master 0x55 for slave 0x01
```
for decoding see python script

Decoding 2d frame
sc sc ds sr fu le pr pr rw a0 a1 Ta a3 a4 a5 a6 a7 a8 a9 b0 b1 b2 b3 b4 b5 b6 b7 b9 b9 c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 d0 d1 d2 d3 d4 d5 d7 d7 d8 d9 e0 cr cr pa
7e 7e f0 01 02 2d 90 31 01 15 21 9a 40 4a 02 c0 02 05 01 c7 01 0c 02 0e 02 c4 00 24 01 17 b4 00 00 00 00 55 8e 5d a7 15 b0 8e 00 aa ff ff ff ff ff 73 4a 57 55 (response slave 0x01)
Decoding 3c frame
sc sc ds sr fu le pr pr rw T1 T2 Ta T3 a4 a5 a6 a7 T4 a9 T5 T6 b2 b3 b4 b5 b6 b7 b9 b9 c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 d0 d1 d2 d3 d4 d5 d7 d7 d8 d9 e0 e1 e2 T7 e4 e5 e6 e7 e8 e9 f0 f1 f2 f3 f4 f5 cr cr pa
7e 7e f0 f0 02 3c 90 31 01 77 98 00 7d 00 00 00 00 76 00 74 75 00 00 10 01 01 90 00 00 00 00 f8 00 00 00 00 00 00 00 11 8c 0f 9f c6 1d 98 15 00 00 00 00 70 9a 00 00 00 00 02 00 00 00 00 02 00 00 5a f0 00 (broadcast master)

Sending Target temperature

12-07 12:47:06 frame=7e 7e 01 f0 00 04 90 31 01 74 87 00 (poll slave 0x01)
                              sr             RW       Ta
12-07 12:47:06 frame=7e 7e f0 01 02 2d 90 31 02 15 21 9a 40 4a 02 c0 02 05 01 c7 01 0c 02 0e 02 c4 00 24 01 17 b4 00 00 00 00 55 8e 5d a7 15 b0 8e 00 aa ff ff ff ff ff 73 cr cr 55 (response slave 0x01)
12-07 12:47:06 frame=7e 7e 01 f0 00 04 90 31 01 74 87 00 (poll slave 0x01)
                              sr             RW
12-07 12:47:06 frame=7e 7e f0 01 04 1c 90 31 02 15 2c c0 01 00 00 10 ff ff 00 00 af b4 04 4b 73 5f 5a 3a 4b 00 00 00 00 53 07 55 (broadcast master)

