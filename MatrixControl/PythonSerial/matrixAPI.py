from characterBitmapMatrices import getBitmapMatrixOfCharacter
import serial, time
# import numpy as np

def writeToSerial(destination, bitmap, shouldClearDisplay = 1, timeout = 0.04):
    # destination: serial destination such as to Arduino
    # bitmap: the list of bytes to send, top left to bottom right
    # shouldClearDisplay: should it reset the display (set to blank display) before displaying bitmap
    # timeout: time to wait to let data get delivered to Arduino
    data = [1] + bitmap + [shouldClearDisplay] # append bitmap list and shouldClearDisplay bit
    destination.write(data)
    time.sleep(timeout)

def writeCharacter(destination, character, shouldClearDisplay = 1, timeout = 0.04):
    # destination: serial destination such as to Arduino
    # bitmap: the list of bytes to send, top left to bottom right
    # shouldClearDisplay: should it reset the display (set to blank display) before displaying bitmap
    # timeout: time to wait to let data get delivered to Arduino
    data = [0] + [ord(character)] + [shouldClearDisplay] # append bitmap list and shouldClearDisplay bit
    # serialWrite128as64(destination, data)
    destination.write(data)
    time.sleep(timeout)

def attachCharacterMatrixToBitmapMatrix(characterMatrix):
    # Attaches one character at the center of the bitmap matrix and returns the bitmap matrix
    width = len(characterMatrix[0])
    height = len(characterMatrix)
    offsetRow = (32 - height) // 2
    offsetCol = (32 - width) // 2

    bitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
    for row in range(height):
        for col in range(width):
            bitmapMatrix[row+offsetRow][col+offsetCol] = characterMatrix[row][col]

    return bitmapMatrix

def attachCharacterMatricesToBitmapMatrix(characterMatrices, maxCharWidth, totalWidth, maxCharHeight, totalHeight):
    # Attaches varying size characters to the current bitmap matrix, appending the characters to existing characters
    # and returns the bitmap matrix
    offsetRow = (32 - totalHeight) // 2
    offsetCol = (32 - totalWidth) // 2
    totalRowOffset = offsetRow
    totalColOffset = offsetCol

    bitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
    for characterMatrix in characterMatrices:
        charWidth = len(characterMatrix[0])
        charHeight = len(characterMatrix)
        # subtract 1 since character matrix starts at index totalRowOffset, not after
        if (totalColOffset + charWidth - 1) > 31:
            totalColOffset = offsetCol
            totalRowOffset += maxCharHeight
        if (totalRowOffset + charHeight - 1) > 31:
            break
        for row in range(charHeight):
            for col in range(charWidth):
                bitmapMatrix[row+totalRowOffset][col+totalColOffset] = characterMatrix[row][col]

        totalColOffset += charWidth

    return bitmapMatrix

def stringToBitmapMatrix(string):
    # generates a bitmap matrix of the characters of the string
    # and returns the bitmap matrix
    size = len(string)
    characterMatrices = []
    totalWidth = 0
    totalHeight = 0
    maxCharWidth = 0
    maxCharHeight = 0

    if size > 3:
        if size > 12:
            string = string[:12]
        size = 3

    for character in string:
        bitmapMatrixOfCharacter = getBitmapMatrixOfCharacter(character, size)
        characterMatrices.append(bitmapMatrixOfCharacter)

        width = len(bitmapMatrixOfCharacter[0])
        height = len(bitmapMatrixOfCharacter)
        totalWidth += width

        if (width > maxCharWidth):
            maxCharWidth = width
        if (height > maxCharHeight):
            maxCharHeight = height

    totalHeight = maxCharHeight
    if totalWidth > 32:
        totalHeight = maxCharHeight * (1 + (totalWidth // 32))
        totalWidth = 32
    bitmapMatrix = attachCharacterMatricesToBitmapMatrix(characterMatrices, maxCharWidth, totalWidth, maxCharHeight, totalHeight)
    return bitmapMatrix

def stringToBitmap(string):
    # generates a bit string given a string and returns the bit string
    return matrixToBitmap(stringToBitmapMatrix(string))

def matrixToBitmap(bitmapMatrix):
    # generates a bit string given a bit matrix and returns the bit string
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

def scrollBitmapMatrixLeftRight(destination, bitmapMatrix, numPixels = 1, pixelSkip = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # numPixels: number of pixels to scroll
    # pixelSkip: number of pixels to skip
    # direction: -1 means scroll left, 1 means scroll right
    # period: the time between frame updates
    for _ in range(numPixels // pixelSkip):
        for _ in range(pixelSkip):
            newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
            for row in range(32):
                for col in range(32):
                    newBitmapMatrix[row][(col + direction) % 32] = bitmapMatrix[row][col]
            bitmapMatrix = newBitmapMatrix

        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(destination, bitmap)
        time.sleep(period)
    return newBitmapMatrix

def scrollBitmapMatrixLeftRightOffScreen(destination, bitmapMatrix, numPixels = 32, pixelSkip = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # numPixels: number of pixels to scroll
    # pixelSkip: number of pixels to skip
    # direction: -1 means scroll left, 1 means scroll right
    # period: the time between frame updates
    for _ in range(numPixels // pixelSkip):
        for _ in range(pixelSkip):
            newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
            for row in range(32):
                for col in range(32):
                    if (col == 31 and direction == 1) or (col == 0 and direction == -1):
                        continue
                    newBitmapMatrix[row][col + direction] = bitmapMatrix[row][col]
            bitmapMatrix = newBitmapMatrix

        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(destination, bitmap)
        time.sleep(period)

    return newBitmapMatrix

def scrollBitmapMatrixUpDown(destination, bitmapMatrix, numPixels = 1, pixelSkip = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # numPixels: number of pixels to scroll
    # pixelSkip: number of pixels to skip
    # direction: -1 means scroll down, 1 means scroll up
    # period: the time between frame updates
    direction = -direction # since display (0,0) starts at top left instead of bottom right
    for _ in range(numPixels // pixelSkip):
        for _ in range(pixelSkip):
            newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
            for row in range(32):
                for col in range(32):
                    newBitmapMatrix[(row + direction) % 32][col] = bitmapMatrix[row][col]
            bitmapMatrix = newBitmapMatrix

        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(destination, bitmap)
        time.sleep(period)
    return newBitmapMatrix

def scrollBitmapMatrixUpDownOffScreen(destination, bitmapMatrix, numPixels = 32, pixelSkip = 1, direction = -1, period = 0.05):
    # Scroll the matrix 1 pixel left or right
    # numPixels: number of pixels to scroll
    # pixelSkip: number of pixels to skip
    # direction: -1 means scroll down, 1 means scroll up
    # period: the time between frame updates
    direction = -direction # since display (0,0) starts at top left instead of bottom right
    for _ in range(numPixels // pixelSkip):
        for _ in range(pixelSkip):
            newBitmapMatrix = [[0 for _ in range(32)] for _ in range(32)]
            for row in range(32):
                for col in range(32):
                    if (row == 31 and direction == 1) or (row == 0 and direction == -1):
                        continue
                    newBitmapMatrix[row + direction][col] = bitmapMatrix[row][col]
            bitmapMatrix = newBitmapMatrix

        bitmap = matrixToBitmap(bitmapMatrix)
        writeToSerial(destination, bitmap)
        time.sleep(period)
    return newBitmapMatrix

def swipeScrollLeftRight(destination, string, direction):
    # destination: device to send to via serial (e.g. arduino)
    # string: the string to scroll
    # direction: -1 means scroll left, 1 means scroll right
    scrollBitmapMatrixLeftRightOffScreen(destination, stringToBitmapMatrix(string), numPixels = 32, pixelSkip = 6, direction = direction)

# def rotateBitmapMatrix90(bitmapMatrix, direction = -1, x0 = 0, y0 = 0, x1 = 31, y1 = 31):
#     # This function mutates bitmapMatrix so that it is rotated
#     # Rotate a matrix by 90 degrees
#     # Direction: -1 means counter-clockwise, 1 means clockwise
#     # Rotate the rectangle given by x0,y0,x1,y1
#     # x0,y0: start of rectangle
#     # x1,y1: end of rectangle
#     rotatedMatrix = []
#     for row in range(y0, y1 + 1):
#         rotatedMatrix.append([])
#         for col in range(x0, x1 + 1):
#             rotatedMatrix[row].append(bitmapMatrix[row][col])

#     if (direction == -1):
#         rotatedMatrix = np.rot90(rotatedMatrix, 1) # rotate 1 time
#     else:
#         rotatedMatrix = np.rot90(rotatedMatrix, 3) # rotate 3 times, 360 - 90 = 270

#     rows = y1 - y0 + 1
#     cols = x1 - x0 + 1
#     for row in range(rows):
#         for col in range(cols):
#             bitmapMatrix[row + x0][col + y0] = rotatedMatrix[row][col]

#     return bitmapMatrix



# *********************************************************************************
'''
arduino = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=115200, timeout=0.1)

time.sleep(1) # give the connection a second to settle
arduino.setDTR(level=0)
time.sleep(1)

# writeCharacter(arduino, 'A')

# bitmapMatrix = [
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
#     [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]]

# bitmap = matrixToBitmap(bitmapMatrix)
# writeToSerial(arduino, bitmap)

# bitmapMatrix = attachCharacterMatrixToBitmapMatrix(getBitmapMatrixOfCharacter('f', 1))
# bitmap = matrixToBitmap(bitmapMatrix)
# writeToSerial(arduino, bitmap)
# bitmapMatrix = scrollBitmapMatrixLeftRightOffScreen(arduino, bitmapMatrix, numPixels = 32, pixelSkip = 4, direction = 1, period = 0.02)

# swipeScrollLeftRight(arduino, "ab", -1)

# writeToSerial(arduino, stringToBitmap("01"))
# time.sleep(1)
# writeToSerial(arduino, stringToBitmap("045"))
# time.sleep(1)
# writeToSerial(arduino, stringToBitmap("0"))
# time.sleep(1)
# writeToSerial(arduino, stringToBitmap("67"))
# time.sleep(1)
# writeToSerial(arduino, stringToBitmap("89"))
# time.sleep(1)
# scrollBitmapMatrixLeftRightOffScreen(arduino, stringToBitmapMatrix("089"), numPixels = 32, pixelSkip = 6)

# for _ in range(4):
#     bitmapMatrix = rotateBitmapMatrix90(bitmapMatrix)
#     bitmap = matrixToBitmap(bitmapMatrix)
#     writeToSerial(arduino, bitmap)
#     time.sleep(1)
'''
# *********************************************************************************
