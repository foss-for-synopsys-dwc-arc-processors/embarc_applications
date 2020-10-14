import tkinter as tk
import time
import threading
import re
from tkinter import messagebox
import random
import serial
from time import sleep

on_hit = False

class Application(tk.Tk):
    def __init__(self):
        super().__init__()
        COM_PORT = '/dev/serial/by-id/usb-Digilent_Digilent_USB_Device_251642542476-if00-port0'
        BAUD_RATES = 115200
        #self.ser = serial.Serial(COM_PORT, BAUD_RATES)


        self.title('家居助手')
        self.createUI()

    def createUI(self):
        tk.Button(self, text='开始', command=lambda :self.thread_it(self.start, 1)).pack(expand=True, side=tk.LEFT)
        self.text = tk.Text(self)
        self.scroll = tk.Scrollbar()
        self.scroll.pack(side=tk.RIGHT,fill=tk.Y)
        self.text.pack(side=tk.LEFT,fill=tk.Y)
        self.scroll.config(command=self.text.yview)
        self.text.config(yscrollcommand=self.scroll.set)

    def start(self, songs):
        global on_hit
        if on_hit == False:
            on_hit = True
            self.ser=serial.Serial("COM9",9600,timeout=0.5)#using serial port: winsows com1
            while(on_hit):
                    self.update()



                    data = self.ser.readline().decode()
                    para = data.split(',')
                    self.text.insert(tk.END, "跌倒概率：%s \t-- %s\n" %(para[0], time.ctime()))
                    self.text.see('end')
                    if re.search('fall',data):
                        messagebox.showwarning('Warning', 'someone fall')

        else:
            on_hit = False
            self.ser.close()#close serial port
            #time.sleep(5)
            #self.var.set('')#var.set('')

    @staticmethod
    def thread_it(func, *args):
        t = threading.Thread(target=func, args=args)
        t.setDaemon(True)
        t.start()
        # t.join()


app = Application()
app.mainloop()
