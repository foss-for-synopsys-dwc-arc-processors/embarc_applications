import socket
import datetime
import time
from grove_current_sensor import Current
from threading import Timer
from datetime import datetime
import tkinter as tk
import random

pin0 = 0
pin1 = 1
pin2 = 2
pin3 = 3
pin4 = 4
pin5 = 5
pin6 = 6
pin7 = 7
sensor_type = "5A_DC"

#if use 10A current sensor input: pin = 0 , sensor_type = "10A"
if (sensor_type == "2.5A"):
    sensitivity = 1000.0 / 800.0
    Vref = 260
if (sensor_type == "5A_DC"):
    sensitivity = 1000.0 / 200.0
    Vref = 1498
if (sensor_type == "5A_AC"):
    sensitivity = 1000.0 / 200.0
    Vref = 1498
if (sensor_type == "10A"):
    sensitivity = 1000.0 / 264.0
    Vref = 322
averageValue = 1

ADC = Current()

#Left client connect to server
TCP_IP = '192.168.137.106' 
TCP_PORT = 5007
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Waiting for connection...')
s.connect((TCP_IP, TCP_PORT))
print('left client has connected to server')

#Initially read pressure data
#'''
temp0 = ADC.get_nchan_current_data(pin0,sensitivity,Vref,averageValue)
temp1 = ADC.get_nchan_current_data(pin1,sensitivity,Vref,averageValue)
temp2 = ADC.get_nchan_current_data(pin2,sensitivity,Vref,averageValue)
temp3 = ADC.get_nchan_current_data(pin3,sensitivity,Vref,averageValue)
temp4 = ADC.get_nchan_current_data(pin4,sensitivity,Vref,averageValue)
temp5 = ADC.get_nchan_current_data(pin5,sensitivity,Vref,averageValue)
temp6 = ADC.get_nchan_current_data(pin6,sensitivity,Vref,averageValue)
temp7 = ADC.get_nchan_current_data(pin7,sensitivity,Vref,averageValue)
#'''

#------For simulation-------
'''
temp0 = [0, 0]
temp1 = [0, 0]
temp2 = [0, 0]
temp3 = [0, 0]
temp4 = [0, 0]
temp5 = [0, 0]
temp6 = [0, 0]
temp7 = [0, 0]
'''
#----------------------------

#Finish flag
finish_flag = False

#Time duration
data_collection_duration = 60 #unit: seconds
n = data_collection_duration * 100
i = 0

#Send pressure data to server
def send_data(mV_value):
    s.send(str(mV_value).encode(encoding="utf-8"))

def read_data():
    global i, n, finish_flag, first_time, temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7
    if(i < n):
        total = temp0[1] + temp1[1] + temp2[1] + temp3[1] + temp4[1] + temp5[1] + temp6[1] + temp7[1]
        pass_time = datetime.now() - first_time
        #print(str(datetime.now()))
        #print('pass time: '+str(pass_time))
        #print('i = '+str(i))

        send_data(str(pass_time)[5:11]+' '+str(int(temp0[1]))+' '+str(int(temp1[1]))+' '+str(int(temp2[1]))+' '+str(int(temp3[1]))+' '+str(int(temp4[1]))+' '+str(int(temp5[1]))+' '+str(int(temp6[1]))+' '+str(int(temp7[1]))+' '+str(total)+' \n')
        print(str(i)+' '+str(pass_time)[5:11]+' '+str(int(temp0[1]))+' '+str(int(temp1[1]))+' '+str(int(temp2[1]))+' '+str(int(temp3[1]))+' '+str(int(temp4[1]))+' '+str(int(temp5[1]))+' '+str(int(temp6[1]))+' '+str(int(temp7[1]))+' '+str(total)+' \n')
        
        #send_data(str(pass_time)[5:11]+' '+'111'+' '+'122'+' '+'133'+' '+'144'+' '+'155'+' '+'166'+' '+'177'+' '+'188'+' '+'199'+' \n')
        #print(str(pass_time)[5:11]+' '+'111'+' '+'122'+' '+'133'+' '+'144'+' '+'155'+' '+'166'+' '+'177'+' '+'188'+' '+'199'+' \n')        
        i += 1
    else:
        send_data('e') #Send end sign
        #s.close()
        print('over')
        #t.cancel()

        finish_flag = True

#Send data repeating thread    
class RepeatingTimer(Timer): 
    def run(self):
        while not self.finished.is_set():
            self.function(*self.args, **self.kwargs)
            self.finished.wait(self.interval)
t = RepeatingTimer(0.009, read_data)

#Receiving timestamp from server
#'''
str_timestamp = s.recv(BUFFER_SIZE).decode()
first_time = datetime.fromtimestamp(int(str_timestamp))
#'''

#first_time = datetime.now()
print('first time: '+str(first_time))

#Start send data thread
t.start()

#Keep reading current pressure data
while finish_flag == False:
    #'''
    temp0 = ADC.get_nchan_current_data(pin0,sensitivity,Vref,averageValue)
    temp1 = ADC.get_nchan_current_data(pin1,sensitivity,Vref,averageValue)
    temp2 = ADC.get_nchan_current_data(pin2,sensitivity,Vref,averageValue)
    temp3 = ADC.get_nchan_current_data(pin3,sensitivity,Vref,averageValue)
    temp4 = ADC.get_nchan_current_data(pin4,sensitivity,Vref,averageValue)
    temp5 = ADC.get_nchan_current_data(pin5,sensitivity,Vref,averageValue)
    temp6 = ADC.get_nchan_current_data(pin6,sensitivity,Vref,averageValue)
    temp7 = ADC.get_nchan_current_data(pin7,sensitivity,Vref,averageValue)
    #'''
    #-------Simulation-------------------------
    '''
    temp0[1] = random.randint(5,40)
    temp1[1] = random.randint(5,40)
    temp2[1] = random.randint(5,40)
    temp3[1] = random.randint(5,40)
    temp4[1] = random.randint(5,40)
    temp5[1] = random.randint(5,40)
    temp6[1] = random.randint(5,40)
    temp7[1] = random.randint(5,40)
    '''
    #------------------------------------------
    #print(str(temp0[1])+' '+str(temp1[1])+' '+str(temp2[1])+' '+str(temp3[1])+' '+str(temp4[1])+' '+str(temp5[1])+' '+str(temp6[1])+' '+str(temp7[1])+' '+str(total)+'\n')
    total = temp0[1] + temp1[1] + temp2[1] + temp3[1] + temp4[1] + temp5[1] + temp6[1] + temp7[1]
    delta_time = str(datetime.now() - first_time)
    data_left = delta_time[5:11] +' '+ str(temp0[1]) +' '+ str(temp1[1]) +' '+ str(temp2[1]) +' '+ str(temp3[1]) +' '+ str(temp4[1]) +' '+ str(temp5[1]) +' '+ str(temp6[1]) +' '+ str(temp7[1]) +' '+ str(total)
    #print(data_left)