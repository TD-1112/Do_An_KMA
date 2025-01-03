from mqtt import get_received_message, publish_message
from detect import detect_faces_and_recognize
import cv2
import time

# Initialize camera
cam = cv2.VideoCapture(0)
if not cam.isOpened():
    print("Error: Could not open camera.")
    exit()

cam.set(cv2.CAP_PROP_FPS, 30)

last_status = None  # Store last received status
message_sent = False  # Move this outside the loop

try:
    while True:
        current_status = get_received_message()
        
        # Only process if we receive a new status different from the last one
        if current_status != last_status:
            print(f"New status received: {current_status}")
            last_status = current_status
            message_sent = False  # Reset flag when status changes

            if current_status == "1":
                print("Starting face detection...")
                start_time = time.time()

                while time.time() - start_time < 3 and not message_sent:
                    ret, frame = cam.read()
                    if not ret:
                        print("Error: Could not read frame from camera.")
                        break

                    results = detect_faces_and_recognize(frame)

                    if results:
                        detected_names = [name for name, (x, y, w, h) in results]
                        
                        if "Ha" in detected_names and not message_sent:
                            for name, (x, y, w, h) in results:
                                cv2.putText(frame, name, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
                                cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
                                print(f"Detected: {name}")
                            publish_message("face_de", "ok")
                            message_sent = True
                            break
                        elif not message_sent:  # If face detected but not "Ha"
                            publish_message("face_de", "no")
                            message_sent = True
                            break
                    else:  # No faces detected
                        if not message_sent and time.time() - start_time >= 2.8:  # Check near timeout
                            publish_message("face_de", "no_detect")
                            message_sent = True
                            break

                    cv2.imshow('Face Recognition', frame)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break
        
        time.sleep(0.1)  # Prevent CPU overload

except KeyboardInterrupt:
    print("\nExiting program")
finally:
    cam.release()
    cv2.destroyAllWindows()