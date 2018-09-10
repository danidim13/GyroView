
# -*- coding: utf-8 -*-
from __future__ import print_function
from PyQt5 import QtCore, QtGui, QtWidgets
import numpy as np
import pyqtgraph as pg
import random
try:
    import subprocess32 as subprocess
except:
    import subprocess
import sys
from camera_dialog import Ui_Dialog

class MainWindow(QtGui.QMainWindow):

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.central_widget = QtGui.QStackedWidget()
        self.setCentralWidget(self.central_widget)
        self.plot_widget = PlotWidget(self)
        self.central_widget.addWidget(self.plot_widget)
        self.setWindowTitle("GyroView")



class PlotWidget(QtGui.QWidget):

    def __init__(self, parent=None):

        super(PlotWidget, self).__init__(parent)

        self.timer = QtCore.QTimer()
        self.data1 = curveData()
        self.data2 = curveData()


        self.init_layout()
        self.init_connections()


        self.click_flag = 0

    def init_layout(self):
        vlayout = QtGui.QVBoxLayout()

        ### Opcion 1 --> Graphics Layout Widget
        self.plotters = pg.GraphicsLayoutWidget()

        self.plot1 = self.plotters.addPlot(0,0,title="Grafica 1")
        self.plot2 = self.plotters.addPlot(1,0,title="Grafica 2")

        self.curve1 = self.plot1.plot()
        self.curve2 = self.plot2.plot()
        vlayout.addWidget(self.plotters)


        ### Opcion 2 --> Plot Widget
        # self.plot1 = pg.PlotWidget(title="Grafica 1")
        # self.plot2 = pg.PlotWidget(title="Grafica 2")
        #
        # self.curve1 = self.plot1.getPlotItem().plot()
        # self.curve2 = self.plot2.getPlotItem().plot()
        #
        #
        # vlayout.addWidget(self.plot1)
        # vlayout.addWidget(self.plot2)

        Hlayout = QtGui.QHBoxLayout()

        self.greenbutton = QtGui.QPushButton()
        self.greenbutton.setFixedSize(QtCore.QSize(24,24))
        self.greenbutton.setIcon(QtGui.QIcon("./greenbutton.jpeg"))
        self.greenbutton.setIconSize(QtCore.QSize(24,24))
        self.greenbutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")

        self.bluebutton = QtGui.QPushButton()
        self.bluebutton.setFixedSize(QtCore.QSize(24,24))
        self.bluebutton.setIcon(QtGui.QIcon("./bluebutton.jpeg"))
        self.bluebutton.setIconSize(QtCore.QSize(24,24))
        self.bluebutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")

        self.redbutton = QtGui.QPushButton()
        self.redbutton.setFixedSize(QtCore.QSize(24,24))
        self.redbutton.setIcon(QtGui.QIcon("./redbutton.jpeg"))
        self.redbutton.setIconSize(QtCore.QSize(24,24))
        self.redbutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")

        Hlayout.addWidget(self.greenbutton)
        Hlayout.addWidget(self.bluebutton)
        Hlayout.addWidget(self.redbutton)

        vlayout.addLayout(Hlayout)

        self.setLayout(vlayout)

    def init_connections(self):

        self.redbutton.clicked.connect(self.start_plotter)
        self.bluebutton.clicked.connect(self.calibrator)
        self.greenbutton.clicked.connect(self.run_camera)
        self.timer.timeout.connect(self.updater)

    def start_plotter(self):

        if self.click_flag == 0:
            self.timer.start(33)
            self.click_flag = 1
        else:
            self.timer.stop()
            self.click_flag = 0
            self.data1.clear()
            self.data2.clear()

    def updater(self):
        time1, data1 = self.data1.updateCurve()
        time2, data2 = self.data2.updateCurve()
        self.curve1.setData(data1)
        self.curve2.setData(data2)
        self.curve1.setPos(time1,0)
        self.curve2.setPos(time2,0)

    def calibrator(self):
        dir = str(QtGui.QFileDialog.getExistingDirectory(self, "Seleccione un folder con las imagenes de calibracion"))
        print("Selected folder: %s" % dir)

        code = subprocess.call(['echo', '"HOLI"'], shell=True)
        print(code)

    def run_camera(self):

        ui = Ui_Dialog(self)

        if ui.exec_() == QtWidgets.QDialog.Accepted:
            print(ui.camera_cmd)

        pass

class curveData():
    def __init__(self, parent=None):
        self.windowWidth = 500
        self.clear()

    def clear(self):
        self.data = np.linspace(0,0,self.windowWidth)
        self.time = 0
        self.x = 0
        self.y = 0

    def updateCurve(self):
        self.data[:-1] = self.data[1:]
        x,y = self.getData()
        self.data[-1] = float(y)
        self.time = x-self.windowWidth
        return self.time, self.data

    def getData(self):
        self.x += 1
        self.y = 0.2*(0.5-random.random())
        return self.x,self.y


if __name__ == '__main__':
    app = QtGui.QApplication([sys.argv])
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
