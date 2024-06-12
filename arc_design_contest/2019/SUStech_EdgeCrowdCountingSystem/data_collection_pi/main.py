import time
import cv2
import os
import sys
from itertools import cycle
import tensorflow as tf
import numpy as np
import socket


def conv2d(x, w):
    return tf.nn.conv2d(x, w, strides=[1, 1, 1, 1], padding='SAME')


def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1],
                          strides=[1, 2, 2, 1], padding='SAME')


def inf(x):
    # tf.reset_default_graph()
    # s net ###########################################################
    w_conv1_1 = tf.get_variable('w_conv1_1', [5, 5, 1, 24])
    b_conv1_1 = tf.get_variable('b_conv1_1', [24])
    h_conv1_1 = tf.nn.relu(conv2d(x, w_conv1_1) + b_conv1_1)

    h_pool1_1 = max_pool_2x2(h_conv1_1)

    w_conv2_1 = tf.get_variable('w_conv2_1', [3, 3, 24, 48])
    b_conv2_1 = tf.get_variable('b_conv2_1', [48])
    h_conv2_1 = tf.nn.relu(conv2d(h_pool1_1, w_conv2_1) + b_conv2_1)

    h_pool2_1 = max_pool_2x2(h_conv2_1)

    w_conv3_1 = tf.get_variable('w_conv3_1', [3, 3, 48, 24])
    b_conv3_1 = tf.get_variable('b_conv3_1', [24])
    h_conv3_1 = tf.nn.relu(conv2d(h_pool2_1, w_conv3_1) + b_conv3_1)

    w_conv4_1 = tf.get_variable('w_conv4_1', [3, 3, 24, 12])
    b_conv4_1 = tf.get_variable('b_conv4_1', [12])
    h_conv4_1 = tf.nn.relu(conv2d(h_conv3_1, w_conv4_1) + b_conv4_1)

    # m net ###########################################################
    w_conv1_2 = tf.get_variable('w_conv1_2', [7, 7, 1, 20])
    b_conv1_2 = tf.get_variable('b_conv1_2', [20])
    h_conv1_2 = tf.nn.relu(conv2d(x, w_conv1_2) + b_conv1_2)

    h_pool1_2 = max_pool_2x2(h_conv1_2)

    w_conv2_2 = tf.get_variable('w_conv2_2', [5, 5, 20, 40])
    b_conv2_2 = tf.get_variable('b_conv2_2', [40])
    h_conv2_2 = tf.nn.relu(conv2d(h_pool1_2, w_conv2_2) + b_conv2_2)

    h_pool2_2 = max_pool_2x2(h_conv2_2)

    w_conv3_2 = tf.get_variable('w_conv3_2', [5, 5, 40, 20])
    b_conv3_2 = tf.get_variable('b_conv3_2', [20])
    h_conv3_2 = tf.nn.relu(conv2d(h_pool2_2, w_conv3_2) + b_conv3_2)

    w_conv4_2 = tf.get_variable('w_conv4_2', [5, 5, 20, 10])
    b_conv4_2 = tf.get_variable('b_conv4_2', [10])
    h_conv4_2 = tf.nn.relu(conv2d(h_conv3_2, w_conv4_2) + b_conv4_2)

    # l net ###########################################################
    w_conv1_3 = tf.get_variable('w_conv1_3', [9, 9, 1, 16])
    b_conv1_3 = tf.get_variable('b_conv1_3', [16])
    h_conv1_3 = tf.nn.relu(conv2d(x, w_conv1_3) + b_conv1_3)

    h_pool1_3 = max_pool_2x2(h_conv1_3)

    w_conv2_3 = tf.get_variable('w_conv2_3', [7, 7, 16, 32])
    b_conv2_3 = tf.get_variable('b_conv2_3', [32])
    h_conv2_3 = tf.nn.relu(conv2d(h_pool1_3, w_conv2_3) + b_conv2_3)

    h_pool2_3 = max_pool_2x2(h_conv2_3)

    w_conv3_3 = tf.get_variable('w_conv3_3', [7, 7, 32, 16])
    b_conv3_3 = tf.get_variable('b_conv3_3', [16])
    h_conv3_3 = tf.nn.relu(conv2d(h_pool2_3, w_conv3_3) + b_conv3_3)

    w_conv4_3 = tf.get_variable('w_conv4_3', [7, 7, 16, 8])
    b_conv4_3 = tf.get_variable('b_conv4_3', [8])
    h_conv4_3 = tf.nn.relu(conv2d(h_conv3_3, w_conv4_3) + b_conv4_3)

    # merge ###########################################################
    h_conv4_merge = tf.concat([h_conv4_1, h_conv4_2, h_conv4_3], 3)

    w_conv5 = tf.get_variable('w_conv5', [1, 1, 30, 1])
    b_conv5 = tf.get_variable('b_conv5', [1])
    h_conv5 = conv2d(h_conv4_merge, w_conv5) + b_conv5

    y_pre = h_conv5

    return y_pre


graph1 = tf.Graph()
with graph1.as_default():
    x = tf.placeholder(tf.float32, [None, None, None, 1], name="input")
    y_act = tf.placeholder(tf.float32, [None, None, None, 1])
    y_pre = inf(x)

frame_path = './data/'
filenames = os.listdir(frame_path)
img_iter = cycle([cv2.imread(os.sep.join([frame_path, name]), 0)
                 for name in filenames])

key = 0
cv2.namedWindow('image', cv2.WINDOW_NORMAL)
cv2.resizeWindow('image', 640, 480)

SEVER_ADDR = ('192.168.31.67', 80)

client_socket = socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(SEVER_ADDR)
print('connect sever(%s) success' % str(SEVER_ADDR))

with tf.Session(graph=graph1) as sess:
    saver = tf.train.Saver()
    saver.restore(sess, 'modelA/model.ckpt')

    while key & 0xFF != 28:
        img = next(img_iter)
        img = np.array(img)
        img = (img-127.5)/128
        data = []
        data.append([img])
        d = data[0]
        x_in = d[0]
        x_in = np.reshape(d[0], (1, d[0].shape[0], d[0].shape[1], 1))
        timea = time.time()
        y_p_den = sess.run(y_pre, feed_dict={"input:0": x_in})
        timeb = time.time()
        cv2.imshow('image', img)

        tm = time.strftime("%Y%m%d%H%M%S", time.localtime())
        real_num = "{:0>4d}".format(np.sum(y_p_den))
        send_data = 'A'+real_num+tm
        client_socket.send(send_data.encode())

        print("prediction result:%f" % real_num)
        print('send message %s' % send_data)
        print('=============================================================')
        print('cost time: %.2f s' % (timea - timeb))

        sys.stdout.flush()
        key = cv2.waitKey(1000)
