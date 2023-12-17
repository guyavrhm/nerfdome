import bluetooth, subprocess
import time
# nearby_devices = bluetooth.discover_devices(duration=4,lookup_names=True,
#                                                       flush_cache=True, lookup_class=False)

# print(nearby_devices)


bt_mac = "D8:3A:DD:41:7A:F9" # Replace with yours.
port = 1
sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bt_mac, port))

while 1:
    sock.send(input("Send: "))
