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
            res = np.squeeze(res.data.max(1)[1].numpy(), axis = 0)
            #res[:3,:] = 0
            #res[:, :3] = 0
            #res[:, 61:] = 0 
            pix_sum = np.sum(res,dtype = np.int32)
            print(pix_sum)
            #distance mode
            a = 2300
            b = 32 #cm
            distance = 1/np.sqrt(pix_sum/a) * b
            
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
            return img, distance, pix_sum
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
                    seg, distance, pix_sum = predDecoder()
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
            
            warn = "Safe Driving..."
            if pix_sum > 2400:
                warn = "Warning! Car Approaching!"
                cv2.putText(img3, warn, (5, 590), cv2.FONT_HERSHEY_TRIPLEX, 1, (20, 20, 255), 1, cv2.LINE_AA)
            else:
                cv2.putText(img3, warn, (5, 590), cv2.FONT_HERSHEY_TRIPLEX, 1, (20, 255, 20), 1, cv2.LINE_AA)

            text = "Distance: " + str("{0:0.2f}".format(distance)) + "cm"
            cv2.putText(img3, text, (5, 615), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (20, 120, 120), 1, cv2.LINE_AA)

            cv2.imshow("Combined", img3)

            cv2.waitKey(1)

if __name__ == "__main__":
    main()
