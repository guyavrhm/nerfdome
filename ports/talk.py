import serial

serial_port = serial.Serial('COM3', baudrate=115200, timeout=1)
try:
    while True:
        send_data = input("$ ") 
        serial_port.write(send_data.encode('utf-8'))

        received_data = serial_port.readline().decode('utf-8').strip()
        print(f"RP2040: {received_data}")

except KeyboardInterrupt:
    serial_port.close()
    print("Serial port closed.")
