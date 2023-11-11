import cv2
import numpy as np


def bgr_to_hsv_limits(bgr_color, tolerance = 10):
    hsv_color = cv2.cvtColor(np.uint8([[bgr_color]]), cv2.COLOR_BGR2HSV)
    print(hsv_color)

    lower_limit = np.array([max(0, hsv_color[0][0][0] - tolerance), 100, 100], dtype=np.uint8)
    upper_limit = np.array([min(179, hsv_color[0][0][0] + tolerance), 255, 255], dtype=np.uint8)

    return lower_limit, upper_limit


def track_object(lower_color, upper_color, video_source=0):
    cap = cv2.VideoCapture(video_source)

    cv2.namedWindow("Object Tracking", cv2.WINDOW_NORMAL)
    cv2.namedWindow("HSV Frame", cv2.WINDOW_NORMAL)
    cv2.namedWindow("Mask Frame", cv2.WINDOW_NORMAL)

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        mask = cv2.inRange(hsv, lower_color, upper_color)
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

        cv2.imshow("Object Tracking", frame)
        cv2.imshow("HSV Frame", hsv)
        cv2.imshow("Mask Frame", mask)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

bgr_color = [255, 0, 0]
print(bgr_color)
lower, upper = bgr_to_hsv_limits(bgr_color, tolerance=30)
print("Lower Limit:", lower)
print("Upper Limit:", upper)
track_object(lower, upper)

