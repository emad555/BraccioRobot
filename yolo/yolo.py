import gi
import serial
import time
from threading import Thread
from queue import Queue

gi.require_version('Gst', '1.0')
from gi.repository import Gst
import cv2
import hailo
from hailo_rpi_common import (
    get_caps_from_pad,
    get_numpy_from_buffer,
    app_callback_class,
)
from detection_pipeline import GStreamerDetectionApp

# -----------------------------------------------------------------------------------------------
# User-defined class to be used in the callback function
# -----------------------------------------------------------------------------------------------
# Inheritance from the app_callback_class
class user_app_callback_class(app_callback_class):
    def __init__(self):
        super().__init__()
        self.serial_connection = None

# -----------------------------------------------------------------------------------------------
# Detection processing function
# -----------------------------------------------------------------------------------------------
def process_detections(detection_queue):
    while True:
        detections, width, height, serial_connection = detection_queue.get()
        for detection in detections:
            label = detection.get_label()
            bbox = detection.get_bbox()
            if label == "cell phone" or label == "remote":
                x1 = int(bbox.xmin() * width)
                y1 = int(bbox.ymin() * height)
                coordinates = f"{x1},{y1}\n"
                print(f"Sending coordinates: {coordinates.strip()}")

                # Send coordinates to Arduino
                try:
                    if serial_connection and serial_connection.out_waiting == 0:
                        serial_connection.write(coordinates.encode())
                except Exception as e:
                    print(f"Error sending data: {e}")

        detection_queue.task_done()

# -----------------------------------------------------------------------------------------------
# Callback function
# -----------------------------------------------------------------------------------------------
def app_callback(pad, info, user_data):
    buffer = info.get_buffer()
    if buffer is None:
        return Gst.PadProbeReturn.OK

    user_data.increment()
    format, width, height = get_caps_from_pad(pad)

    roi = hailo.get_roi_from_buffer(buffer)
    detections = roi.get_objects_typed(hailo.HAILO_DETECTION)

    # Add detections to the queue for asynchronous processing
    detection_queue.put((detections, width, height, user_data.serial_connection))

    frame = None
    if user_data.use_frame and format and width and height:
        frame = get_numpy_from_buffer(buffer, format, width, height)
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        user_data.set_frame(frame)

    print(f"Frame count: {user_data.get_count()}")
    return Gst.PadProbeReturn.OK

if __name__ == "__main__":
    # Set up serial communication
    try:
        serial_connection = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
        time.sleep(2)
    except Exception as e:
        print(f"Error initializing serial connection: {e}")
        serial_connection = None

    # Create an instance of the user app callback class
    user_data = user_app_callback_class()
    user_data.serial_connection = serial_connection

    # Create a detection queue and processing thread
    detection_queue = Queue()
    Thread(target=process_detections, args=(detection_queue,), daemon=True).start()

    app = GStreamerDetectionApp(app_callback, user_data)

    try:
        app.run()
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        if serial_connection:
            serial_connection.close()
