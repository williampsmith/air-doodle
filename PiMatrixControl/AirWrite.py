import serial
import time
import bluetooth

# gesture dictionary
gestures = {1 : '0', 2 : '8', 3 : '9'}

def writeCharacter(destination, character, shouldClearDisplay = 1, timeout = 0.04):
    # destination: serial destination such as to Arduino
    # bitmap: the list of bytes to send, top left to bottom right
    # shouldClearDisplay: should it reset the display (set to blank display) before displaying bitmap
    # timeout: time to wait to let data get delivered to Arduino
    data = [0] + [ord(character)] + [shouldClearDisplay] # append bitmap list and shouldClearDisplay bit
    # serialWrite128as64(destination, data)
    destination.write(data)
    time.sleep(timeout)

print("Setting up arduino connection.")

#arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=0.1)

time.sleep(1)
#arduino.setDTR(0)
time.sleep(1)

print("Arduino connection setup.")

# If bluetooth error occurs code cleans up then starts again
while True:

    print("Setting up bluetooth connection.")

    server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    server_sock.bind(("",bluetooth.PORT_ANY))
    server_sock.listen(1)

    port = server_sock.getsockname()[1]

    uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"

    bluetooth.advertise_service( server_sock, "AirDoodleServer",
                       service_id = uuid,
                       service_classes = [ uuid, bluetooth.SERIAL_PORT_CLASS ],
                       profiles = [ bluetooth.SERIAL_PORT_PROFILE ],
    #                   protocols = [ OBEX_UUID ]
                        )

    print("Waiting for connection on RFCOMM channel %d" % port)
    client_sock, client_info = server_sock.accept()

    print("Accepted connection from ", client_info)

    print("Bluetooth setup.")

    print("Entering send-display loop.")

    curr = 0
    w = {}

    try:
        while True:
            data = client_sock.recv(1024)
            if (len(data) == 0): break
            data = ord(data)
            time.sleep(0.5)
            for i in range(0, len(data), 2):
                pos = int(data[i])
                val = int(data[i+1])
                print 'Received => Pos: [', pos, ' ] Val: [', val, ' ]' #gestures[val]
                #
                # if pos == curr:
                # 	#arduino.write(val.encode())
                #     #writeCharacter(arduino, 'S')
                #     writeCharacter(arduino, val)
                #     time.sleep(0.5)
                #     curr += 1
                #     curr = curr % 256
                #     #time.sleep(0.5)
                #     while curr in w:
                #     	#arduino.write(w.pop(curr).encode())
                #         #writeToSerial(arduino, bitmap)
                #         #writeCharacter(arduino, 'S')
                #         writeCharacter(arduino, w.pop(curr))
                #         time.sleep(0.5)
                #     	curr += 1
                #         curr = curr % 256
                #     	#time.sleep(0.5)
                #     else:
                #         w[pos] = val
                #     curr = curr % 256
    except IOError:
        pass

    print("Bluetooth Disconnected")

    client_sock.close()
    server_sock.close()

print("All done")
