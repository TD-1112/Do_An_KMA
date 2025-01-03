import serial 


def send_data_to_esp32(data):
    ser = serial.Serial('', 9600)
    ser.write(data.encode())
    ser.close()

