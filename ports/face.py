import cv2
import time
import bluetooth
import socket
import struct

bt_mac = "D8:3A:DD:41:7A:F9"
port = 1
sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bt_mac, port))

# Load the pre-trained face detector from OpenCV
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

        # Map coordinates to the range of 0 to 180
        mapped_x = int((x + w/2) * 180 / frame.shape[1])
        mapped_y = int((y + h/2) * 180 / frame.shape[0])
        sock.send(struct.pack('BBB', 1, mapped_y, 180 - mapped_x))

        print(f"Mapped face coordinates: ({mapped_x}, {mapped_y})")


    cv2.imshow('Face Tracking', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
