import cv2
import numpy as np

def track_blue_object(video_source=0):
    cap = cv2.VideoCapture(video_source)

    cv2.namedWindow("Blue Object Trackingq", cv2.WINDOW_NORMAL)
    cv2.namedWindow("HSV Frame", cv2.WINDOW_NORMAL)
    cv2.namedWindow("Mask Frame", cv2.WINDOW_NORMAL)

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower_blue = np.array([100, 50, 50])
        upper_blue = np.array([130, 255, 255])

        mask = cv2.inRange(hsv, lower_blue, upper_blue)
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if contours:
            largest_contour = max(contours, key=cv2.contourArea)

            M = cv2.moments(largest_contour)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])

                # Draw a circle at the centroid
                cv2.circle(frame, (cx, cy), 5, (0, 255, 0), -1)
            print(cx, cy);

        cv2.imshow("Blue Object Tracking", frame)
        cv2.imshow("HSV Frame", hsv)
        cv2.imshow("Mask Frame", mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

track_blue_object()

