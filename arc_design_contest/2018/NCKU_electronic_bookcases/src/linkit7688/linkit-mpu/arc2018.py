import requests
import serial
import time

book_ID_length = 2
command_ext_length = 2
user_ID_length = 5
command_total_length = 9

url="http://192.168.1.102/ARC2DB.php"
s = None
def setup():
    global s
    # open serial COM port to /dev/ttyS0, which maps to UART0(D0/D1)
    # the baudrate is set to 57600 and should be the same as the one
    # specified in the Arduino sketch uploaded to ATmega32U4.
    s = serial.Serial("/dev/ttyS0", 57600)

def loop():
    if s.inWaiting() >= book_ID_length+command_ext_length:
        a=s.read(s.inWaiting())
        if a[command_total_length-1] == 'Z' : ##end of cmd
            if a[0] == 'A':
                s.flushInput()
                mydata={"SearchID":a[1:1+book_ID_length]}
                response=requests.post(url,data=mydata)
                response.raise_for_status()
                print(response.text[1:])
                if "error mysql connect" in response.text:
                    s.write("E-2Z")
                else:
                    temp = response.text[1:]
                    s.write(temp.encode()+"Z")
            elif a[0] == 'B':
                s.flushInput()
                mydata={"BorrowID":a[1:1+book_ID_length],"NewOwner":a[1+book_ID_length:1+book_ID_length+user_ID_length]}
                response=requests.post(url,data=mydata)
                response.raise_for_status()
                print(response.text[1:])
                if "error mysql connect" in response.text:
                    s.write("E-2Z")
                else:
                    temp = response.text[1:]
                    s.write(temp.encode()+"Z")
            elif a[0] == 'C':
                s.flushInput()
                mydata={"ReturnID":a[1:1+book_ID_length],"ReturnLocation":a[1+book_ID_length:1+book_ID_length+3]}
                response=requests.post(url,data=mydata)
                response.raise_for_status()
                print(response.text[1:])
                if "error mysql connect" in response.text:
                    s.write("E-2Z")
                else:
                    temp = response.text[1:]
                    s.write(temp.encode()+"Z")
            elif a[0] == 'D':
                s.flushInput()
                mydata={"ReserveID":a[1:1+book_ID_length],"Reservation":a[1+book_ID_length:1+book_ID_length+user_ID_length]}
                response=requests.post(url,data=mydata)
                response.raise_for_status()
                print(response.text[1:])
                if "error mysql connect" in response.text:
                    s.write("E-2Z")
                else:
                    temp = response.text[1:]
                    s.write(temp.encode()+"Z")
            elif a[0] == 'E':
                s.flushInput()
                mydata={"CancelID":a[1:1+book_ID_length],"CancelReservation":a[1+book_ID_length:1+book_ID_length+user_ID_length]}
                response=requests.post(url,data=mydata)
                response.raise_for_status()
                print(response.text[1:])
                if "error mysql connect" in response.text:
                    s.write("E-2Z")
                else:
                    temp = response.text[1:]
                    s.write(temp.encode()+"Z")
            else:
                s.write("0")
        else:
            s.write("0")
            
if __name__=='__main__':
    setup()
    while True:
        loop() 
