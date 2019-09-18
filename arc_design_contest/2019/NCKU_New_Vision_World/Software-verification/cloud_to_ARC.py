# import sys
# sys.path.append("D:/openpose-master/openpose-master/build/examples/tutorial_api_python/")
from openni import openni2
import numpy as np
import sys
import cv2
import os
from sys import platform
import ftdi_pybind
import socket
import struct
import time
from threading import Thread
from playsound import playsound

# Define music to do parallel
def play_piano_C1_1():
    playsound('music/piano/C1_1.mp3')
def play_piano_C1_2():
    playsound('music/piano/C1_2.mp3')
def play_piano_C1_3():
    playsound('music/piano/C1_3.mp3')
def play_piano_C1_4():
    playsound('music/piano/C1_4.mp3')
def play_piano_C1_5():
    playsound('music/piano/C1_5.mp3')
def play_piano_C1_6():
    playsound('music/piano/C1_6.mp3')
def play_piano_C1_7():
    playsound('music/piano/C1_7.mp3')

def play_piano_C2_1():
    playsound('music/piano/C2_1.mp3')
def play_piano_C2_2():
    playsound('music/piano/C2_2.mp3')
def play_piano_C2_3():
    playsound('music/piano/C2_3.mp3')
def play_piano_C2_4():
    playsound('music/piano/C2_4.mp3')
def play_piano_C2_5():
    playsound('music/piano/C2_5.mp3')
def play_piano_C2_6():
    playsound('music/piano/C2_6.mp3')
def play_piano_C2_7():
    playsound('music/piano/C2_7.mp3')

def play_piano_C3_1():
    playsound('music/piano/C3_1.mp3')
def play_piano_C3_2():
    playsound('music/piano/C3_2.mp3')
def play_piano_C3_3():
    playsound('music/piano/C3_3.mp3')
def play_piano_C3_4():
    playsound('music/piano/C3_4.mp3')
def play_piano_C3_5():
    playsound('music/piano/C3_5.mp3')
def play_piano_C3_6():
    playsound('music/piano/C3_6.mp3')
def play_piano_C3_7():
    playsound('music/piano/C3_7.mp3')

def play_piano_C4_1():
    playsound('music/piano/C4_1.mp3')
def play_piano_C4_2():
    playsound('music/piano/C4_2.mp3')
def play_piano_C4_3():
    playsound('music/piano/C4_3.mp3')
def play_piano_C4_4():
    playsound('music/piano/C4_4.mp3')
def play_piano_C4_5():
    playsound('music/piano/C4_5.mp3')
def play_piano_C4_6():
    playsound('music/piano/C4_6.mp3')
def play_piano_C4_7():
    playsound('music/piano/C4_7.mp3')

def play_drum():
    playsound('music/drum_front.mp3')

def play_guitar():
    playsound('music/guitar.mp3')

# Import Openpose (Windows/Ubuntu/OSX)
dir_path = os.path.dirname(os.path.realpath(__file__))
try:
    # Windows Import
    if platform == "win32":
        # Change these variables to point to the correct folder (Release/x64 etc.)
        sys.path.append(dir_path + '/../../python/openpose/Release');
        os.environ['PATH']  = os.environ['PATH'] + ';' + dir_path + '/../../x64/Release;' +  dir_path + '/../../bin;'
        import pyopenpose as op
    else:
        # Change these variables to point to the correct folder (Release/x64 etc.)
        sys.path.append('../../python');
        # If you run `make install` (default path is `/usr/local/python` for Ubuntu), you can also access the OpenPose/python module from there. This will install OpenPose and the python library at your desired installation path. Ensure that this is in your python path in order to use it.
        # sys.path.append('/usr/local/python')
        from openpose import pyopenpose as op
except ImportError as e:
    print('Error: OpenPose library could not be found. Did you enable `BUILD_PYTHON` in CMake and have this Python script in the right folder?')
    raise e

# Drive Kinect
openni2.initialize()
dev = openni2.Device.open_any()
print('opening kinect', dev.get_device_info())

depth_stream = dev.create_depth_stream()
color_stream = dev.create_color_stream()
depth_stream.start()
color_stream.start()

# Custom Params (refer to include/openpose/flags.hpp for more parameters)
params = dict()
params["model_folder"] = "../../../models/"
params["hand"] = True
params["disable_multi_thread"] = True
params["number_people_max"] = 1
params["hand_detector"] = 0
params["body"] = 1

# Starting OpenPose
opWrapper = op.WrapperPython()
opWrapper.configure(params)
opWrapper.start()
datum = op.Datum()

# Make sure to connect to ARC
ftdi_pybind.init()
ftdi_pybind.send_data("vlsi", 4)
count = 0

# -----------------------camera-------------------------------
while True:
    # rgb map
    cframe = color_stream.read_frame()
    cframe_data = np.array(cframe.get_buffer_as_triplet()).reshape([480, 640, 3])
    cframe_data = cv2.cvtColor(cframe_data, cv2.COLOR_BGR2RGB)

    # read hand rectangle locations
    handRectangles = [
        # Left/Right hands person 0
        [
            op.Rectangle(320.035889, 377.675049, 69.300949, 69.300949),
            op.Rectangle(0., 0., 0., 0.),
        ],
        # Left/Right hands person 1
        [
            op.Rectangle(80.155792, 407.673492, 80.812706, 80.812706),
            op.Rectangle(46.449715, 404.559753, 98.898178, 98.898178),
        ],
        # Left/Right hands person 2
        [
            op.Rectangle(185.692673, 303.112244, 157.587555, 157.587555),
            op.Rectangle(88.984360, 268.866547, 117.818230, 117.818230),
        ]
    ]

    datum.cvInputData = cframe_data
    datum.handRectangles = handRectangles
    # Process and display image
    opWrapper.emplaceAndPop([datum])
    output_img = datum.cvOutputData

    # Draw line on the window to help view clear
    piano_dis = 19
    vertical_line_color_320 = (0, 255, 0)
    cv2.line(output_img, (320, 0), (320, 640), vertical_line_color_320, 3)
    for i in range(1, 15):
        if i % 7 == 0:
            vertical_line_color = (0, 0, 225)
        else:
            vertical_line_color = (0, 0, 0)
        cv2.line(output_img, (320 - i * piano_dis, 0), (320 - i * piano_dis, 640), vertical_line_color, 3)
        cv2.line(output_img, (320 + i * piano_dis, 0), (320 + i * piano_dis, 640), vertical_line_color, 3)

    cv2.imshow("OpenPose 1.4.0 - Tutorial Python API", output_img)
    key = cv2.waitKey(1)

# Start to recognize keypoints
    if datum.handKeypoints[1].shape == ():
        print("Cannot find people")
    elif datum.poseKeypoints[0][0][0] == 0 and datum.poseKeypoints[0][0][1] == 0 and datum.poseKeypoints[0][0][2] == 0:
        print("Cannot find body")
    elif datum.handKeypoints[0][0][0][0] == 0 and datum.handKeypoints[0][0][0][1] == 0 and datum.handKeypoints[0][0][0][2] == 0:
        print("Cannot find left hand")
    elif datum.handKeypoints[1][0][0][0] == 0 and datum.handKeypoints[1][0][0][1] == 0 and datum.handKeypoints[1][0][0][2] == 0:
        print("Cannot find right hand")
    else:
        # Send data to ARC
        output_data_list = [0 for n in range(168)]
        for i in range(0, 21):
            x = int(datum.handKeypoints[0][0][i][0])
            y = int(datum.handKeypoints[0][0][i][1])
            output_data_list[i*4] = x // 256
            output_data_list[i*4+1] = x % 256
            output_data_list[i*4+2] = y // 256
            output_data_list[i*4+3] = y % 256

        for i in range(0, 21):
            x = int(datum.handKeypoints[1][0][i][0])
            y = int(datum.handKeypoints[1][0][i][1])
            output_data_list[i*4+84] = x // 256
            output_data_list[i*4+85] = x % 256
            output_data_list[i*4+86] = y // 256
            output_data_list[i*4+87] = y % 256

        count = count + 1
        # print(count)

        # Communicate with ARC
        ftdi_pybind.send_data("vlsi", 4)
        ftdi_pybind.send_point_data(ftdi_pybind.list_foo(output_data_list))
        ftdi_pybind.send_data("OK", 2)

        # Read data from ARC
        result = ftdi_pybind.read_data()
        result2 = result & 0xffffffff
        # Ex: result2 = 3221225472: 2  / 1073741824: 0
        result2_bit_reverse = '{:032b}'.format(result2)
        # Ex: result2_bit_reverse = 1100 0000 0000 0000 0000 0000 0000 0000
        result2_bit = result2_bit_reverse[::-1]
        # Ex: result2_bit = 0000 0000 0000 0000 0000 0000 0000 0011
        print("result:", result2)

        # Piano
        if result2_bit[31] == str(0) and result2_bit[30] == str(1):
            if result2_bit[27] == str(1):
                T = Thread(target=play_piano_C4_7)
                T.start()
            if result2_bit[26] == str(1):
                T = Thread(target=play_piano_C4_6)
                T.start()
            if result2_bit[25] == str(1):
                T = Thread(target=play_piano_C4_5)
                T.start()
            if result2_bit[24] == str(1):
                T = Thread(target=play_piano_C4_4)
                T.start()
            if result2_bit[23] == str(1):
                T = Thread(target=play_piano_C4_3)
                T.start()
            if result2_bit[22] == str(1):
                T = Thread(target=play_piano_C4_2)
                T.start()
            if result2_bit[21] == str(1):
                T = Thread(target=play_piano_C4_1)
                T.start()
            if result2_bit[20] == str(1):
                T = Thread(target=play_piano_C3_7)
                T.start()
            if result2_bit[19] == str(1):
                T = Thread(target=play_piano_C3_6)
                T.start()
            if result2_bit[18] == str(1):
                T = Thread(target=play_piano_C3_5)
                T.start()
            if result2_bit[17] == str(1):
                T = Thread(target=play_piano_C3_4)
                T.start()
            if result2_bit[16] == str(1):
                T = Thread(target=play_piano_C3_3)
                T.start()
            if result2_bit[15] == str(1):
                T = Thread(target=play_piano_C3_2)
                T.start()
            if result2_bit[14] == str(1):
                T = Thread(target=play_piano_C3_1)
                T.start()
            if result2_bit[13] == str(1):
                T = Thread(target=play_piano_C2_7)
                T.start()
            if result2_bit[12] == str(1):
                T = Thread(target=play_piano_C2_6)
                T.start()
            if result2_bit[11] == str(1):
                T = Thread(target=play_piano_C2_5)
                T.start()
            if result2_bit[10] == str(1):
                T = Thread(target=play_piano_C2_4)
                T.start()
            if result2_bit[9] == str(1):
                T = Thread(target=play_piano_C2_3)
                T.start()
            if result2_bit[8] == str(1):
                T = Thread(target=play_piano_C2_2)
                T.start()
            if result2_bit[7] == str(1):
                T = Thread(target=play_piano_C2_1)
                T.start()
            if result2_bit[6] == str(1):
                T = Thread(target=play_piano_C1_7)
                T.start()
            if result2_bit[5] == str(1):
                T = Thread(target=play_piano_C1_6)
                T.start()
            if result2_bit[4] == str(1):
                T = Thread(target=play_piano_C1_5)
                T.start()
            if result2_bit[3] == str(1):
                T = Thread(target=play_piano_C1_4)
                T.start()
            if result2_bit[2] == str(1):
                T = Thread(target=play_piano_C1_3)
                T.start()
            if result2_bit[1] == str(1):
                T = Thread(target=play_piano_C1_2)
                T.start()
            if result2_bit[0] == str(1):
                T = Thread(target=play_piano_C1_1)
                T.start()
            else:
                nonono = 0
        # Drum
        elif result2_bit[31] == str(1) and result2_bit[30] == str(0):
            if result2_bit[29] == str(1) or result2_bit[28] == str(1):
                T = Thread(target=play_drum)
                T.start()
            else:
                nonono = 0
        # Guitar
        elif result2_bit[31] == str(1) and result2_bit[30] == str(1):
            if result2_bit[28] == str(1):
                T = Thread(target=play_guitar)
                T.start()
            else:
                nonono = 0
        else:
            nonono = 0
# close app
color_stream.stop()
dev.close()

