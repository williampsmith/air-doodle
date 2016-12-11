import serial, time
import numpy as np

def writeToSerial(destination, bitmap, shouldClearDisplay = 1, timeout = 0.04):
    # destination: serial destination such as to Arduino
    # bitmap: the list of bytes to send, top left to bottom right
    # shouldClearDisplay: 1 means to clear previous display before displaying new bitmap
    # timeout: time to wait to let data get delivered to Arduino
    data = [1] + bitmap + [shouldClearDisplay] # append bitmap list and shouldClearDisplay bit
    destination.write(data)
    time.sleep(timeout)

def scrollBitmapMatrixLeftRight(bitmapMatrix, numPixels = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # direction: -1 means scroll left, 1 means scroll right
    for _ in range(numPixels):
        newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
        for col in range(32):
            for row in range(32):
                newBitmapMatrix[row][(col + direction) % 32] = bitmapMatrix[row][col]
        bitmapMatrix = newBitmapMatrix
        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(arduino, bitmap)
        time.sleep(period)
    return newBitmapMatrix

def scrollBitmapMatrixUpDown(bitmapMatrix, numPixels = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # direction: -1 means scroll down, 1 means scroll up
    direction = -direction # since display (0,0) starts at top left instead of bottom right
    for _ in range(numPixels):
        newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
        for col in range(32):
            for row in range(32):
                newBitmapMatrix[(row + direction) % 32][col] = bitmapMatrix[row][col]
        bitmapMatrix = newBitmapMatrix
        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(arduino, bitmap)
        time.sleep(period)
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

def rotateBitmapMatrix90(bitmapMatrix, direction = -1, x0 = 0, y0 = 0, x1 = 31, y1 = 31):
    # This function mutates bitmapMatrix so that it is rotated
    # Rotate a matrix by 90 degrees
    # Direction: -1 means counter-clockwise, 1 means clockwise
    # Rotate the rectangle given by x0,y0,x1,y1
    # x0,y0: start of rectangle
    # x1,y1: end of rectangle
    rotatedMatrix = []
    for row in range(y0, y1 + 1):
        rotatedMatrix.append([])
        for col in range(x0, x1 + 1):
            rotatedMatrix[row].append(bitmapMatrix[row][col])

    if (direction == -1):
        rotatedMatrix = np.rot90(rotatedMatrix, 1) # rotate 1 time 
    else:
        rotatedMatrix = np.rot90(rotatedMatrix, 3) # rotate 3 times, 360 - 90 = 270

    rows = y1 - y0 + 1
    cols = x1 - x0 + 1
    for row in range(rows):
        for col in range(cols):
            bitmapMatrix[row + x0][col + y0] = rotatedMatrix[row][col]
            
    return bitmapMatrix

arduino = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=115200, timeout=0.1)

time.sleep(1) # give the connection a second to settle
arduino.setDTR(level=0)
time.sleep(1)

# display bitmap. Will eventually come from ded reckoning algorithm
bitmapMatrix = [
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]]

bitmap = matrixToBitmap(bitmapMatrix)
writeToSerial(arduino, bitmap)

bitmapMatrix = scrollBitmapMatrixLeftRight(bitmapMatrix, numPixels=4)
bitmapMatrix = scrollBitmapMatrixUpDown(bitmapMatrix, numPixels=4)

for _ in range(4):
    bitmapMatrix = rotateBitmapMatrix90(bitmapMatrix)
    bitmap = matrixToBitmap(bitmapMatrix)
    writeToSerial(arduino, bitmap)
    time.sleep(1)


