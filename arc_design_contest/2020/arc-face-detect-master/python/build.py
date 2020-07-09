from serial import Serial, EIGHTBITS, PARITY_NONE, STOPBITS_ONE
from time import sleep
import time
import sys
import cv2
import matplotlib.pyplot as plt
import numpy as np
from skimage import exposure
import os
import json
import tensorflow as tf
from keras.models import Model
from sklearn.metrics.pairwise import cosine_similarity
from model_hercules import Center_mFCN3_relu_max2_WID_fz1_2
COM_PORT = 'COM3'
#COM_PORT = '/dev/serial/by-id/usb-Digilent_Digilent_USB_Device_251642542476-if00-port0'
#BAUD_RATES = 115200
#BAUD_RATES = int(sys.argv[1])
model_path = 'lcfr.hdf5'
input_shape = (64, 64, 1)
model = Center_mFCN3_relu_max2_WID_fz1_2(input_shape=input_shape, classes=10582, bn_axis=-1, isCenterloss=False)
model.load_weights(model_path, by_name=True)
FC1_vector = Model(model.input, model.get_layer('new_FV').output)


def main():
	global file_name
	file_name = sys.argv[1]


if __name__ == "__main__":
	main()
#preprae database
with open('database.json') as f:
    cha_arr = json.load(f)
if file_name in cha_arr:
	print('\n')
	print('+------------------------------------------------------------+')
	print('|   Name already exist. The program has been terminated :(   |')
	print('+------------------------------------------------------------+')
	sys.exit(0)
s = Serial(
		port='COM3',
		baudrate=1000000,
		bytesize=EIGHTBITS,
		parity=PARITY_NONE,
		stopbits=STOPBITS_ONE,
		xonxoff=False,
		rtscts=False
	)
print("Configured")
while True:
	#mcu_feedback = s.readline().decode()
	# mcu_feedback = ser.read(1)
	b1, b2, b3 = b'\x00', b'\x00', b'\x00'
	RGB = True
	
	img = np.zeros((64, 64, 3)).astype(np.uint8)

	while True:
		b1, b2, b3 = b2, b3, s.read(1)

		if b1 == b'R' and b2 == b'D' and b3 == b'Y':
			start = time.time();
			print("GET")
			cnt = 0
			while True:
				if(not RGB):
					b1 = s.read(1)
					b2 = s.read(1)
					b1i = int.from_bytes(b1, byteorder='little')
					b2i = int.from_bytes(b2, byteorder='little')
					R = b1i // 8
					tmp = b1i % 8
					G = tmp * 8 + b2i // 32
					B = b2i % 32
				else:
					b1 = s.read(1)
					b2 = s.read(1)
					b3 = s.read(1)
					R = int.from_bytes(b1, byteorder='little')
					G = int.from_bytes(b2, byteorder='little')
					B = int.from_bytes(b3, byteorder='little')


				#print(R, G, B)
                # print(b1, b2);

				img[cnt // 64][cnt % 64][2] = np.clip(R * 8, 0, 255)
				img[cnt // 64][cnt % 64][1] = np.clip((R+B)*4, 0, 255)
				img[cnt // 64][cnt % 64][0] = np.clip(B * 8, 0, 255)
				
				cnt = cnt +1
				if cnt == 64 * 64:
					break
			imgs = []
			img3 = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
			cv2.imwrite('color_img.jpg', img)
			
			#img2 = exposure.equalize_adapthist(img, clip_limit=0.03)
			img2 = cv2.resize(img3, (240, 240))
			cv2.imshow("Image", img2)
			#start build
			if( cv2.waitKey(10) >= 0 ):
				img3 = (img3-127.5)/127.5
				img3 = np.reshape(img3, [64,64,1])
				imgs.append(img3)
				imgs = np.array(imgs)
				r = FC1_vector.predict(imgs)
				r = r[0][0][0].tolist()
				cha_arr[file_name] = r
				print('\n')
				print('Input_file:', file_name)
				print('Data imported successfully :)')
				print('(Face number in database now:' + str(len(cha_arr)) + ')')
				json.dump(cha_arr, open('database.json', 'w'), indent=4)
			cv2.waitKey(1)
			#print("frame done \n")
			
			b = s.read(1)
			'''
			bi = int.from_bytes(b, byteorder='little')
			if(bi>0):
				print("Face detected\n")
			else:
				print("no face\n")
			'''
	#print(mcu_feedback,)

