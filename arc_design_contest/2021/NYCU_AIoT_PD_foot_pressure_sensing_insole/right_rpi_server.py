import socket
import datetime
import time
from grove_current_sensor import Current
import threading
from threading import Timer
from datetime import datetime
import numpy as np
import serial
import os
from decimal import Decimal
import random

#==================================================Configure parameters===============================================================
data_collection_duration = 60 #unit: seconds
begin_restore_time = 30 #unit: seconds
#=====================================================================================================================================

#===============================================Initialize current sensor of RPi======================================================
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
#=====================================================================================================================================


#=================================================Initialize parameters===============================================================
data_left = str.encode('11 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1')
data_right = '1 1 1 1 1 1 1 1 1 1 1 1 1 1 1'
left_data_ready = False
right_data_ready = False
left_data = []
right_data = []
left_final_data = []
right_final_data = []
str_final_score = -1

#String: RGB value for sending to APP client (L1 ~ R8)
RGB_value_l1 = '#FFFFFF'
RGB_value_l2 = '#FFFFFF'
RGB_value_l3 = '#FFFFFF'
RGB_value_l4 = '#FFFFFF'
RGB_value_l5 = '#FFFFFF'
RGB_value_l6 = '#FFFFFF'
RGB_value_l7 = '#FFFFFF'
RGB_value_l8 = '#FFFFFF'
RGB_value_r1 = '#FFFFFF'
RGB_value_r2 = '#FFFFFF'
RGB_value_r3 = '#FFFFFF'
RGB_value_r4 = '#FFFFFF'
RGB_value_r5 = '#FFFFFF'
RGB_value_r6 = '#FFFFFF'
RGB_value_r7 = '#FFFFFF'
RGB_value_r8 = '#FFFFFF'

#Stop flag
stop_flag = False
finish_flag = False
#=====================================================================================================================================


#==============================================Transfer pressure value to color value=================================================
def color(mV_voltage):
    if (mV_voltage > 255):
        mV_voltage = 255
    elif (mV_voltage < 0):
        mV_voltage = 0
    pin_voltage = 255 - mV_voltage
    if(pin_voltage < 16):
        voltage_trans = str(hex(int(pin_voltage))).replace('0x', '0')
    else:
        voltage_trans = str(hex(int(pin_voltage))).replace('0x', '')
    color_value = '#ff' + voltage_trans + voltage_trans
    return color_value
#=====================================================================================================================================


#==========================================Draw functions: change pressure data to RGB values=========================================
def draw_left(): #Left foot
    global data_left, RGB_value_l1, RGB_value_l2, RGB_value_l3, RGB_value_l4, RGB_value_l5, RGB_value_l6, RGB_value_l7, RGB_value_l8
    word = data_left.decode()[7:50].split(' ')
    #print(word[0]+' '+word[1]+' '+word[2]+' '+word[3]+' '+word[4]+' '+word[5]+' '+word[6]+' '+word[7]+' '+word[8])
        
    l1 = 63.75+((float(word[0])-6)*9.8)
    l2 = 63.75+((float(word[1])-4)*5.1)
    l3 = 63.75+((float(word[2])-3)*8.5)
    l4 = 63.75+((float(word[3])-5)*7.97)
    l5 = 63.75+((float(word[4])-5)*7.083)
    l6 = 63.75+((float(word[5])-3)*15.94)
    l7 = 63.75+((float(word[6])-3)*14.166)
    l8 = 63.75+((float(word[7])-1)*15.94)

    RGB_value_l1 = color(l1).upper()
    RGB_value_l2 = color(l2).upper()
    RGB_value_l3 = color(l3).upper()
    RGB_value_l4 = color(l4).upper()
    RGB_value_l5 = color(l5).upper()
    RGB_value_l6 = color(l6).upper()
    RGB_value_l7 = color(l7).upper()
    RGB_value_l8 = color(l8).upper()

def draw_right():#Right foot
    global data_right, RGB_value_r1, RGB_value_r2, RGB_value_r3, RGB_value_r4, RGB_value_r5, RGB_value_r6, RGB_value_r7, RGB_value_r8
    #word = data_right[7:50].split(' ')
    word = data_right.split(' ')
        
    r1 = 63.75+((float(word[1])-7)*15.94)
    r2 = 63.75+((float(word[2])-4)*8.5)
    r3 = 63.75+((float(word[3])-3)*6.07)
    r4 = 63.75+((float(word[4])-4)*7.5)
    r5 = 63.75+((float(word[5])-7)*6.375)
    r6 = 63.75+((float(word[6])-2)*9.808)
    r7 = 63.75+((float(word[7])-3)*12.75)
    r8 = 63.75+((float(word[8])-2)*7.9688)
        
    RGB_value_r1 = color(r1).upper()
    RGB_value_r2 = color(r2).upper()
    RGB_value_r3 = color(r3).upper()
    RGB_value_r4 = color(r4).upper()
    RGB_value_r5 = color(r5).upper()
    RGB_value_r6 = color(r6).upper()
    RGB_value_r7 = color(r7).upper()
    RGB_value_r8 = color(r8).upper()
#=====================================================================================================================================


#=============================================Thread: recieve data from left foot RPi client==========================================
def threaded_client_left(connection):
    global data_left, left_data_ready, left_data, begin_restore_time

    #----------------------------------------------Recieve data and write file--------------------------------------------------------
    fp_left = open('data/data_left_APP.txt','w') #'data/data_left_' + name.get() + '.txt'
    while True:
        data_left = connection.recv(BUFFER_SIZE)
        if (data_left.decode()[0] == 'e'):
            dr_left.cancel()
            break
        fp_left.write(data_left.decode())
    fp_left.close()
    connection.close()
    #---------------------------------------------------------------------------------------------------------------------------------

    #----------------------------------Calculate first and third quartile of left foot data-------------------------------------------
    arr = []
    with open('data/data_left_APP.txt') as f: #'data/data_left_' + name.get() + '.txt'
        for line in f.readlines():
            if (line[0] != 'e'):
                s = line.split(' ')
                arr.append(float(s[9]))
    Q1 = np.quantile(arr, .25)
    Q3 = np.quantile(arr, .75)
    if Q3 == Q1:
        Q3 = Q1 + 1
    print('Left Q1: '+str(Q1)+'   Q3: '+str(Q3))
    #---------------------------------------------------------------------------------------------------------------------------------

    #-------------------------Map the raw foot pressure data to database data with first and third quartile---------------------------
    #'''
    with open('data/data_left_APP.txt') as f: #'data/data_left_' + name.get() + '.txt'
        i = 0
        n = begin_restore_time * 100
        for line in f.readlines():
            if line[0] != 'e' and i >= n:
                s = line.split(' ')
                #total = 5.647+((float(s[9])-Q1)*824.723/(Q3-Q1))
                total = 5.647+((arr[i]-Q1)*824.723/(Q3-Q1))
                if (total < 0):
                    total = 0 
                left_temp = []
                left_temp.append(float(s[0]))
                left_temp.append(total)
                left_data.append(left_temp)
            i += 1
    #'''
    #---------------------Simulation: read restored files----------------------
    '''
    with open('data/sample_test_left_co2.txt') as f: #'data/Quartile_Left.txt'
        i = 0
        for line in f.readlines():
            if line[0] != 'e' and i >= 0:
                s = line.split(' ')
                left_temp = []
                left_temp.append(float(s[0]))
                left_temp.append(float(s[1]))
                left_data.append(left_temp)
                #print('left_data:'+str(left_data[i-500][0])+'  '+str(left_data[i-500][1])+' i='+str(i))
            i += 1
    '''
    #--------------------------------------------------------------------------
    #---------------------------------------------------------------------------------------------------------------------------------

    data_left = str.encode('11 1 1 1 1 1 1 1 1 1 1 1 1 1 1')
    left_data_ready = True
    print('Left foot data receiving done!')
#=====================================================================================================================================


#====================================Thread: send pressure data and final score to APP client=========================================
def threaded_client_APP(connection):
    global finish_flag, stop_flag, RGB_value_l1, RGB_value_l2, RGB_value_l3, RGB_value_l4, RGB_value_l5, RGB_value_l6, RGB_value_l7, RGB_value_l8, RGB_value_r1, RGB_value_r2, RGB_value_r3, RGB_value_r4, RGB_value_r5, RGB_value_r6, RGB_value_r7, RGB_value_r8
    data_from_APP = 'O'
    while data_from_APP[0] != 'S':
        time.sleep(0.01)
        if finish_flag == False:
            str_to_APP = '{'+RGB_value_l1+','+RGB_value_l2+','+RGB_value_l3+','+RGB_value_l4+','+RGB_value_l5+','+RGB_value_l6+','+RGB_value_l7+','+RGB_value_l8+','+RGB_value_r1+','+RGB_value_r2+','+RGB_value_r3+','+RGB_value_r4+','+RGB_value_r5+','+RGB_value_r6+','+RGB_value_r7+','+RGB_value_r8+'} \n'
            #print(str_to_APP)
            connection.send(str_to_APP.encode())
            data_from_APP = connection.recv(BUFFER_SIZE).decode()
        else:
            print('Send final score: '+str_final_score)
            connection.send(str_final_score.encode())
            data_from_APP = connection.recv(BUFFER_SIZE).decode()
            print(data_from_APP)
    print('APP stop!')
    stop_flag = True
#=====================================================================================================================================


#====================================Result funtion: after data collecting, including data pre-processing, serial transmition and change the mass=========================================
#Show result
def show_result():
    global left_data_ready, right_data_ready
    #Wait for data receiving
    while True:
        if left_data_ready==True and right_data_ready==True:
            break

    #Initialize parameters
    left_data_ready = False
    right_data_ready = False

    #-----------------------Data Preprocessing--------------------------
    #'''
    global right_data, left_data, right_final_data, left_final_data
    #Maximum Value
    maximum_left = 1
    maximum_right = 1

    left_preprocessed_data = []
    right_preprocessed_data = []
                
    sample = 100

    name_cnt = 0.0

    x1 = 0.0
    x2 = 0.0

    left_temp_data = 0.0
    right_temp_data = 0.0

    row_cnt = 0

    slope_temp = 0.0
    time_temp = 0.0
    inter_temp = 0.0
                            
    start_time = -0.1
    end_time = 0.0
    time_per_sample = 0.0
    slope = 0.0
    inter = 0.0

    over_500 = False
    below_100 = False
    step_flag = False

    cnt = 0

    start_index = 0
    end_index = 0
    index_l = 0
    index_r = 0
                        
    while start_time < 0:
        if left_data[cnt][1] < 5:
            start_time = left_data[cnt][0]
            start_index = cnt
        cnt += 1
                
    end_flag = False
    step = 0
    start_step = 4 
    while step < (4 + start_step):
        while (not over_500):
            if left_data[cnt][1] > 500:
                over_500 = True
                print("over_500")
            cnt += 1
            #print(cnt)
                    
        over_500 = False
        below_100 = False
        step_flag = True
        while left_data[cnt][1] > 5:
            cnt += 1
            if left_data[cnt][1] > 500 and below_100:
                step_flag = False
            if left_data[cnt][1] < 100:
                below_100 = True
            if cnt == 8000:
                step_flag = False
                end_flag = True
                break
                            
        end_index = cnt
        end_time = left_data[cnt][0]
        time_per_sample = (end_time - start_time) / sample
        index_l = start_index
                    
        if step_flag:
            step += 1
            print(step)
            for i in range(100):
                t = 0.0
                t = start_time + i * time_per_sample
                #print(t)
                while left_data[index_l][0] > t or t >= left_data[index_l+1][0]:
                    index_l += 1
                            
                #print(index_l)
                time_temp = round((left_data[index_l + 1][0] - left_data[index_l][0]), 5)
                slope_temp = round((left_data[index_l + 1][1] - left_data[index_l][1]), 5)
                inter_temp = round((left_data[index_l + 1][1] * left_data[index_l][0] - left_data[index_l][1] * left_data[index_l+1][0]), 5)
                            
                slope = slope_temp / time_temp
                slope = round(slope, 5)
                inter = inter_temp / time_temp
                inter = round(inter, 5)
                t = round(t, 5)
                left_temp_data = slope * t - inter
                left_temp_data = round(left_temp_data, 5)
                        
                            
                while right_data[index_r][0] > t or t >= right_data[index_r+1][0]:
                    index_r += 1
                    #print(index_r)
                            
                time_temp = round((right_data[index_r + 1][0] - right_data[index_r][0]), 5)
                slope_temp = round((right_data[index_r + 1][1] - right_data[index_r][1]), 5)
                inter_temp = round((right_data[index_r + 1][1] * right_data[index_r][0] - right_data[index_r][1] * right_data[index_r+1][0]), 5)
                            
                slope = slope_temp / time_temp
                slope = round(slope, 5)
                inter = inter_temp / time_temp
                inter = round(inter, 5)
                t = round(t, 5)
                right_temp_data = slope * t - inter
                right_temp_data = round(right_temp_data, 5)
                            
                #t = round(t, 5)
                            
                #Maximum value judgement
                if left_temp_data > maximum_left: maximum_left = left_temp_data
                if right_temp_data > maximum_right: maximum_right = right_temp_data

                left_preprocessed_data.append(left_temp_data)
                right_preprocessed_data.append(right_temp_data)
                            
        start_index = end_index + 1
        start_time = left_data[start_index][0]

    print('Data preprocessing finish time: '+str(datetime.now()))
    #'''
    #--------------------------------------------------------------------

    #------------Scaling the preprocessed data to range: 0 ~ 255---------
    #'''
    for i in range(400):
        left_final_data.append(round(left_preprocessed_data[i] * (255/maximum_left), 3))
        right_final_data.append(round(right_preprocessed_data[i] * (255/maximum_right), 3))
    #'''
    #--------------------------------------------------------------------

    #------------Serial Transmit----------comment below to jump----------
    #'''
    ser = serial . Serial('/dev/ttyUSB0',baudrate = 115200)
    ser.flush()
    #open serial
    ser.close()
    if not ser.isOpen():
        ser.open()
    print('/dev/ttyUSB0 is open', ser.isOpen())
    endsign = 'c'
    startsign = 'd'

    golden=['','']
    def data_transmit():
        global left_final_data, right_final_data
        ans=['','']
        ser.write(startsign.encode())
        data = ser.readline()
        str1 = data.decode().replace('\r\n', '')
        while str1 != 'data_buf = 100':
            data = ser.readline()
            str1 = data.decode().replace('\r\n', '')

        print('start transmit')
        
        for i in range(400):
            s = str(right_final_data[i])
            if s.find('.') == -1:
                s = 'a' + s + '.000b'
            part = s.split('.')
            if len(part[1]) == 1:
                s = 'a' + s + '00b'
            elif len(part[1]) == 2:
                s = 'a' + s+ '0b'
            elif len(part[1]) == 3:
                s = 'a' + s + 'b'
            ser.write(s.encode())
            '''
            print('string'+str(i)+'='+s)
            for j in range(1):
                data = ser.readline()
                str1 = data.decode().replace('\r\n', '')
                print(str1)
            '''

        for i in range(400):
            s = str(left_final_data[i])
            if s.find('.') == -1:
                s = 'a' + s + '.000b'
            part = s.split('.')
            if len(part[1]) == 1:
                s = 'a' + s + '00b'
            elif len(part[1]) == 2:
                s = 'a' + s+ '0b'
            elif len(part[1]) == 3:
                s = 'a' + s + 'b'
            ser.write(s.encode())
            '''
            print('string'+str(i)+'='+s)
            for j in range(1):
                data = ser.readline()
                str1 = data.decode().replace('\r\n', '')
                print(str1)
            '''
        ser.write(endsign.encode())

        num = 6
        for i in range(num + 2):
            data = ser.readline()
            str1 = str(data.decode())
            str1 = str1.replace('\r\n', '')
            print(str1)
            if i < num:
                continue
            else:
                ans[i-num] = str1
        return ans

    golden=data_transmit()
    leftscore = float(golden[0])
    rightscore = float(golden[1])

    #-------------------------------------------------Send final score to APP---------------------------------------------------------
    global str_final_score, finish_flag
    str_final_score = '[' + str(leftscore) + ',' + str(rightscore) + '] \n'
    finish_flag = True
    #---------------------------------------------------------------------------------------------------------------------------------

    str_result = 'Left Pd score: '+str(leftscore)+'%\nLeft Co score: '+str(100-leftscore)+'%\nRight Pd score: '+str(rightscore)+'%\nRight Co score: '+str(100-rightscore)+'%'
    print(str_result)
    print('Finish time: '+str(datetime.now()))
    #'''
    #--------------------------------------------------------
#=====================================================================================================================================


#--------------Data collection (Right foot)-------------------
#'''
#Initialize socket parameter of server
TCP_IP = '192.168.137.106' #IP of right foot RPi
TCP_PORT = 5007
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(2) #Left foot RPi and mobile APP

#Connect to left foot client
print('Waiting for left foot RPi to connect...')
conn_left_RPi, addr_left_RPi = s.accept()
print ('Left foot RPi client connection address:', addr_left_RPi)

#------------------------APP client---------------------------------
#'''
#Connect to mobile APP client
print('Waiting for APP to connect...')
conn_APP, addr_APP = s.accept()
print ('APP client connection address:', addr_APP)
    
#Start signal from APP
data_from_APP = conn_APP.recv(BUFFER_SIZE).decode()
print('recieve string: '+data_from_APP)
while data_from_APP[0] != 'G':
    print(' waiting......, recieve string: '+data_from_APP)
    data_from_APP = conn_APP.recv(BUFFER_SIZE).decode()
#'''
#-------------------------------------------------------------------
#Receiving left foot RPi data thread
thread_left = threading.Thread(target=threaded_client_left, args=(conn_left_RPi, ))
thread_left.start()

#Draw distribution diagram repeating threads
class RepeatingTimer(Timer): 
    def run(self):
        while not self.finished.is_set():
            self.function(*self.args, **self.kwargs)
            self.finished.wait(self.interval)
dr_left = RepeatingTimer(0.05, draw_left)  
dr_left.start()
dr_right = RepeatingTimer(0.05, draw_right)
dr_right.start()

#Send timestamp to left foot client
str_timestamp = str(int(datetime.now().timestamp())).encode(encoding="utf-8")
begin_time =  datetime.fromtimestamp(int(str_timestamp.decode()))
conn_left_RPi.send(str_timestamp)

#APP client thread
#'''
thread_APP = threading.Thread(target=threaded_client_APP, args=(conn_APP, ))
thread_APP.start()
#'''

fp_right = open('data/data_right_APP.txt','w') #'data/data_right_' + name.get() + '.txt'
n = data_collection_duration * 100
i = 0
while i < n and stop_flag == False:
    r1 = ADC.get_nchan_current_data(pin0,sensitivity,Vref,averageValue)
    r2 = ADC.get_nchan_current_data(pin1,sensitivity,Vref,averageValue)
    r3 = ADC.get_nchan_current_data(pin2,sensitivity,Vref,averageValue)
    r4 = ADC.get_nchan_current_data(pin3,sensitivity,Vref,averageValue)
    r5 = ADC.get_nchan_current_data(pin4,sensitivity,Vref,averageValue)
    r6 = ADC.get_nchan_current_data(pin5,sensitivity,Vref,averageValue)
    r7 = ADC.get_nchan_current_data(pin6,sensitivity,Vref,averageValue)
    r8 = ADC.get_nchan_current_data(pin7,sensitivity,Vref,averageValue)
    time.sleep(0.004)
        
    total = r1[1] + r2[1] + r3[1] + r4[1] + r5[1] + r6[1] + r7[1] + r8[1]
    delta_time = str(datetime.now() - begin_time)
    data_right = delta_time[5:11] +' '+ str(r1[1]) +' '+ str(r2[1]) +' '+ str(r3[1]) +' '+ str(r4[1]) +' '+ str(r5[1]) +' '+ str(r6[1]) +' '+ str(r7[1]) +' '+ str(r8[1]) +' '+ str(total)
    #print(str(datetime.now()))
    print(data_right+ ' i = ' + str(i))
    fp_right.write(data_right+'\n')
    i += 1
fp_right.close()

#Check if stop has been pressed
if stop_flag == False:
    arr = []
    with open('data/data_right_APP.txt') as f: #'data/data_right_' + name.get() + '.txt'
        for line in f.readlines():
            if (line[0] != 'e'):
                s = line.split(' ')
                arr.append(float(s[9]))           
    Q1 = np.quantile(arr, .25)
    Q3 = np.quantile(arr, .75)
    if Q3 == Q1:
        Q3 = Q1 + 1
    print('Right Q1: '+str(Q1)+'   Q3: '+str(Q3))

    #-------------------------Map the raw foot pressure data to database data with first and third quartile---------------------------
    #---------------------------Simulation: read restored files------------------------------------
    '''
    with open('data/sample_test_right_co2.txt') as f: #'data/Quartile_Right.txt'
        i = 0
        for line in f.readlines():
            if line[0] != 'e' and i >= 0:
                s = line.split(' ')
                right_temp = []
                right_temp.append(float(s[0]))
                right_temp.append(float(s[1]))
                right_data.append(right_temp)
                #print('right_data:'+str(right_data[i-480][0])+'  '+str(right_data[i-480][1])+' i='+str(i))
            i += 1
    '''
    #----------------------------------------------------------------------------------------------
    #'''
    with open('data/data_right_APP.txt') as f: #'data/data_right_' + name.get() + '.txt'
        i = 0
        n = begin_restore_time * 100 - 100
        for line in f.readlines():
            if line[0] != 'e' and i >= n:
                s = line.split(' ')
                #total = 3.12+((float(s[9])-Q1)*832.302/(Q3-Q1))
                #print('i = '+str(i))
                total = 3.12+((arr[i]-Q1)*832.302/(Q3-Q1))
                if (total < 0):
                    total = 0
                right_temp = []
                right_temp.append(float(s[0]))
                right_temp.append(total)
                right_data.append(right_temp)
                #print('right_data:'+str(right_data[i-480][0])+'  '+str(right_data[i-480][1])+' i='+str(i))
            i += 1
    #'''
    #---------------------------------------------------------------------------------------------------------------------------------

    data_right = '1 1 1 1 1 1 1 1 1 1 1 1 1 1 1'
    right_data_ready = True
    print('Right foot data receiving done!')
#'''
#------------------------------------------------------------------
    
#Show result thread
if stop_flag == False:
    thread_result = threading.Thread(target=show_result)
    thread_result.start()
else:
    #Initialize parameters
    left_data_ready = False
    right_data_ready = False
