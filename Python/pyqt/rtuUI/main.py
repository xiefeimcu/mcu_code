
__author__ = 'xiefei'

import sys
import serial
import binascii
import time
from  PyQt5 import QtWidgets,QtGui,QtCore
from rtuUI import Ui_Form
from  rtuMsg import  messag
import threading
import urllib.request
import serial.tools.list_ports

import pyautogui as pag

class myWindow(QtWidgets.QWidget,Ui_Form):
    ser = serial.Serial()
    rxBuf = []
    rxBuf = []

    rtuArg = []

    def __init__(self):
        super(myWindow,self).__init__()
        self.setupUi(self)

    def check_port(self):
        Com_list = []
        print("check")
        port_list = list(serial.tools.list_ports.comports())
        self.comboBox.clear()
        for port in port_list:
            Com_list.append(port[0])
            self.comboBox.addItem(port[0])
        if(len(Com_list) == 0):
            self.label.setText("无可用串口")

    def read_data(self):
        print("The receive_data threading is start")
        num = 0

        while (self.ser.isOpen()):
            size = self.ser.inWaiting()
            if size:
                self.rxBuf= self.ser.read_all()
                self.textBrowser.append(binascii.b2a_hex( self.rxBuf).decode())
                self.textBrowser.moveCursor(QtGui.QTextCursor.End)
                self.ser.flushInput()

    def open_port(self):
        if(self.ser.is_open):
            return

        print("open")
        self.ser.port = self.comboBox.currentText()
        self.ser.baudrate = int(self.comboBox_2.currentText())
        self.ser.bytesize = int(self.comboBox_4.currentText())
        self.ser.stopbits = int(self.comboBox_3.currentText())
        #self.ser.parity = self.comboBox_5.currentText()
        try:
           self.ser.open()
        except IOError:
            print("faild")
            return

        if(self.ser.is_open):
            self.label.setText("打开成功")
            t1 = threading.Thread(target=self.read_data)
            t1.setDaemon(True)
            t1.start()
        else:
            self.label.setText("打开失败")
            return

    def close_port(self):
        print(sys.path)
        if(self.ser.is_open == False):
            return

        self.ser.close()
        print("close")
        self.ser.close()
        self.label.setText("已关闭")

    def add(self,num):
        return num + 1

    def get_arg_data(self):
        self.rtuArg.clear()
        for idx in range(0 ,self.tableWidget_1.rowCount()) :
            item = QtWidgets.QTableWidgetItem()
            item = self.tableWidget_1.item(idx, 0)
            self.rtuArg.append(int(item.text()))

        for idx in range(0 ,self.tableWidget_2.rowCount()) :
            item = QtWidgets.QTableWidgetItem()
            item = self.tableWidget_2.item(idx, 0)
            self.rtuArg.append(int(item.text()))

        for idx in range(0 ,self.tableWidget_3.rowCount()) :
            item = QtWidgets.QTableWidgetItem()
            item = self.tableWidget_3.item(idx, 0)
            self.rtuArg.append(int(item.text()))

    def write_rtu(self):
        messag.getSystime(messag)
        return


        # 判断串口是否打开
        # if(self.ser.is_open == False):
        #     self.textBrowser.append('Write Fail Because The Comm Is Not Open !')
        #     self.textBrowser.moveCursor(QtGui.QTextCursor.End)
        #     return

        # # 得到表格内的数据
        # self.get_arg_data()
        # # 将参配置数据通过串口发送给RTU
        # tempList ='bgbg1' + str(self.tableWidget_1.rowCount()) + self.rtuArg[0:self.tableWidget_1.rowCount()] + "eded"
        # # self.ser.write(tempList)
        # self.textBrowser.append('Write :' + str(tempList))
        # self.textBrowser.moveCursor(QtGui.QTextCursor.End)
        #
        # tempList ='bgbg2' + str(self.tableWidget_2.rowCount()) + self.rtuArg[0:self.tableWidget_2.rowCount()] + "eded"
        # # self.ser.write(tempList)
        # self.textBrowser.append('Write :' + str(tempList))
        # self.textBrowser.moveCursor(QtGui.QTextCursor.End)
        #
        # tempList ='bgbg3' + str(self.tableWidget_3.rowCount()) + self.rtuArg[0:self.tableWidget_3.rowCount()] + "eded"
        # # self.ser.write(tempList)
        # self.textBrowser.append('Write :' + str(tempList))
        # self.textBrowser.moveCursor(QtGui.QTextCursor.End)

    def moveTable(self):

        print(pag.position())

    def read_rtu(self):
        print("read_rtu")


if __name__ == '__main__':
    '''
    主函数
    '''
    app = QtWidgets.QApplication(sys.argv)
    window = myWindow()
    window.show()
    sys.exit(app.exec_())
