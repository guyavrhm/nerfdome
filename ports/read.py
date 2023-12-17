import serial

serial_port = serial.Serial('COM3', baudrate=115200, timeout=1)

try:
    while 1:
        data = serial_port.readline().decode('utf-8').strip()
        if data:
            print(f"Received data: {data}")

except KeyboardInterrupt:
    serial_port.close()
    print("Serial port closed.")
