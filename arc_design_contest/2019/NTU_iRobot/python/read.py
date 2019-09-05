import serial
from time import sleep
import sys

COM_PORT = '/dev/serial/by-id/usb-Digilent_Digilent_USB_Device_251642542476-if00-port0'
#BAUD_RATES = 115200
BAUD_RATES = int(sys.argv[1])
ser = serial.Serial(COM_PORT, BAUD_RATES)
print("Configured")
while True:
    mcu_feedback = ser.readline().decode()
    # mcu_feedback = ser.read(1)
    print(mcu_feedback,)
