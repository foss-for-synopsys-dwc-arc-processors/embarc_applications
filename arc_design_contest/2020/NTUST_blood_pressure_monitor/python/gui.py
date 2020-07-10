#coding=utf-8
import sys

from PyQt5.QtWidgets import *
import numpy as np
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import random
from PyQt5.QtSql import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from serial import Serial
import struct
import time
'''pip3 install py-ecg-detectors'''
from ecgdetectors import Detectors
fs=100
detectors = Detectors(fs)
user = []
he = []
we = []

name=[]
capital=[]
ecg=[]
ppg=[]
num=0
ll=[]
time1=[]
index=[]
nn=[]
hr=[]
bp=[]
y=[]
num0=0
def sendMatrix1(matrix):
    row=matrix.shape[0]
    ser_arc.write('ZXXX'.encode())
    ser_arc.write(struct.pack('ii',row,row))
    print(struct.pack('ii',row,row));
    ser_arc.write('XXZX'.encode())
    print(row)
    for i in range(row):
        ser_arc.write(struct.pack('f',matrix[i]))##############row col to i j
    ser_arc.write('ZZZX'.encode())
def cut_ecg(content,no):
    global y
    r_peaks = detectors.two_average_detector(content)
    temp=[]
    f=0
    for r in r_peaks:
        if((r+34)>1500):
            continue
        elif((r-18)<1):
            continue
        else:
            #plt.plot(content[r-18:r+34])
            y.append(no)
            if(f==0):
                f=1
                temp=content[r-18:r+34]
            else:
                temp=np.vstack((temp,content[r-18:r+34]))
    return temp

def str2num(str1):
    temp=[]
    try:
        temp=int(str1)
    except:
        temp=str1
        try:
            temp=float(str1)
        except:
            temp=str1
    return temp

def str_read(filename):
# Try to read a txt file and return a list.Return [] if there was a mistake.
    try:
        file = open(filename,'r')
    except IOError:
        error = []
        return error
    content = file.readlines()
    file.close()
    for i in range(len(content)):
        content[i] = content[i][:len(content[i])-1]
    return content

def text_save(content,filename,mode='a'):
    # Try to save a list variable in txt file.
    temp=np.array(content).shape
    file = open(filename,mode)
    for i in range(temp[0]):
        try:
            for j in range(temp[1]):
                file.write(str(content[i][j])+',')
            file.write('\n')
        except:
            file.write(str(content[i])+',')   
    file.close()

def text_read(filename):
# Try to read a txt file and return a list.Return [] if there was a mistake.
    try:
        file = open(filename,'r')
    except IOError:
        error = []
        return error
    content = file.readlines()
    file.close()
    for i in range(len(content)):
        content[i] = content[i].split(',')
        content[i] = content[i][:len(content[i])-1]
        for j in range(len(content[i])):
            try:
                temp=int(content[i][j])
            except:
                temp=content[i][j]
                try:
                    temp=float(content[i][j])
                except:
                    temp=content[i][j]
            content[i][j]=temp
    return content


def file2list(filename):  
    fr = open(filename)
    array = fr.readlines() #以檔案中的每行為一個元素，形成一個list列表  
    num = len(array)  
    returnMat = np.zeros((num,1))#初始化元素為0的，行號數個列表，其中每個元素仍是列表，元素數是3，在此表示矩陣  
    index = 0   
    for line in array:  
        line = line.strip()#去掉一行後的回車符號  
        linelist = line.split(' ')#將一行根據分割符,劃分成多個元素的列表  
        returnMat[index,:] = linelist[1]#向矩陣賦值，注意這種賦值方式比較笨拙  
        index +=1  
    return returnMat

def bytesToFloat(h1,h2,h3,h4):
    ba = bytearray()
    ba.append(h1)
    ba.append(h2)
    ba.append(h3)
    ba.append(h4)
    return struct.unpack("!f",ba)[0]

def sendMatrix(matrix):
    row=matrix.shape[0]
    col=matrix.shape[1]
    ser_arc.write('ZXXX'.encode())
    ser_arc.write(struct.pack('ii',row,col))
    print(struct.pack('ii',row,col));
    ser_arc.write('XXZX'.encode())
    print(row)
    print(col)
    for i in range(row):
        for j in range(col):
            ser_arc.write(struct.pack('f',matrix[i][j]))##############row col to i j
    ser_arc.write('ZZZX'.encode())


class regist(QWidget):
    def __init__(self):
        super().__init__()
        self.left = 10
        self.top = 10
        self.width = 600
        self.height = 600
        self.initUi()
        self.all_data=[]

    def initUi(self):
        self.setGeometry(self.left, self.top, self.width, self.height)
    def plot(self):
        self.name_label = QLabel('Name',self)
        self.name_edit = QLineEdit("Name",self)
        self.name_label.resize(200,40)
        self.name_label.move(150,100)
        self.name_edit.resize(200,40)
        self.name_edit.move(250,100)
        self.gender_label = QLabel('Gender',self)
        self.gender_edit = QComboBox(self)
        gender=["Male","Famale"]
        self.gender_edit.addItems(gender)
        self.gender_label.resize(200,40)
        self.gender_label.move(150,200)
        self.gender_edit.resize(200,40)
        self.gender_edit.move(250,200)
        self.height_label = QLabel('Height',self)
        self.height_edit = QLineEdit("Height",self)
        self.height_label.resize(200,40)
        self.height_label.move(150,300)
        self.height_edit.resize(200,40)
        self.height_edit.move(250,300)
        self.weight_label = QLabel('Weight',self)
        self.weight_edit = QLineEdit("Weight",self)
        self.weight_label.resize(200,40)
        self.weight_label.move(150,400)
        self.weight_edit.resize(200,40)
        self.weight_edit.move(250,400)
        self.bt1 =  QPushButton('REGIST',self)
        self.bt2 =  QPushButton('QUIT',self)
        self.bt1.move(150,500)
        self.bt2.move(250,500)
        self.bt1.clicked.connect(self.reg)
        self.bt2.clicked.connect(self.quit)

        self.show()
    def reg(self):
        global name,capital,ecg,ppg,num,ll,time1,index,nn,hr,bp,num0
        print(self.name_edit.text())
        if (self.name_edit.text()!='Name') & (self.name_edit.text()!=' '):
            na=[[self.name_edit.text()],[num0]] #eg.[['aaa'],[0]]
            
            name=np.append(name,na,axis=1)
            '''
            gender0= #get gender :male=0,female=1
            height0=self.height_edit.text()
            weight0=weight_edit.text()
            te=[gender0,height0,weight0]
            capital.append(te)
            '''
            # user.append(self.name_edit.text())
            # he.append(self.height_edit.text())
            # we.append(self.weight_edit.text())
            self.close()

    def quit(self):
        self.close()
class SubWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUi()
        self.all_data=[]

    def initUi(self):
        global name,capital,ecg,ppg,num,ll,time1,index
        self.m = PlotCanvas1(self, width=10, height=6)
        self.setWindowTitle('TableWidget Usage')
        self.table = QTableWidget()
        self.table.setColumnCount(3)
        self.table.setRowCount(10)
        #self.table.setHorizontalHeaderLabels(horizontalHeader)
        self.table.setEditTriggers(QTableWidget.NoEditTriggers)
        #self.table.setSelectionBehavior(QTableWidget.SelectColumns)
        self.table.setSelectionMode(QTableWidget.SingleSelection  )
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setColumnWidth(4,200)
        self.table.setRowHeight(0,40)
        #self.table.setFrameShape(QFrame.HLine)#設定樣式
        #self.table.setShowGrid(False) #取消網格線
        self.table.verticalHeader().setVisible(False) #隱藏垂直表頭
        self.table.setHorizontalHeaderLabels(['時間','心率','血壓'])
        
        vbox = QVBoxLayout()
        vbox.addStretch(1)
        vbox.addWidget(self.m)
        vbox.addWidget(self.table)
        hbox = QHBoxLayout()
        hbox.addStretch(1)
        hbox.addLayout(vbox)
        self.setLayout(hbox)
        self.table.cellClicked.connect(self.Click)
    def showEvent(self,e):
        i=0
        global name,capital,ecg,ppg,num,ll,time1,index,nn,hr,bp
        self.table.setRowCount(len(index))
        for j in index:
            self.table.setItem(i,0,QTableWidgetItem(time1[j]))
            self.table.setItem(i,1,QTableWidgetItem(str(hr[0][j])))
            self.table.setItem(i,2,QTableWidgetItem(str(bp[0][j])))
            print(time1[0])
            i=i+1
        self.table.resizeColumnsToContents()
        self.table.resizeRowsToContents()
    def Click(self,row,col):
        print(row)
        print(col)
        global name,capital,ecg,ppg,num,ll,time1,index
        #self.current_data=ecg[index[row]]
        # self.table.setItem(0,0,QTableWidgetItem('%d'%(col)))
        # print(self.current_data)
        self.m.ax.cla()
        #self.m.ax.plot(self.current_data, 'r-')
        self.m.ax.plot(ecg[index[row]], 'r-')
        self.m.ax1.cla()
        self.m.ax1.plot(ppg[index[row]], 'r-')
        self.m.draw()
        self.show()
    def plot(self):
        self.show()
        
#class waiting():
    

class App(QMainWindow):

    def __init__(self):
        super().__init__()
        self.left = 10
        self.top = 10
        self.title = 'PyQt5 matplotlib example - pythonspot.com'
        self.width = 1600
        self.height = 900
        self.usernum = 0
        self.current_data=[]
        self.sub_window = SubWindow()
        self.r = regist()
        self.waiting = regist()
        self.initUI()
    def update(self):
        self.m.ax.axes.cla()
        self.m.ax1.axes.cla()
        self.m.ax2.axes.cla()
        self.m.draw()
    def start(self):
        global name,capital,ecg,ppg,num,ll,time1,index,nn,hr,bp
        self.usernum = self.usernum
        # user.append(self.edit.text()) 
        # self.label.setText(self.edit.text())
        # self.edit.setText("Name")
        # self.r.plot()
        COM_PORT = 'COM3'  
        BAUD_RATES = 115200  
        ser_ecg = Serial(COM_PORT, BAUD_RATES,bytesize=8)   
        
        global ser_arc
        ser_arc = Serial('COM6',115200,bytesize=8)
        plus=[]
        ##################開始量測
        outputdata=[]
        count=0
        r_peak=[]
        
        ##try:
        while True:
            f=0
            while ser_ecg.in_waiting:         
                data_raw = ser_ecg.read(size=8)  
                ecg_data=bytesToFloat(data_raw[3],data_raw[2],data_raw[1],data_raw[0])
                ppg_data=bytesToFloat(data_raw[7],data_raw[6],data_raw[5],data_raw[4])
                temp=np.vstack((ecg_data,ppg_data))
                print(temp);
                if(count==0):
                    outputdata=temp
                    count=1
                elif(count<1500):       
                    outputdata=np.hstack((outputdata,temp))
                    count+=1
                else:
                    ser_arc.write('AAXX'.encode())      #指令碼1
                    f=1
                    break
            if(f==1):
                break
        
        while True:
            f=0
            while ser_arc.in_waiting:
                data_raw1 = ser_arc.read(size=4)#######換名子
                print(data_raw1)
                data=data_raw1.decode()
                print(data)
                if(data=='XXAA'):                       #確認碼1
                    sendMatrix(outputdata)
                    r_peaks = detectors.two_average_detector(outputdata[0,:])
                    #print(r_peaks)
                    asd=0
                    for ww in range(1500):
                        if(ww==r_peaks[asd]):
                            r_peak.append(1)
                            if((len(r_peaks)-1)==asd):
                                asd=asd
                            else:
                                asd=asd+1
                        else:
                            r_peak.append(0)
                    rr=np.array(r_peak)
                    sendMatrix1(rr)
                    f=1
                    break
                else:
                    print('確認碼錯誤')
                    f=-1
                    break
            if(f==1):
                break
        
        while True:
            f=0
            while ser_arc.in_waiting:
                data_raw = ser_arc.read(size=4)
                data=data_raw.decode()
                print('send')
                print(data)
                if(data=='BBXX'):                       #指令碼2
                    ser_arc.write('XXBB'.encode())      #確認碼2  
                    print('send')         
                    f=1
                    break
            if(f==1):
                break
            
        while True:
            f=0
            while ser_arc.in_waiting:
                ##################################接收ARC回傳身分、心跳、血壓
                print('ok')
                data_raw2 = ser_arc.read(size=4)   # 讀取一行
                #data3=data_raw2.decode()
                
                data2=bytesToFloat(data_raw2[3],data_raw2[2],data_raw2[1],data_raw2[0])
                print(data2)
                plus.append(data2)
                print(len(plus))
                if(len(plus)==3):
                      # plt.plot(plus[0:1499])
                      # plt.plot(plus[1500:2999])
                      # plt.plot(plus[3001:4499])
                      # plt.show()
                      f=1
                      break
                # if(len(plus)==2999):
                #     # plt.plot(plus[0:1499])
                #     # plt.plot(plus[1500:2999])
                #     # # plt.plot(plus[3001:4499])
                #     # # plt.plot(plus[4500:5999])
                #     # plt.show()
                #     f=1
                #     break
                
            if(f==1):
                ser_ecg.close()
                ser_arc.close()
                no0=plus[2] #身分代碼,eg.0、1、2
                global y,num0
                y=[]
                X=[]
                if(no0==9998):
                    y=[0,0,0,0,0]
                    X=[]
                    name0=np.array(name[0])
                    print(name[0])
                    name0=np.unique(name0)
                    print(name0)
                    num0=len(name0)
                    print(len(name0))
                    f=0
                    '''
                    for i in range(num0):
                        n1=name[1]==i
                        n2=name[0][n1][0]
                        cut=cut_ecg(ecg[n1][0],i)
                        if(f==0):
                            X=cut
                        else:
                            X=np.vstack((X,cut))
                    '''
                    cut=cut_ecg(outputdata[0],num0)
                    X=cut
                    
                    self.r.plot()
                    
                    hr1=plus[0] #心率
                    print(hr)
                    print(hr1)
                    hr=np.append(hr,hr1)
                    bp1=plus[1] #血壓
                    bp=np.append(bp,bp1)
                    ecg=np.append(ecg,outputdata[0])
                    ppg=np.append(ppg,outputdata[1])
                    nowtime=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
                    time1.append(nowtime)
                    
                    
                    y_c=1
                    
                    mean_vector = [] 
                    print(y_c)
                    for i in y_c:
                        mean_vector.append(np.mean(X[y == i]))
                        # print('Mean Vector class %s:%s\n' % (i, mean_vector[i]))
                        
                    """within-class scatter matrix"""
                    S_W = np.zeros((X.shape[1], X.shape[1]))
                    for i in y_c:
                        Xi = X[y == i] - mean_vector[i]
                        S_W += np.mat(Xi).T * np.mat(Xi)
                    #print('within-class scatter matrix:\n', S_W)
                     
                    """between-class scatter matrix """
                    S_B = np.zeros((X.shape[1], X.shape[1]))
                    mu = np.mean(X)  # 所有樣本平均值
                    for i in y_c:
                        Ni = len(X[y == i])
                        S_B += Ni * np.mat(mean_vector[i] - mu).T * np.mat(mean_vector[i] - mu)
                    #print('within-class scatter matrix:\n', S_B)
                    
                    eigvals, eigvecs = np.linalg.eig(np.linalg.inv(S_W) * S_B)  # 求特徵值，特徵向量
                    # np.testing.assert_array_almost_equal(np.mat(np.linalg.inv(S_W) * S_B) * np.mat(eigvecs[:, 0].reshape(4, 1)),
                    #                                      eigvals[0] * np.mat(eigvecs[:, 0].reshape(4, 1)), decimal=6, err_msg='',
                    #                                      verbose=True)
                    
                    # sorting the eigenvectors by decreasing eigenvalues
                    eig_pairs = [(np.abs(eigvals[i]), eigvecs[:, i]) for i in range(len(eigvals))]
                    eig_pairs = sorted(eig_pairs, key=lambda k: k[0], reverse=True)
                    pd = {}     #投影矩陣
                    for cc in range(len(y_c)):       #目標維度
                        if(cc==0):
                            pd=eig_pairs[cc][1].reshape(len(eig_pairs[0][1]), 1)
                        else:
                            pd=np.hstack((pd, eig_pairs[cc][1].reshape(len(eig_pairs[0][1]), 1)))
                    
                    
                    X_trans = X.dot(pd)     #降維後資料
                    mean_vector1=np.array(mean_vector)
                    mean_trans = mean_vector1.dot(pd)       #降維後類別平均
                    
                    
                    
                    COM_PORT = 'COM3'  
                    BAUD_RATES = 115200  
                    ser_ecg = Serial(COM_PORT, BAUD_RATES,bytesize=8)   
                    
                    ser_arc = Serial('COM6',115200,bytesize=8)
                    
                    
                    ser_arc.write('CCXX'.encode())      #指令碼3
                    
                    while True:
                        f=0
                        while ser_arc.in_waiting:
                            data_raw = ser_arc.read(size=4)
                            data=data_raw.decode()
                            print('send')
                            if(data=='XXCC'): 
                                print('ok')                      #確認碼3
                                sendMatrix(pd)
                                f=1
                                break
                        if(f==1):
                            break
                        
                        
                    ser_arc.write('DDXX'.encode())      #指令碼4
                    
                    while True:
                        f=0
                        while ser_arc.in_waiting:
                            data_raw = ser_arc.read(size=4)
                            data=data_raw.decode()
                            if(data=='XXDD'):                       #確認碼4
                                sendMatrix(mean_trans)
                                f=1
                                break
                        if(f==1):
                            ser_ecg.close()
                            ser_arc.close()
                            break
                if(no0!=9999):
                    n1=name[1]==no0
                    n2=name[0][n1][0]
                    na=[[n2],[int(no0)]] #eg.[['aaa'],[0]]
                    name=np.append(name,na,axis=1)
                    hr1=plus[0] #心率
                    hr=np.append(hr,hr1,axis=1)
                    bp1=plus[1] #血壓
                    bp=np.append(bp,bp1,axis=1)
                    ecg=np.append(ecg,outputdata[0],axis=0)
                    ppg=np.append(ppg,outputdata[1],axis=0)
                    nowtime=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
                    time1.append(nowtime)
                else:
                    #######################計算新的lda
                    '''X=data
                    y=lable'''
                    name0=np.array(name[0])
                    print(name[0])
                    name0=np.unique(name0)
                    print(name0)
                    num0=len(name0)
                    print(len(name0))
                    f=0
                    for i in range(num0):
                        n1=name[1]==i
                        n2=name[0][n1][0]
                        cut=cut_ecg(ecg[n1][0],i)
                        if(f==0):
                            X=cut
                        else:
                            X=np.vstack((X,cut))
                    cut=cut_ecg(outputdata[0],num0)
                    X=np.vstack((X,cut))
                    
                    self.r.plot()
                    
                    hr1=plus[0] #心率
                    hr=np.append(hr,hr1,axis=1)
                    bp1=plus[1] #血壓
                    bp=np.append(bp,bp1,axis=1)
                    ecg=np.append(ecg,outputdata[0],axis=0)
                    ppg=np.append(ppg,outputdata[1],axis=0)
                    nowtime=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
                    time1.append(nowtime)
                    
                    
                    y_c=np.unique(y)
                    
                    mean_vector = [] 
                    for i in y_c:
                        mean_vector.append(np.mean(X[y == i], axis=0))
                        # print('Mean Vector class %s:%s\n' % (i, mean_vector[i]))
                        
                    """within-class scatter matrix"""
                    S_W = np.zeros((X.shape[1], X.shape[1]))
                    for i in y_c:
                        Xi = X[y == i] - mean_vector[i]
                        S_W += np.mat(Xi).T * np.mat(Xi)
                    #print('within-class scatter matrix:\n', S_W)
                     
                    """between-class scatter matrix """
                    S_B = np.zeros((X.shape[1], X.shape[1]))
                    mu = np.mean(X, axis=0)  # 所有樣本平均值
                    for i in y_c:
                        Ni = len(X[y == i])
                        S_B += Ni * np.mat(mean_vector[i] - mu).T * np.mat(mean_vector[i] - mu)
                    #print('within-class scatter matrix:\n', S_B)
                    
                    eigvals, eigvecs = np.linalg.eig(np.linalg.inv(S_W) * S_B)  # 求特徵值，特徵向量
                    # np.testing.assert_array_almost_equal(np.mat(np.linalg.inv(S_W) * S_B) * np.mat(eigvecs[:, 0].reshape(4, 1)),
                    #                                      eigvals[0] * np.mat(eigvecs[:, 0].reshape(4, 1)), decimal=6, err_msg='',
                    #                                      verbose=True)
                    
                    # sorting the eigenvectors by decreasing eigenvalues
                    eig_pairs = [(np.abs(eigvals[i]), eigvecs[:, i]) for i in range(len(eigvals))]
                    eig_pairs = sorted(eig_pairs, key=lambda k: k[0], reverse=True)
                    pd = {}     #投影矩陣
                    for cc in range(len(y_c)):       #目標維度
                        if(cc==0):
                            pd=eig_pairs[cc][1].reshape(len(eig_pairs[0][1]), 1)
                        else:
                            pd=np.hstack((pd, eig_pairs[cc][1].reshape(len(eig_pairs[0][1]), 1)))
                    
                    
                    X_trans = X.dot(pd)     #降維後資料
                    mean_vector1=np.array(mean_vector)
                    mean_trans = mean_vector1.dot(pd)       #降維後類別平均
                    
                    
                    
                    COM_PORT = 'COM3'  
                    BAUD_RATES = 115200  
                    ser_ecg = Serial(COM_PORT, BAUD_RATES,bytesize=8)   
                    
                    ser_arc = Serial('COM6',115200,bytesize=8)
                    
                    
                    ser_arc.write('CCXX'.encode())      #指令碼3
                    
                    while True:
                        f=0
                        while ser_arc.in_waiting:
                            data_raw = ser_arc.read(size=4)
                            data=data_raw.decode()
                            print('send')
                            if(data=='XXCC'): 
                                print('ok')                      #確認碼3
                                sendMatrix(pd)
                                f=1
                                break
                        if(f==1):
                            break
                        
                        
                    ser_arc.write('DDXX'.encode())      #指令碼4
                    
                    while True:
                        f=0
                        while ser_arc.in_waiting:
                            data_raw = ser_arc.read(size=4)
                            data=data_raw.decode()
                            if(data=='XXDD'):                       #確認碼4
                                sendMatrix(mean_trans)
                                f=1
                                break
                        if(f==1):
                            ser_ecg.close()
                            ser_arc.close()
                            break
                    
                    '''except:
                        ser_ecg.close()
                        ser_arc.close()
                        print('錯誤！')
                    '''
                break
        
        '''except:
            ser_ecg.close()
            ser_arc.close()
            print('錯誤！')
        '''
    def set_table(self):
        global name,cafpital,ecg,ppg,num,ll,time1,index,nn,hr,bp
        text = self.edit.text()
        name1=np.array(name)
        ll=name1[0]==text
        nn=name1[1][ll]
        index=np.array(range(len(ll)))[ll==True]
        try:
            num=str2num(nn[0])
            f=1
        except:
            QMessageBox.warning(self,'錯誤','找不到使用者', QMessageBox.Ok)
            f=-1
        
        print(num)
        #self.model =QStandardItemModel(4,4)
        # for row in range(4):
        #     for column in range(4):
        #         item=QStandardItem('row1   %s,column1   %s'%(row,column))
        #         self.model.setItem(row,column,item)
        # self.table = QTableView(self)
        # self.table.resizeRowToContents(20)
        # self.table.resizeColumnToContents(20)
        # self.table.setModel(self.model)
        #self.sub_window.all_data=self.m.all_data
        print(self.sub_window.all_data)
        if(f==1):
            self.sub_window.plot()
    def initUI(self):
        global name,capital,ecg,ppg,num,ll,time1,index,nn,hr,bp,num0
        #text_save(name,'name.txt',mode='w')
        name=text_read('name.txt')
        name0=np.array(name[0])
        name0=np.unique(name0)
        num0=len(name0)
        
        #text_save(capital,'capital.txt',mode='w')
        capital=text_read('capital.txt')
        
        #text_save(ecg,'ecg.txt',mode='w')
        ecg=text_read('ecg.txt')
        ecg=np.array(ecg)
        
        #text_save(ppg,'ppg.txt',mode='w')
        ppg=text_read('ppg.txt')
        ppg=np.array(ppg)
        
        time1=str_read('time.txt')
        
        hr=text_read('hr.txt')
        
        bp=text_read('bp.txt')
        
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        #self.setFixedSize(1280, 640)
        self.m = PlotCanvas(self, width=10, height=6)
        self.m.move(0,0)
        self.label=QLabel('detection',self)
        self.label.move(1200,300)
        self.label.resize(100,50)
        self.edit = QLineEdit("Name",self)
        self.edit.move(1100,50)
        self.edit.resize(200,30)
        self.model =QStandardItemModel(4,10000)
        for row in range(4):
            for column in range(4):
                item=QStandardItem('row %s,column %s'%(row,column))
                self.model.setItem(row,column,item)
        self.table = QTableView(self)
        self.table.setModel(self.model)
        #self.table.hideColumn(1)
        #self.table.hideColumn(7)
        self.table.move(1000,500)
        self.table.resize(500,300)
        button = QPushButton('FIND', self)
        button.move(1100,100)
        button.resize(100,50)
        button.clicked.connect(self.set_table)
        button = QPushButton('Start measurement', self)
        button.move(1200,100)
        button.resize(100,50)
        button.clicked.connect(self.start)
        self.show()


class PlotCanvas(FigureCanvas):

    def __init__(self, parent=None, width=10, height=6, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(311)

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)
        fig.tight_layout() 
        FigureCanvas.setSizePolicy(self,
                QSizePolicy.Expanding,
                QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)
        self.plot()
        fig.tight_layout() 


    def plot(self):
        #fname = 'array_data20.out'
        #self.all_data=file2list(fname)
        data = [random.random() for i in range(25)]
        self.ax = self.figure.add_subplot(311)
        #self.ax.plot(self.all_data, 'r-')
        self.ax.set_title('ECG signal')
        data = [random.random() for i in range(25)]
        self.ax1 = self.figure.add_subplot(312)
        #self.ax1.plot(self.all_data, 'r-')
        self.ax1.set_title('PPG1 signal')
        data = [random.random() for i in range(25)]
        self.ax2 = self.figure.add_subplot(313)
        #self.ax2.plot(self.all_data, 'r-')
        self.ax2.set_title('PPG2 signal')
        self.draw()
class PlotCanvas1(FigureCanvas):

    def __init__(self, parent=None, width=10, height=6, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(311)

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)
        fig.tight_layout() 
        FigureCanvas.setSizePolicy(self,
                QSizePolicy.Expanding,
                QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)
        self.plot()
        fig.tight_layout() 


    def plot(self):
        #fname = 'array_data70.out'
        #self.all_data=file2list(fname)
        #data = [random.random() for i in range(25)]
        self.ax = self.figure.add_subplot(311)
        #self.ax.plot(self.all_data, 'r-')
        self.ax.set_title('ECG signal')
        #data = [random.random() for i in range(25)]
        self.ax1 = self.figure.add_subplot(312)
        #self.ax1.plot(self.all_data, 'r-')
        self.ax1.set_title('PPG1 signal')
        #data = [random.random() for i in range(25)]
        self.ax2 = self.figure.add_subplot(313)
        #self.ax2.plot(self.all_data, 'r-')
        self.ax2.set_title('PPG2 signal')
        self.draw()
# if __name__ == '__main__':
#     app = QApplication(sys.argv)
#     ex = App()
#     sys.exit(app.exec_())
    
if __name__ == '__main__':
    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = App()
    ui.show()
    sys.exit(app.exec_())