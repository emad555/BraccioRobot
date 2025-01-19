import cv2
import serial
import time
import threading

# Initialize serial connection to Arduino (adjust COM port and baud rate)
arduino = serial.Serial('/dev/ttyACM0', 9600)  # Replace with your Arduino port
time.sleep(2)  # Wait for connection to establish

# Initialize video capture
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FPS, 15)  # Limit FPS for better performance
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)  # Lower resolution for better performance
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

# Global variables for tracking
tracking = False
bbox = None
tracker = cv2.TrackerCSRT_create()  # Using CSRT tracker for more accurate tracking
frame_counter = 0
frame_interval = 2  # Track every 2 frames to reduce load

# Mouse callback function to get the ROI centered on the click
def select_roi(event, x, y, flags, param):
    global bbox, tracking
    if event == cv2.EVENT_LBUTTONDOWN:
        width, height = 100, 100  # Size of ROI, adjust as needed
        x_start = max(0, x - width // 2)
        y_start = max(0, y - height // 2)
        bbox = (x_start, y_start, width, height)
        tracking = True
        tracker.init(frame, bbox)  # Initialize tracker with centered ROI

# Serial writing function to run in a separate thread
def send_coordinates_to_arduino(coordinates):
    try:
        arduino.write(coordinates.encode())
    except Exception as e:
        print(f"Serial write error: {e}")

# Set up mouse callback
cv2.namedWindow("Live Tracking")
cv2.setMouseCallback("Live Tracking", select_roi)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Process tracking every frame_interval frames to reduce load
    if tracking and frame_counter % frame_interval == 0:
        success, bbox = tracker.update(frame)
        if success:
            x, y, w, h = map(int, bbox)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

            # Send coordinates to Arduino in a separate thread
            coordinates = f"{x},{y}\n"
            threading.Thread(target=send_coordinates_to_arduino, args=(coordinates,)).start()
        else:
            cv2.putText(frame, "Tracking failed", (50, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)

    # Display the tracking frame
    cv2.putText(frame, "Click to select object", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    cv2.imshow("Live Tracking", frame)

    frame_counter += 1

    # Exit on 'q' key
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
arduino.close()
cv2.destroyAllWindows()