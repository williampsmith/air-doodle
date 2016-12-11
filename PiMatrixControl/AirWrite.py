import serial
import time
import bluetooth

print("Setting up arduino connection.")

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=0.1)

time.sleep(1)
arduino.setDTR(level=0)
time.sleep(1)

print("Arduino connection setup.")

print("Setting up bluetooth connection.")

server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_sock.bind(("",bluetooth.PORT_ANY))
server_sock.listen(1)

port = server_sock.getsockname()[1]

uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"

bluetooth.advertise_service( server_sock, "AirDoodleServer",
                   service_id = uuid,
                   service_classes = [ uuid, SERIAL_PORT_CLASS ],
                   profiles = [ SERIAL_PORT_PROFILE ],
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
        pos = client_sock.recv(1024)
        if (len(pos) == 0): break
        time.sleep(0.5)
        val = client_sock.recv(1024)
        print("Received => Pos: [%s] Val: [%s]" % pos, val)
        if pos == curr:
        	arduino.write(val.encode())
        	curr += 1
        	time.sleep(0.5)
        	while curr in w:
        		arduino.write(w.pop(curr).encode())
    			curr += 1
    			time.sleep(0.5)
        else:
        	w[pos] = val
        curr = curr % 256
except IOError:
    pass

print("Bluetooth Disconnected")

client_sock.close()
server_sock.close()
print("All done")