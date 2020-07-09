import os
import cv2
import numpy as np
import sys
import json

#prepare database & check exist
with open('database.json') as f:
    cha_arr = json.load(f)

print("<Exist names>\n")
for key in cha_arr:
    print('=> ', key)

txt = input("User you wanna delete: ")
print('Delete: ', txt)

if txt not in cha_arr:
    print('\n')
    print('+------------------------------------------------------------+')
    print('|   Name doesnt exist. The program has been terminated :(   |')
    print('+------------------------------------------------------------+')
    sys.exit(0)


#update dict & write json file
del cha_arr[txt]
print('\n')
print('Data delete successfully :)')
print('(Face number in database now:' + str(len(cha_arr)) + ')')
json.dump(cha_arr, open('database.json', 'w'), indent=4)
