import serial
import requests

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=10)
url = "http://localhost:8000/temperature/"

while True:
    try:
        ser.flushOutput()
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode("utf-8").strip()
        print(decoded_bytes)
        if 'POST' in decoded_bytes:
            arr = decoded_bytes.split()
            requests.post(arr[1] + arr[2])
            ser.write(b'1')
            ser.flushOutput()
        
    except:
        print("Keyboard Interrupt")
        break