import time
import sys
import cv2
import matplotlib.pyplot as plt
import numpy as np
import torch.nn.functional as F
import torch

from skimage import exposure
from serial import Serial, EIGHTBITS, PARITY_NONE, STOPBITS_ONE
s = Serial(
        port='/dev/serial/by-id/usb-Digilent_Digilent_USB_Device_251642542474-if00-port0',
        #port='/dev/serial/by-id/usb-Digilent_Digilent_USB_Device_251642542476-if00-port0',
        baudrate=1000000,
        bytesize=EIGHTBITS,
        parity=PARITY_NONE,
        stopbits=STOPBITS_ONE,
        xonxoff=False,
        rtscts=False
    )

def predDecoder():
    img = np.zeros((64, 64, 3)).astype(np.uint8)
    cnt = 0;
    # print(s.readline())
    pred = []
    while True:
        b = s.read(1)
        f = int.from_bytes(b, byteorder='little')
        pred.append(f)
        cnt = cnt + 1
        if cnt == 512:
            pred = np.array(pred, dtype=float)
            # print(pred)
            pred = torch.from_numpy(pred)
            pred = pred.reshape(1,2,16,16)
            res = F.interpolate(pred, size =(64,64), mode = 'bilinear', align_corners = True)
            #print(res)
            #res = np.squeeze(F.softmax(res,dim =1).numpy(), axis = 0)
            
            #print("max: ", res.data.max(1)[0])
            #print("min: ", res.data.min(1)[0])
            
            #count car number mode
            car = 0
            res = np.squeeze(res.data.max(1)[1].numpy(), axis = 0)
            res[:20, :] = 0
            res[45:,:] = 0
            car_sum = np.sum(res,dtype = np.int32)
            one_car = 400
            two_car = 500
            three_car = 800 
            #car_num = round(np.sum(res)/one_car_pix_num)
            if car_sum >= three_car:
                print("3!!")
                car = 3
            elif car_sum >= two_car:
                print("2!!")
                car = 2
            else:
                print("1!!")
                car = 1
            print(car_sum)
            '''
            #distance mode
            a = 
            b =
            distance = np.sqrt(np.sum(res)/a) * b
            
            #res_min = np.squeeze(res.data.min(1)[0].numpy(), axis = 0)
            #res_max = np.squeeze(res.data.max(1)[0].numpy(), axis = 0)
            res = np.squeeze(res.data.max(1)[1].numpy(), axis = 0)
            #res[:20,:10] = 0
            #res[:20, 50:] = 0
            res[:20, :] = 0
            res[45:,:] = 0
            '''

            #add = res_max+res_min
            #print(add)
            #add_mask = add[:,:]> 0
            #print(add_mask)
            #res = res * add_mask
            #res = res_max[:,:] == res_min[:,:]

            for idx, row in enumerate(res):
                for jdx, e in enumerate(row):
                    if e == 0:
                        img[idx][jdx][2] = 255
                        img[idx][jdx][1] = 255
                        img[idx][jdx][0] = 255
                    else:
                        img[idx][jdx][2] = 0
                        img[idx][jdx][1] = 0
                        img[idx][jdx][0] = 255
                    # print(e, end = " ")
                # print()
            return img, car
            break;


def main():

    b1, b2, b3 = b'\x00', b'\x00', b'\x00'

    cnt = 0
    RGB = True;

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


                # print(R, G, B)
                # print(b1, b2);

                img[cnt // 64][cnt % 64][2] = np.clip(R * 8, 0, 255)
                img[cnt // 64][cnt % 64][1] = np.clip((R+B)*4, 0, 255)
                img[cnt // 64][cnt % 64][0] = np.clip(B * 8, 0, 255)

                cnt = cnt + 1
                if cnt == 64 * 64:
                    seg, car_num = predDecoder()
                    break
            cv2.imwrite('color_img.jpg', img)
            img2 = exposure.equalize_adapthist(img, clip_limit=0.03)
            img2 = cv2.resize(img2[:,:,:], (640, 640))
            seg2 = cv2.resize(seg[:,:,:], (640,640))
            print("fps = {}".format(1./(time.time()-start)))

            #cv2.imshow("Image", img2)
            #cv2.imshow("Seg", seg2)

            alpha = 0.3
            img3 = ((np.multiply(1 - alpha, img2 * 255) +
                     np.multiply(alpha, seg2))).astype(np.uint8)
            
            text = "Detect! Car Number: " + str(car_num)
            cv2.putText(img3, text, (5, 610), cv2.FONT_HERSHEY_TRIPLEX, 1, (20, 20, 120), 1, cv2.LINE_AA)

            cv2.imshow("Combined", img3)

            cv2.waitKey(1)

if __name__ == "__main__":
    main()
