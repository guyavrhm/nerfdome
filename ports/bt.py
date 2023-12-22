import bluetooth
import time
import struct

bt_mac = "D8:3A:DD:41:7A:F9"
port = 1
sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bt_mac, port))
try:
    while 1:
        try:
            command_type, param1, param2 = map(int, input("Enter: ").split())
            data_to_send = struct.pack('BBB', command_type, param1, param2)
            sock.send(data_to_send)
            print(f"Sent: {struct.unpack('BBB', data_to_send)}")
        except (ValueError, struct.error):
            print("Usage: <command (uint8_t)> <param (uint8_t)> <param (uint8_t)>")

        # response = sock.recv(1024)
        # print(response)
except (KeyboardInterrupt, EOFError):
    sock.close()
