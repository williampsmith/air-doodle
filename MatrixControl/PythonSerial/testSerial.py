import serial, time
arduino = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=9600, timeout=.1)
time.sleep(1) #give the connection a second to settle
arduino.setDTR(level=0)
time.sleep(1)
# x = 5
# arduino.write([x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x])
# arduino.write([5,5,5,5,5,5,5,5])
# arduino.write([0b00000000, 0b11000000,
#   0b00000001, 0b11000000,
#   0b00000001, 0b11000000,
#   0b00000011, 0b11100000,
#   0b11110011, 0b11100000,
#   0b11111110, 0b11111000,
#   0b01111110, 0b11111111,
#   0b00110011, 0b10011111,
#   0b00011111, 0b11111100,
#   0b00001101, 0b01110000,
#   0b00011011, 0b10100000,
#   0b00111111, 0b11100000,
#   0b00111111, 0b11110000,
#   0b01111100, 0b11110000,
#   0b01110000, 0b01110000,
#   0b00000000, 0b00110000])

# bitmap = []
# x = 1
# numBytes = 66
# for _ in range(numBytes):
#     bitmap.append(x)
# arduino.write(bitmap)

# time.sleep(1)
# bitmap = []
# x = 5
# numBytes = 66
# for _ in range(numBytes):
#     bitmap.append(x)
# arduino.write(bitmap)

numBytes = 66
bitmap = []
x = 0
for _ in range(numBytes):
    bitmap.append(x)

timer = 0.2
bitmap[2] = 1
bitmap[5] = 1
bitmap[8] = 1
bitmap[11] = 1
bitmap[14] = 1
bitmap[17] = 1
bitmap[20] = 1
bitmap[23] = 1
bitmap[26] = 1
bitmap[29] = 1
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 3
bitmap[5] = 3
bitmap[8] = 3
bitmap[11] = 3
bitmap[14] = 3
bitmap[17] = 3
bitmap[20] = 3
bitmap[23] = 3
bitmap[26] = 3
bitmap[29] = 3
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 7
bitmap[5] = 7
bitmap[8] = 7
bitmap[11] = 7
bitmap[14] = 7
bitmap[17] = 7
bitmap[20] = 7
bitmap[23] = 7
bitmap[26] = 7
bitmap[29] = 7
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 15
bitmap[5] = 15
bitmap[8] = 15
bitmap[11] = 15
bitmap[14] = 15
bitmap[17] = 15
bitmap[20] = 15
bitmap[23] = 15
bitmap[26] = 15
bitmap[29] = 15
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 31
bitmap[5] = 31
bitmap[8] = 31
bitmap[11] = 31
bitmap[14] = 31
bitmap[17] = 31
bitmap[20] = 31
bitmap[23] = 31
bitmap[26] = 31
bitmap[29] = 31
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 63
bitmap[5] = 63
bitmap[8] = 63
bitmap[11] = 63
bitmap[14] = 63
bitmap[17] = 63
bitmap[20] = 63
bitmap[23] = 63
bitmap[26] = 63
bitmap[29] = 63
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 127
bitmap[5] = 127
bitmap[8] = 127
bitmap[11] = 127
bitmap[14] = 127
bitmap[17] = 127
bitmap[20] = 127
bitmap[23] = 127
bitmap[26] = 127
bitmap[29] = 127
arduino.write(bitmap)
time.sleep(timer)
bitmap[2] = 255
bitmap[5] = 255
bitmap[8] = 255
bitmap[11] = 255
bitmap[14] = 255
bitmap[17] = 255
bitmap[20] = 255
bitmap[23] = 255
bitmap[26] = 255
bitmap[29] = 255
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 1
bitmap[4] = 1
bitmap[7] = 1
bitmap[10] = 1
bitmap[13] = 1
bitmap[16] = 1
bitmap[19] = 1
bitmap[22] = 1
bitmap[25] = 1
bitmap[28] = 1
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 3
bitmap[4] = 3
bitmap[7] = 3
bitmap[10] = 3
bitmap[13] = 3
bitmap[16] = 3
bitmap[19] = 3
bitmap[22] = 3
bitmap[25] = 3
bitmap[28] = 3
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 7
bitmap[4] = 7
bitmap[7] = 7
bitmap[10] = 7
bitmap[13] = 7
bitmap[16] = 7
bitmap[19] = 7
bitmap[22] = 7
bitmap[25] = 7
bitmap[28] = 7
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 15
bitmap[4] = 15
bitmap[7] = 15
bitmap[10] = 15
bitmap[13] = 15
bitmap[16] = 15
bitmap[19] = 15
bitmap[22] = 15
bitmap[25] = 15
bitmap[28] = 15
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 31
bitmap[4] = 31
bitmap[7] = 31
bitmap[10] = 31
bitmap[13] = 31
bitmap[16] = 31
bitmap[19] = 31
bitmap[22] = 31
bitmap[25] = 31
bitmap[28] = 31
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 63
bitmap[4] = 63
bitmap[7] = 63
bitmap[10] = 63
bitmap[13] = 63
bitmap[16] = 63
bitmap[19] = 63
bitmap[22] = 63
bitmap[25] = 63
bitmap[28] = 63
arduino.write(bitmap)
time.sleep(timer)
bitmap[1] = 127
bitmap[4] = 127
bitmap[7] = 127
bitmap[10] = 127
bitmap[13] = 127
bitmap[16] = 127
bitmap[19] = 127
bitmap[22] = 127
bitmap[25] = 127
bitmap[28] = 127
arduino.write(bitmap)


# print(bin(int(arduino.read())))

# count = 0
# while(count < 128):
#     print(arduino.read())
#     # time.sleep(1)
#     count += 1

# import serial
# import time

# arduino = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=9600, timeout=.1)

# time.sleep(1)
# arduino.setDTR(level=0)
# time.sleep(1)

# # display bitmap. Will eventually come from ded reckoning algorithm
# bitmap = [
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
#     [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1]]

# display = []

# for i in range(32):
#     for j in range(4):
#         bit_array = bitmap[i][8 * j: (8 * j) + 8] # get 8 bit chunk
#         bit_string = ''

#         for b in bit_array:
#             bit_string += str(b)
#         #print('bit string:', bit_string)
#         display_int = int(bit_string, 2)
#         #print('correspongding int:', display_int)
#         display.append(display_int)

# arduino.write(display)
# print(display)
