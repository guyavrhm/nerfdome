import cv2
import numpy as np

def track_blue_object(video_source=0):
    # Open the video capture
    cap = cv2.VideoCapture(video_source)


    while True:
        # Read a frame from the video capture
        ret, frame = cap.read()
        if not ret:
            break

        # Convert the frame to the HSV color space
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # Define the lower and upper bounds for the blue color in HSV
        lower_blue = np.array([100, 50, 50])
        upper_blue = np.array([130, 255, 255])

        # Create a mask using the inRange function
        mask = cv2.inRange(hsv, lower_blue, upper_blue)

        # Find contours in the mask
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # If contours are found, find the largest one
        if contours:
            largest_contour = max(contours, key=cv2.contourArea)

            # Get the centroid of the largest contour
            M = cv2.moments(largest_contour)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])

                # Draw a circle at the centroid
                cv2.circle(frame, (cx, cy), 5, (0, 255, 0), -1)
            print(cx, cy);

        # Display the frame
        cv2.imshow("Blue Object Tracking", frame)

        # Exit the loop when 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the video capture and close the OpenCV windows
    cap.release()
    cv2.destroyAllWindows()

# Run the tracking function
track_blue_object()

