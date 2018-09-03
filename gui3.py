from pyqtgraph.Qt import QtCore, QtGui
from numpy import *
import pyqtgraph as pg
import random


class curve():
    def __init__(self, parent=None):
        self.windowWidth = 500
        self.data = linspace(0,0,self.windowWidth)
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


class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.central_widget = QtGui.QStackedWidget()
        self.setCentralWidget(self.central_widget)
        self.plot_widget = PlotWidget(self)
        self.plot_widget.redbutton.clicked.connect(self.plotter)
        self.central_widget.addWidget(self.plot_widget)

        self.click_flag = 0
        self.plot = curve()
        self.plot2 = curve()


    def plotter(self):
        self.curve = self.plot_widget.plot.getPlotItem().plot()
        self.curve2 = self.plot_widget.plot2.getPlotItem().plot()

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.updater)
        if self.click_flag == 0:
            self.timer.start(33)
            self.click_flag = 1
        else:
            self.timer.stop()
            self.click_flag = 0


    def updater(self):
        time, data = self.plot.updateCurve()
        time2, data2 = self.plot2.updateCurve()
        self.curve.setData(data)
        self.curve2.setData(data2)
        self.curve.setPos(time,0)
        self.curve2.setPos(time2,0)


class PlotWidget(QtGui.QWidget):
    def __init__(self, parent=None):
        super(PlotWidget, self).__init__(parent)
        layout = QtGui.QVBoxLayout()
        self.plot = pg.PlotWidget()
        layout.addWidget(self.plot)
        self.plot2 = pg.PlotWidget()
        layout.addWidget(self.plot2)
        Hlayout = QtGui.QHBoxLayout()
        self.redbutton = QtGui.QPushButton()
        self.redbutton.setFixedSize(QtCore.QSize(24,24))
        self.redbutton.setIcon(QtGui.QIcon("./redbutton.jpeg"))
        self.redbutton.setIconSize(QtCore.QSize(24,24))
        self.redbutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")
        Hlayout.addWidget(self.redbutton)
        self.greenbutton = QtGui.QPushButton()
        self.greenbutton.setFixedSize(QtCore.QSize(24,24))
        self.greenbutton.setIcon(QtGui.QIcon("./greenbutton.jpeg"))
        self.greenbutton.setIconSize(QtCore.QSize(24,24))
        self.greenbutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")
        Hlayout.addWidget(self.greenbutton)
        self.bluebutton = QtGui.QPushButton()
        self.bluebutton.setFixedSize(QtCore.QSize(24,24))
        self.bluebutton.setIcon(QtGui.QIcon("./bluebutton.jpeg"))
        self.bluebutton.setIconSize(QtCore.QSize(24,24))
        self.bluebutton.setStyleSheet("QPushButton {color: #F6F6F6; background: transparent}")
        Hlayout.addWidget(self.bluebutton)
        layout.addLayout(Hlayout)
        self.setLayout(layout)

if __name__ == '__main__':
    app = QtGui.QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
