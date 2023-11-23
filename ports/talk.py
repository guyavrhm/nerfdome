import serial

# Open the serial port for communication
serial_port = serial.Serial('COM3', baudrate=115200, timeout=1)

try:
    while True:
        # Send data
        send_data = input("$ ")  # Get user input
        serial_port.write(send_data.encode('utf-8'))

        # Receive data
        received_data = serial_port.readline().decode('utf-8').strip()
        print(f"RP2040: {received_data}")

except KeyboardInterrupt:
    # Close the serial port when the script is interrupted
    serial_port.close()
    print("Serial port closed.")
