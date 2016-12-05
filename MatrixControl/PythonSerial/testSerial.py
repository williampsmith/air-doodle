import serial, time
# def serialWrite128as64(destination, data):
#     # Optional: send 128 byte data in 512 bit or 64 byte chunks
#     destination.write(data[:64]) # send bytes 0 through 63
#     time.sleep(0.005) # wait 5ms
#     destination.write(data[64:]) # send bytes 64 through 127
#     # time.sleep(0.08) # optional sleep: use if program closes after writing.
#                      # It needs to finish writing before program closes, and
#                      # the sleep will keep program open until it finishes writing.

def writeToSerial(destination, bitmap, shouldClearDisplay = 1, timeout = 0.04):
    # destination: serial destination such as to Arduino
    # bitmap: the list of bytes to send, top left to bottom right
    # shouldClearDisplay: should it reset the display (set to blank display) before displaying bitmap
    # timeout: time to wait to let data get delivered to Arduino
    data = bitmap + [shouldClearDisplay] # append bitmap list and shouldClearDisplay bit
    # serialWrite128as64(destination, data)
    destination.write(data)
    time.sleep(timeout)

def scrollBitmapMatrix(bitmapMatrix, LeftOrRight = 0):
    # Scroll the matrix 1 pixel left or right
    # LeftOrRight: 0 means scroll left, 1 means scroll right
    newBitmapMatrix = []
    for col in range(32):
        for row in range(32):
            newBitmapMatrix[row][col] = bitmapMatrix[(row - 1) % 32][col]
    return newBitmapMatrix

def matrixToBitmap(bitmapMatrix):
    bitmap = []

    for i in range(32):
        for j in range(4):
            bit_array = bitmapMatrix[i][8 * j: (8 * j) + 8] # get 8 bit chunk
            bit_string = ''

            for b in bit_array:
                bit_string += str(b)

            display_int = int(bit_string, 2)
            bitmap.append(display_int)

    return bitmap

arduino = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=115200, timeout=0.1)

time.sleep(1) # give the connection a second to settle
arduino.setDTR(level=0)
time.sleep(1)

# print(arduino.read()) # read and print 1 byte
# time.sleep(1)
# print(bin(int(arduino.read()))) # cast to int, then binary and print binary

x = 15
bitmap = [x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,
          x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x]

writeToSerial(arduino, bitmap, 0, timeout=5)

numBytes = 128
bitmap = []
x = 0
for _ in range(numBytes):
   bitmap.append(x)

timer = 0.04
bitmap[3] = 1
bitmap[7] = 1
bitmap[11] = 1
bitmap[15] = 1
bitmap[19] = 1
bitmap[23] = 1
bitmap[27] = 1
bitmap[31] = 1
bitmap[35] = 1
bitmap[39] = 1
writeToSerial(arduino, bitmap)
bitmap[3] = 3
bitmap[7] = 3
bitmap[11] = 3
bitmap[15] = 3
bitmap[19] = 3
bitmap[23] = 3
bitmap[27] = 3
bitmap[31] = 3
bitmap[35] = 3
bitmap[39] = 3
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 7
bitmap[7] = 7
bitmap[11] = 7
bitmap[15] = 7
bitmap[19] = 7
bitmap[23] = 7
bitmap[27] = 7
bitmap[31] = 7
bitmap[35] = 7
bitmap[39] = 7
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 15
bitmap[7] = 15
bitmap[11] = 15
bitmap[15] = 15
bitmap[19] = 15
bitmap[23] = 15
bitmap[27] = 15
bitmap[31] = 15
bitmap[35] = 15
bitmap[39] = 15
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 31
bitmap[7] = 31
bitmap[11] = 31
bitmap[15] = 31
bitmap[19] = 31
bitmap[23] = 31
bitmap[27] = 31
bitmap[31] = 31
bitmap[35] = 31
bitmap[39] = 31
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 63
bitmap[7] = 63
bitmap[11] = 63
bitmap[15] = 63
bitmap[19] = 63
bitmap[23] = 63
bitmap[27] = 63
bitmap[31] = 63
bitmap[35] = 63
bitmap[39] = 63
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 127
bitmap[7] = 127
bitmap[11] = 127
bitmap[15] = 127
bitmap[19] = 127
bitmap[23] = 127
bitmap[27] = 127
bitmap[31] = 127
bitmap[35] = 127
bitmap[39] = 127
writeToSerial(arduino, bitmap, 0)
bitmap[3] = 255
bitmap[7] = 255
bitmap[11] = 255
bitmap[15] = 255
bitmap[19] = 255
bitmap[23] = 255
bitmap[27] = 255
bitmap[31] = 255
bitmap[35] = 255
bitmap[39] = 255
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 1
bitmap[6] = 1
bitmap[10] = 1
bitmap[14] = 1
bitmap[18] = 1
bitmap[22] = 1
bitmap[26] = 1
bitmap[30] = 1
bitmap[34] = 1
bitmap[38] = 1
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 3
bitmap[6] = 3
bitmap[10] = 3
bitmap[14] = 3
bitmap[18] = 3
bitmap[22] = 3
bitmap[26] = 3
bitmap[30] = 3
bitmap[34] = 3
bitmap[38] = 3
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 7
bitmap[6] = 7
bitmap[10] = 7
bitmap[14] = 7
bitmap[18] = 7
bitmap[22] = 7
bitmap[26] = 7
bitmap[30] = 7
bitmap[34] = 7
bitmap[38] = 7
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 15
bitmap[6] = 15
bitmap[10] = 15
bitmap[14] = 15
bitmap[18] = 15
bitmap[22] = 15
bitmap[26] = 15
bitmap[30] = 15
bitmap[34] = 15
bitmap[38] = 15
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 31
bitmap[6] = 31
bitmap[10] = 31
bitmap[14] = 31
bitmap[18] = 31
bitmap[22] = 31
bitmap[26] = 31
bitmap[30] = 31
bitmap[34] = 31
bitmap[38] = 31
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 63
bitmap[6] = 63
bitmap[10] = 63
bitmap[14] = 63
bitmap[18] = 63
bitmap[22] = 63
bitmap[26] = 63
bitmap[30] = 63
bitmap[34] = 63
bitmap[38] = 63
writeToSerial(arduino, bitmap, 0)
bitmap[2] = 127
bitmap[6] = 127
bitmap[10] = 127
bitmap[14] = 127
bitmap[18] = 127
bitmap[22] = 127
bitmap[26] = 127
bitmap[30] = 127
bitmap[34] = 127
bitmap[38] = 127
writeToSerial(arduino, bitmap, 0)
