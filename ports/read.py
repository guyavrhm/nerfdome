import serial

serial_port = serial.Serial('COM5', baudrate=9600, timeout=1)

try:
    while 1:
        data = serial_port.readline().decode('utf-8').strip()
        print(f"Received data: {data}")

except KeyboardInterrupt:
    serial_port.close()
    print("Serial port closed.")
