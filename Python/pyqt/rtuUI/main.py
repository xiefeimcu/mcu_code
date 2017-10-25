
__author__ = 'xiefei'

import sys
from  PyQt5 import QtWidgets

from rtuUI import Ui_Form

if __name__ == '__main__':
    '''
    主函数
    '''
    class myWindow(QtWidgets.QMainWindow,Ui_Form):
        def __init__(self):
            super(myWindow,self).__init__()
            self.setupUi(self)

        def slot1(self):
            # item = QtWidgets.QTableWidgetItem()
            # self.tableWidget.setVerticalHeaderItem(self,10, item)
            print("aaa")

    app = QtWidgets.QApplication(sys.argv)
    window = myWindow()
    window.show()
    sys.exit(app.exec_())
    # app = QApplication(sys.argv)
    # mainWindow = QMainWindow()
    # ui = Ui_Form()
    # ui.setupUi(mainWindow)
    # mainWindow.show()
