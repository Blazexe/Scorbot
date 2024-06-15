# -*- coding: utf-8 -*-

import re
import inverse
import numpy as np
import sys
import scorbot
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QWidget, QMessageBox, QApplication, QFileDialog
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice, Qt , pyqtSignal, QTimer
from PyQt5.QtGui import QColor
import matplotlib.pyplot as plt

class ScorbotApp(QtWidgets.QMainWindow, scorbot.Ui_MainWindow):

    def __init__(self):
        
        super().__init__()
        self.setupUi(self) 

        self.T1_send = 0
        self.T2_send = 125
        self.T3_send = 0
        self.T4_send = 0
        self.T5_send = 0
        self.T1 = 0.0  # Добавляем атрибуты
        self.T2 = 125.0
        self.T3 = 0.0
        self.T4 = 0
        self.T5 = 0.0
        self.x = 119.67
        self.y = 0.0
        self.z = 396.85
        self.pitch = 0
        self.roll = 0
        self.lenght = 0
        self.gr = 0

        self.T1_show = 0
        self.T2_show = 0
        self.T3_show = 0
        self.T4_show = 0
        self.T5_show = 0

        self.x_show = 0
        self.y_show = 0
        self.z_show = 0
        self.pitch_show = 0
        self.roll_show = 0

        self.T1_slider = 0
        self.T2_slider = 125
        self.T3_slider = 0
        self.T4_slider = 0
        self.T5_slider = 0

        self.grad_v = 5
        self.mm_v = 20

        self.V = 0
        self.V1 = 0
        self.V2 = 0
        self.V3 = 0
        self.V4 = 0
        self.V5 = 0
        self.V1_send = 0
        self.V2_send = 0
        self.V3_send = 0
        self.V4_send = 0
        self.V5_send = 0

        self.x2 = 0
        self.y2 = 0
        self.z2 = 0

        self.message = str
        self.stop_flag = 0
        self.done_flag = 0
        self.increment = 10

        self.serial = QSerialPort()
        self.serial.setBaudRate(500000)
        self.serial.readyRead.connect(self.receive)

        self.searchButton.clicked.connect(self.BeginserialCOMPort)
        self.homeButton.clicked.connect(self.home_position)
        self.stopButton.clicked.connect(self.stop_event)

        self.base_slider.valueChanged.connect(self.update_base_slider)
        self.shoulder_slider.valueChanged.connect(self.update_shoulder_slider)
        self.elbow_slider.valueChanged.connect(self.update_elbow_slider)
        self.pitch_slider.valueChanged.connect(self.update_pitch_slider)
        self.roll_slider.valueChanged.connect(self.update_roll_slider)

        self.xSldr.valueChanged.connect(self.update_x_slider)
        self.ySldr.valueChanged.connect(self.update_y_slider)
        self.zSldr.valueChanged.connect(self.update_z_slider)
        self.pitchSldr.valueChanged.connect(self.update_pitch_slider_1)
        self.rollSldr.valueChanged.connect(self.update_roll_slider_1)

        self.lineEditX.textChanged.connect(self.update_lineEditX)
        self.lineEditY.textChanged.connect(self.update_lineEditY)
        self.lineEditZ.textChanged.connect(self.update_lineEditZ)
        self.lineEditPitch.textChanged.connect(self.update_lineEditPitch)
        self.lineEditRoll.textChanged.connect(self.update_lineEditRoll)

        self.GrCloseBtn.clicked.connect(self.close_gripper)
        self.GrOpenBtn.clicked.connect(self.open_gripper)
        
        self.BaseupBtn.pressed.connect(self.start_base_up)
        self.BaseupBtn.released.connect(self.stop_base_up)
        self.BasedwnBtn.pressed.connect(self.start_base_down)
        self.BasedwnBtn.released.connect(self.stop_base_down)

        self.ShdrupBtn.pressed.connect(self.start_shdr_up)
        self.ShdrupBtn.released.connect(self.stop_shdr_up)
        self.ShdrdwnBtn.pressed.connect(self.start_shdr_down)
        self.ShdrdwnBtn.released.connect(self.stop_shdr_down)

        self.ElbowupBtn.pressed.connect(self.start_elbow_up)
        self.ElbowupBtn.released.connect(self.stop_elbow_up)
        self.ElbowdwnBtn.pressed.connect(self.start_elbow_down)
        self.ElbowdwnBtn.released.connect(self.stop_elbow_down)

        self.PitchupBtn.pressed.connect(self.start_pitch_up)
        self.PitchupBtn.released.connect(self.stop_pitch_up)
        self.PitchdwnBtn.pressed.connect(self.start_pitch_down)
        self.PitchdwnBtn.released.connect(self.stop_pitch_down)

        self.RollupBtn.pressed.connect(self.start_roll_up)
        self.RollupBtn.released.connect(self.stop_roll_up)
        self.RolldwnBtn.pressed.connect(self.start_roll_down)
        self.RolldwnBtn.released.connect(self.stop_roll_down)

        self.positions_range = -1
        self.positions = []
        self.positions_show = []
        self.addPosButton.clicked.connect(self.add_position)
        
        self.commands = []
        self.command_index = 0
        self.com_flag = 0
        self.add_comBtn.clicked.connect(self.add_command)
        self.command_timer = QTimer()
        self.command_timer.setSingleShot(True)
        self.exec_Btn.clicked.connect(self.execute_command)
        self.command_timer.timeout.connect(self.execute_next_command)

        self.timer_x_up = QTimer(self)
        self.timer_x_up.timeout.connect(self.x_up_continuously)
        self.timer_x_down = QTimer(self)
        self.timer_x_down.timeout.connect(self.x_down_continuously)

        self.timer_y_up = QTimer(self)
        self.timer_y_up.timeout.connect(self.y_up_continuously)
        self.timer_y_down = QTimer(self)
        self.timer_y_down.timeout.connect(self.y_down_continuously)

        self.timer_z_up = QTimer(self)
        self.timer_z_up.timeout.connect(self.z_up_continuously)
        self.timer_z_down = QTimer(self)
        self.timer_z_down.timeout.connect(self.z_down_continuously)

        self.timer_pitch_up = QTimer(self)
        self.timer_pitch_up.timeout.connect(self.pitch_up_continuously)
        self.timer_pitch_down = QTimer(self)
        self.timer_pitch_down.timeout.connect(self.pitch_down_continuously)

        self.XupBtn.pressed.connect(self.start_x_up)
        self.XupBtn.released.connect(self.stop_x_up)
        self.XdwnBtn.pressed.connect(self.start_x_down)
        self.XdwnBtn.released.connect(self.stop_x_down)

        self.YupBtn.pressed.connect(self.start_y_up)
        self.YupBtn.released.connect(self.stop_y_up)
        self.YdwnBtn.pressed.connect(self.start_y_down)
        self.YdwnBtn.released.connect(self.stop_y_down)

        self.ZupBtn.pressed.connect(self.start_z_up)
        self.ZupBtn.released.connect(self.stop_z_up)
        self.ZdwnBtn.pressed.connect(self.start_z_down)
        self.ZdwnBtn.released.connect(self.stop_z_down)

        self.Orientup_Btn.pressed.connect(self.start_orientation_up)
        self.Orientup_Btn.released.connect(self.stop_orientation_up)
        self.Orientdown_Btn.pressed.connect(self.start_orientation_down)
        self.Orientdown_Btn.released.connect(self.stop_orientation_down)

        self.speed_comboBox.addItem("Градусы/с")
        self.speed_comboBox.addItem("мм/с")
        self.speed_lineEdit.textChanged.connect(self.set_speed)

        self. t = 0
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.path_planning)

        self.axisSendBtn.clicked.connect(self.send_angles)
        self.cordsSendBtn.clicked.connect(self.start_motion)

        self.Load_coms_button.clicked.connect(self.loadCommands)
        self.Load_pos_button.clicked.connect(self.loadPositions)
        self.save_Btn.clicked.connect(self.saveAll)
        self.del_comBtn.clicked.connect(self.deleteCommand)
        self.delPosButton.clicked.connect(self.deletePosition)

    def BeginserialCOMPort(self):
        nameBt = self.searchButton.text()

        if nameBt == "Поиск":
            portList = []
            ports = QSerialPortInfo().availablePorts()
            for port in ports:
                portList.append(port.portName())
            if len(portList) == 0:
                self.plainTextEditLog.appendPlainText("Нет доступных COM Портов \n")
                self.comboBoxCom.clear()
            else:
                portComList = list(set(portList))
                self.comboBoxCom.addItems(portComList)
                self.searchButton.setText('Открыть')

        if nameBt == 'Открыть':
            self.serial.setPortName(self.comboBoxCom.currentText())
            self.serial.open(QIODevice.ReadWrite)
            text_str = "Открыт COM порт " + self.comboBoxCom.currentText()
            self.plainTextEditLog.appendPlainText(text_str)
            self.searchButton.setText('Закрыть')
        
        if nameBt == 'Закрыть':
            self.searchButton.setText('Закрыть')
            self.serial.close()
            text_str = "Закрыт COM порт " + self.comboBoxCom.currentText()
            self.plainTextEditLog.appendPlainText(text_str)
            self.searchButton.setText('Поиск')
            self.comboBoxCom.clear()

    def receive(self):
        while self.serial.canReadLine():
            try:
                    text = self.serial.readLine().data().decode('utf-8', errors='ignore')
                    text = text.rstrip('\r\n')
                    values = text.split()
                    
                    self.T1_show, self.T2_show, self.T3_show, self.T4_show, self.T5_show = map(float, values)
                    self.T1_show = round(self.T1_show / 44.44, 2)
                    self.T2_show = round(self.T2_show / 36 + 125, 2)
                    self.T3_show = round(-self.T3_show / 30, 2)

                    self.T4_show = (self.T4_show + self.T5_show)/2
                    self.T5_show = (self.T4_show - self.T5_show)/2
                    self.T4_show = round(-self.T4_show / 8.3, 2)
                    self.T5_show = round(-self.T5_show / 8.3, 2)
                        
                    self.label_T1.setText(f'{self.T1_show}')
                    self.label_T2.setText(f'{self.T2_show}')
                    self.label_T3.setText(f'{self.T3_show}')
                    self.label_T4.setText(f'{round(self.T4_show,2)}')
                    self.label_T5.setText(f'{round(self.T5_show,2)}')

                    coords = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
                    self.x_show = round(coords[0],2)
                    self.y_show = round(coords[1],2)
                    self.z_show = round(coords[2],2)
                    self.pitch_show = round(self.T4_show,2)
                    self.roll_show = round(self.T5_show, 2)

                    self.label_x.setText(f'{self.x_show}')
                    self.label_y.setText(f'{self.y_show}')
                    self.label_z.setText(f'{self.z_show}')
                    self.label_pitch.setText(f'{self.pitch_show}')
                    self.label_roll.setText(f'{self.roll_show}')
            except ValueError:
                pass

    def set_increment(self,value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True and float(value) >= 0 and float(value) <= 100 and float(value):
            self.increment = float(value)
        else:
            self.increment = 5
            self.plainTextEditLog.appendPlainText("Максимальное значение инкремента = 100\n")

    def set_speed(self,value):
        selected_text = self.speed_comboBox.currentText()
        if selected_text == "Градусы/с":
            try:
                float(value)
                f = True
            except ValueError:
                f = False
            if f == True and float(value) >= 0 and float(value) <= 20 and float(value) >= 2:
                self.grad_v = float(value)
            else:
                self.plainTextEditLog.appendPlainText("МИН. скорость 2 град./c МАКС. скорость 20 град./c\n")
        elif selected_text == "мм/с":
            try:
                float(value)
                f = True
            except ValueError:
                f = False
            if f == True and float(value) >= 0 and float(value) <= 200 and float(value) >= 5:
                self.mm_v = float(value)
            else:
                self.plainTextEditLog.appendPlainText("МИН. скорость 5 мм./с МАКС. скорость 200 мм./с\n")
        else:
            self.lineEdit.clear()

    def home_position(self):
        if self.serial.isOpen():
            message_home = '31'
            self.serial.write(message_home.encode())
            self.x = 119.67
            self.y = 0.0
            self.z = 396.85
            self.T1 = 0.0  # Добавляем атрибуты
            self.T2 = 125.0
            self.T3 = 0.0
            self.T4 = -90
            self.T5 = 0.0
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    
    def stop_event(self):
        nameStopBt = self.stopButton.text()
        if self.serial.isOpen():
            if nameStopBt == "СТОП":
                    message_stop = '99'
                    self.serial.write(message_stop.encode())
                    self.plainTextEditLog.appendPlainText("Работа приостановлена\n")
                    self.stopButton.setText('Продолжить')
                    self.stop_flag = 1
            else:
                    message_stop = '99'
                    self.serial.write(message_stop.encode())
                    self.plainTextEditLog.appendPlainText("Работа возобновлена\n")
                    self.stopButton.setText('СТОП')
                    self.stop_flag = 0
        else:
                self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def update_base_slider(self, value):
        self.base_show.setText(f'{value} градусов')
        self.T1_slider = value
    def update_shoulder_slider(self, value):
        self.shoulder_show.setText(f'{value} градусов')
        self.T2_slider = value
    def update_elbow_slider(self, value):
        self.elbow_show.setText(f'{value} градусов')
        self.T3_slider = value
    def update_pitch_slider(self, value):
        self.pitch_label.setText(f'{value} градусов')
        self.T4_slider= value
    def update_roll_slider(self, value):
        self.roll_label.setText(f'{value} градусов')
        self.T5_slider = value

    def start_base_up(self):
        if self.serial.isOpen():
            if self.T1_show <= 155:  # Проверяем границы
                if self.grad_v != 0:
                    self.V1_send = self.grad_v
                    message = '61 ' + str(round(self.V1_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 1 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_base_up(self):
        if self.serial.isOpen():
            self.V1_send = 0
            message = '61 ' + str(round(self.V1_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_base_down(self):
        if self.serial.isOpen():
            if self.T1_show >= -155:  # Проверяем границы
                if self.grad_v != 0:
                    self.V1_send = -self.grad_v
                    message = '61 ' + str(round(self.V1_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 1 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_base_down(self):
        if self.serial.isOpen():
            if self.T1_show >= -155:  # Проверяем границы
                self.V1_send = 0
                message = '61 ' + str(round(self.V1_send,2))  # Формируем сообщение для Scorbot
                self.serial.write(message.encode())
                pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
                self.x = pos[0]
                self.y = pos[1]
                self.z = pos[2]
            else:
                self.plainTextEditLog.appendPlainText("Ось 1 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
        
    def start_shdr_up(self):
        if self.serial.isOpen():
            if self.T2_show <= 125:  # Проверяем границы
                if self.grad_v != 0:
                    self.V2_send = self.grad_v
                    message = '62 ' + str(round(self.V2_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 2 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_shdr_up(self):
        if self.serial.isOpen():
            self.V2_send = 0
            message = '62 ' + str(round(self.V2_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_shdr_down(self):
        if self.serial.isOpen():
            if self.T2_show >= -30:  # Проверяем границы
                if self.grad_v != 0:
                    self.V2_send = -self.grad_v
                    message = '62 ' + str(round(self.V2_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 2 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_shdr_down(self):
        if self.serial.isOpen():
            self.V2_send = 0
            message = '62 ' + str(round(self.V2_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_elbow_up(self): 
        if self.serial.isOpen():
            if self.T3_show <= 130:  # Проверяем границы
                if self.grad_v != 0:
                    self.V3_send = self.grad_v
                    message = '63 ' + str(round(self.V3_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 3 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_elbow_up(self):
        if self.serial.isOpen():
            self.V3_send = 0
            message = '63 ' + str(round(self.V3_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_elbow_down(self):
        if self.serial.isOpen():
            if self.T3_show >= -130:  # Проверяем границы
                if self.grad_v != 0:
                    self.V3_send = -self.grad_v
                    message = '63 ' + str(round(self.V3_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 3 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_elbow_down(self):
        if self.serial.isOpen():
            self.V3_send = 0
            message = '63 ' + str(round(self.V3_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_pitch_up(self):
        if self.serial.isOpen():
            if self.T4_show <= 130:  # Проверяем границы
                if self.grad_v != 0:
                    self.V4_send = self.grad_v
                    message = '64 ' + str(round(self.V4_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 4 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_pitch_up(self):
        if self.serial.isOpen():
            self.V4_send = 0
            message = '64 ' + str(round(self.V4_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            self.pitch = self.T4_show
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_pitch_down(self): 
        if self.serial.isOpen():
            if self.T4_show >= -130:  # Проверяем границы
                if self.grad_v != 0:
                    self.V4_send = -self.grad_v
                    message = '64 ' + str(round(self.V4_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 4 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_pitch_down(self):
        if self.serial.isOpen():
            self.V4_send = 0
            message = '64 ' + str(round(self.V4_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            self.pitch = self.T4_show
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    
    def start_roll_up(self): 
        if self.serial.isOpen():
            if self.T5_show <= 180:  # Проверяем границы
                if self.grad_v != 0:
                    self.V5_send = self.grad_v
                    message = '65 ' + str(round(self.V5_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 5 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_roll_up(self):
        if self.serial.isOpen():
            self.V5_send = 0
            message = '65 ' + str(round(self.V5_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            self.roll = self.T5_show
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_roll_down(self):
        if self.serial.isOpen():
            if self.T5_show >= -180:  # Проверяем границы
                if self.grad_v != 0:
                    self.V5_send = -self.grad_v
                    message = '65 ' + str(round(self.V5_send,2))  # Формируем сообщение для Scorbot
                    self.serial.write(message.encode())
                else:
                    self.plainTextEditLog.appendPlainText("Сначала установите скорость в град./с\n")
            else:
                self.plainTextEditLog.appendPlainText("Ось 5 достигла предельного положения\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_roll_down(self):
        if self.serial.isOpen():
            self.V5_send = 0
            message = '65 ' + str(round(self.V5_send,2))  # Формируем сообщение для Scorbot
            self.serial.write(message.encode())
            self.roll = self.T5_show
            pos = inverse.dkm(self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show)
            self.x = pos[0]
            self.y = pos[1]
            self.z = pos[2]
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def update_leght(self,value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True and float(value) >= 0 and float(value) <= 75:
            self.lenght = float(value)
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число от 1 до 75 мм\n")

    def open_gripper(self):
        if self.serial.isOpen():
            self.message = '17'
            self.serial.write(self.message.encode())
            self.command_timer.start(3000) 
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def close_gripper(self):
        if self.serial.isOpen():
                self.message = '16'   
                self.serial.write(self.message.encode()) 
                self.command_timer.start(3000) 
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_x_up(self):
        if self.serial.isOpen():
            self.timer_x_up.start(int(1000/self.mm_v))
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_x_up(self):
        self.timer_x_up.stop() 
    def start_x_down(self):
        if self.serial.isOpen():
            self.timer_x_down.start(int(1000/self.mm_v))
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_x_down(self):
        self.timer_x_down.stop()

    def start_y_up(self):
        if self.serial.isOpen():
            self.timer_y_up.start(int(1000/self.mm_v))
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_y_up(self):
            self.timer_y_up.stop()  
    def start_y_down(self):
        if self.serial.isOpen():
            self.timer_y_down.start(int(1000/self.mm_v)) 
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
    def stop_y_down(self):
        self.timer_y_down.stop()
    
    def start_z_up(self):
        if self.serial.isOpen():
            self.timer_z_up.start(int(1000/self.mm_v))  
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n")    
    def stop_z_up(self):
        self.timer_z_up.stop() 
    def start_z_down(self):
        if self.serial.isOpen():
            self.timer_z_down.start(int(1000/self.mm_v)) 
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_z_down(self):
        self.timer_z_down.stop()

    def start_orientation_up(self):
        if self.serial.isOpen():
            self.timer_pitch_up.start(int(1000/self.mm_v)) 
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_orientation_up(self):
        self.timer_pitch_up.stop() 

    def start_orientation_down(self):
        if self.serial.isOpen():
            self.timer_pitch_down.start(int(1000/self.mm_v)) 
        self.plainTextEditLog.appendPlainText("COM порт не подключен\n") 
    def stop_orientation_down(self):
        self.timer_pitch_down.stop()

    def x_up_continuously(self):
            if self.x <= 609 and not np.isnan(self.T2_send): # Проверяем границы 
                self.x += 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси X достиг предельного положения\n")
    def x_down_continuously(self):
            if self.x >= -609 and (self.y>= 120 or self.y <=-120 or self.x >= 120) and not np.isnan(self.T2_send): 
                self.x -= 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси X достиг предельного положения\n")

    def y_up_continuously(self):
            if self.y <= 609 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send):
                self.y += 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси Y достиг предельного положения\n")
    def y_down_continuously(self):
            if self.y >= -609 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send): 
                self.y -= 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси Y достиг предельного положения\n")

    def z_up_continuously(self):
            if self.z <= 929 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send):                
                self.z += 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси Z достиг предельного положения\n")
    def z_down_continuously(self):
            if self.z >= 0 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send): 
                self.z -= 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по оси Z достиг предельного положения\n")

    def pitch_up_continuously(self):
            if self.pitch <= 129 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send):                
                self.pitch += 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по ориентации достиг предельного положения\n")
    def pitch_down_continuously(self):
            if self.pitch >= -39 and (self.x>= 120 or self.x <=-120) and not np.isnan(self.T2_send): 
                self.pitch -= 1
                self.send_cordinates()
            else:
                self.plainTextEditLog.appendPlainText("Scorbot по ориентации достиг предельного положения\n")

    def update_lineEditX(self, value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True:
            value = int(value)
            self.xSldr.setSliderPosition(value)
            self.x = value
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число \n")

    def update_lineEditY(self, value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True:
            value = int(value)
            self.ySldr.setSliderPosition(value)
            self.y = value
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число \n")

    def update_lineEditZ(self, value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True:
            value = int(value)
            self.zSldr.setSliderPosition(value)
            self.z = value
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число \n")

    def update_lineEditPitch(self, value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True:
            value = int(value)
            self.pitchSldr.setSliderPosition(value)
            self.pitch = value
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число \n")

    def update_lineEditRoll(self, value):
        try:
            float(value)
            f = True
        except ValueError:
            f = False
        if f == True:
            value = int(value)
            self.rollSldr.setSliderPosition(value)
            self.roll = value
        if f == False and value!="":
            self.plainTextEditLog.appendPlainText("Введите число \n")

    def update_x_slider(self, value):
        self.lineEditX.setText(f'{value}')
        self.x = value
    def update_y_slider(self, value):
        self.lineEditY.setText(f'{value}')
        self.y = value
    def update_z_slider(self, value):
        self.lineEditZ.setText(f'{value}')
        self.z = value
    def update_pitch_slider_1(self, value):
        self.lineEditPitch.setText(f'{value}')
        self.pitch = value
    def update_roll_slider_1(self, value):
        self.lineEditRoll.setText(f'{value}')
        self.roll = value

    def send_cordinates(self):
        if self.serial.isOpen():
            try:
                angles = inverse.ikm(self.x,self.y,self.z,self.pitch,self.roll)
                self.T1_send = round((angles[0] * 44.44),2)
                self.T2_send = round(((-125 + angles[1]) * 36),2)
                self.T3_send = round((-angles[2] * 30),2)
                self.T4_send = round((angles[3] * 8.3),2)
                self.T5_send = round((angles[4] * 8.3),2)
                # Check for NaN values
                if not np.isnan(self.T2_send) and not np.isnan(self.T3_send):
                    self.message = f"0 11 {self.T1_send} 12 {self.T2_send} 13 {self.T3_send} 14 {self.T4_send} 15 {self.T5_send}"
                    self.serial.write(self.message.encode())
            except Exception as e:
                self.plainTextEditLog.appendPlainText("Недопустимые значения координат\n")
            except ValueError:
                self.plainTextEditLog.appendPlainText("Недопустимые значения координат\n")
            except RuntimeWarning:
                self.plainTextEditLog.appendPlainText("Недопустимые значения координат\n")
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")    
    
    def send_angles(self):
        if self.serial.isOpen() and self.stop_flag == 0:
            self.T1_send = round(self.T1_slider * 44.44,2)
            self.T2_send = round((-125 + self.T2_slider)*36,2)
            self.T3_send = round((-self.T3_slider)*30,2)
            self.T4_send = round((self.T4_slider)*8.3,2)
            self.T5_send = round(self.T5_slider*8.3,2)
            self.message = f"0 11 {self.T1_send} 12 {self.T2_send} 13 {self.T3_send} 15 {self.T5_send} 14 {self.T4_send}"
            self.serial.write(self.message.encode())
            coords = inverse.dkm(self.T1_slider,self.T2_slider,self.T3_slider,self.T4_slider,self.T5_slider)
            self.x = coords[0]
            self.y = coords[1]
            self.z = coords[2]
            self.pitch = self.T4_send/8.3
            self._roll = self.T5_send/8.3
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def start_motion(self):
        self.path_x = self.x - self.x_show
        self.path_y = self.y - self.y_show
        self.path_z = self.z - self.z_show
        self.L = np.sqrt(self.path_x**2 + self.path_y**2 +self.path_z**2)
        self.Llast = self.L
        self.x_start = self.x_show
        self.y_start = self.y_show
        self.z_start = self.z_show
        self.a = (self.mm_v**2)/(self.L * 0.4)
        self.t_full = self.L*0.8/(self.mm_v) + self.L*0.6/self.mm_v
        self.start_time()

    def start_time(self):
        if self.mm_v == 0:
            self.plainTextEditLog.appendPlainText("Сначала установите скорость в мм/с\n")
        elif self.mm_v > 300:
            self.plainTextEditLog.appendPlainText("Максимальная скорость 300 мм/с\n")
        else:
            self.timer.start(5)
    
    def path_planning(self):
        if self.serial.isOpen():
            if self.stop_flag == 0:
                if self.done_flag == 1:
                    self.timer.stop()
                    self.t = 0
                    self.V = 0
                    self.done_flag = 0
                    self.command_timer.start(500)
                else:
                    self.t += 0.005
                    self.send_velocities()
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")
            self.timer.stop()

    def send_velocities(self):
            need_pos = inverse.path_planning(self.x_start,self.y_start,self.z_start,self.x,self.y,self.z,self.mm_v,self.a,self.t,self.t_full,self.L,self.Llast) # Линейная интерполяция по dt = 0.1 сек
            x_need = need_pos[0]
            y_need = need_pos[1]
            z_need = need_pos[2]

            self.path_xlast = self.x - x_need
            self.path_ylast = self.y - y_need
            self.path_zlast = self.z - z_need
            self.Llast = np.sqrt(self.path_xlast**2 + self.path_ylast**2 +self.path_zlast**2) #Расчет оставшегося пути
            
            if (self.t >= self.t_full - 0.01):
                self.done_flag = 1
                return 0

            if ((self.Llast) >= (self.L*0.8)):                            #Расчёт скорости с ускорением 20% пути в начале и конце 
                if self.V < self.mm_v:
                    self.V = self.mm_v * 5 * (1 - (self.Llast)/(self.L))
            elif ((self.Llast) <= (self.L*0.2)):
                if self.V >= 0:
                    self.V  = self.mm_v * 5 * (self.Llast/self.L)
            else:
                self.V = self.mm_v

            pitch = round(self.pitch*(self.t/self.t_full) + self.pitch_show * (1-self.t/self.t_full),2)
            roll = round(self.roll*(self.t/self.t_full) + self.roll_show * (1-self.t/self.t_full),2)

            need_angles = inverse.ikm(x_need,y_need,z_need,pitch,roll)             # Обратная кинематика
            T1_send = round(need_angles[0] * 44.44,1)
            T2_send = round((-125 + need_angles[1])*36,1)
            T3_send = round((-need_angles[2])*30,1)
            T4_send = round((need_angles[3])*8.3,1)
            T5_send = round(need_angles[4]*8.3,1)

            need_velocities = inverse.velocity_planning(self.x_show,self.y_show,self.z_show,x_need,y_need,z_need,self.V)      #Расчёт проекций линейной скорости рабочего органа на оси
            x_dot = need_velocities[0]
            y_dot = need_velocities[1]
            z_dot = need_velocities[2]

            need_omega = inverse.jacobian(x_dot,y_dot,z_dot,self.T1_show,self.T2_show,self.T3_show,self.T4_show,self.T5_show) #Расчёт угловых скоростей звеньев
            if need_omega == None:
                return 0
            V1 = round(need_omega[0],1)
            V2 = round(need_omega[1],1)
            V3 = round(need_omega[2],1)
            V4 = round(need_omega[3],1)
            V5 = round(need_omega[4],1)

            message = f"p 11 {T1_send} 12 {T2_send} 13 {T3_send} 14 {T4_send} 15 {T5_send} 21 {V1} 22 {V2} 23 {V3} 24 {V4} 25 {V5}"
            self.serial.write(message.encode())

    def updateCommandList(self):
        self.com_widget.clear()
        self.com_widget.addItems(self.commands)

    def updatePositionList(self):
        self.pos_widget.clear()
        self.positions_show.clear()
        self.positions_range = 0
        for pos in self.positions:
            self.positions_range += 1
            self.positions_show.append(f"Позиция {self.positions_range}: {pos}")
        self.pos_widget.addItems([str(position) for position in self.positions_show])

    def add_position(self):
        if self.serial.isOpen():
            self.positions.append([round(self.x,2),round(self.y,2),round(self.z,2),round(self.pitch,2),round(self.roll,2)])
            self.updatePositionList()
        else:
            self.plainTextEditLog.appendPlainText("COM порт не подключен\n")

    def add_command(self):
        new_command = self.com_edit.toPlainText().upper()
        selected_index = self.com_widget.currentRow()  # Получаем индекс выделенного элемента
        if selected_index != -1:  # Проверяем, что элемент выделен
            self.commands.insert(selected_index, new_command)  # Вставляем новую команду перед выделенным элементом
        else:
            self.commands.append(new_command)  # Если ничего не выделено, просто добавляем новую команду в конец списка
        self.updateCommandList()  # Обновляем список команд в виджете

    def execute_command(self):
        self.command_index = 0
        if self.commands:
            self.command_timer.start(1000)

    def execute_next_command(self):
        if self.command_index < len(self.commands):
            command = self.commands[self.command_index]
            self.parse_command(command)
            self.command_index += 1
            self.highlight_current_command()

    def highlight_current_command(self):
        # Сбросить фон всех элементов списка команд
        for i in range(self.com_widget.count()):
            item = self.com_widget.item(i)
            item.setBackground(QColor("white"))
        # Подсветить текущий элемент
        if 0 <= self.command_index < self.com_widget.count():
            item = self.com_widget.item(self.command_index-1)
            item.setBackground(QColor("lightgreen"))

    def parse_command(self,command):
        LMOVE_pattern = re.compile(r"LMOVE\((\d+)\)")
        HOME_pattern = re.compile(r"HOME")
        SPEED_pattern = re.compile(r"SPEED\((\d+)\)")
        CLOSE_pattern = re.compile(r"CLOSE")
        OPEN_pattern = re.compile(r"OPEN")

        match_SPEED = SPEED_pattern.match(command)
        match_LMOVE = LMOVE_pattern.match(command)
        match_HOME = HOME_pattern.match(command)
        match_CLOSE = CLOSE_pattern.match(command)
        match_OPEN = OPEN_pattern.match(command)
        if match_LMOVE:
            pos = int(match_LMOVE.group(1)) - 1
            if pos > self.positions_range:
                self.plainTextEditLog.appendPlainText(f"{command} - Позиции с таким номером не существует")
                return None
            self.x = round(self.positions[pos][0],2)
            self.y = round(self.positions[pos][1],2)
            self.z = round(self.positions[pos][2],2)
            self.pitch = round(self.positions[pos][3],2)
            self.roll = round(self.positions[pos][4],2)
            self.start_motion()
            self.com_flag = 1
        elif match_HOME:
            self.message = "0 11 0 12 0 13 0 15 0 14 0"
            self.serial.write(self.message.encode())
            self.com_flag = 2
            self.x = 119.67
            self.y = 0.0
            self.z = 396.85
            self.pitch = 0
            self.roll = 0
            self.command_timer.start(10000)
        elif match_SPEED:
            self.mm_v = int(match_SPEED.group(1))
            self.command_timer.start(1000)
        elif match_CLOSE:
            self.close_gripper()
            self.com_flag = 3
        elif match_OPEN:
            self.open_gripper()
            self.com_flag = 4
        else:
            self.plainTextEditLog.appendPlainText(f"{command} - Синтаксическая ошибка")
            return None
    
    def saveCommands(self):
        filename, _ = QFileDialog.getSaveFileName(self, "Сохраните управляющую программу", "", "Text Files (*.txt)")
        if filename:
            with open(filename, 'w') as file:
                for command in self.commands:
                    file.write(f"{command}\n")
            QMessageBox.information(self, "Сохранено", "Управляющая программа сохранена.")

    def savePositions(self):
        filename, _ = QFileDialog.getSaveFileName(self, "Сохраните позиции", "", "Text Files (*.txt)")
        if filename:
            with open(filename, 'w') as file:
                for position in self.positions:
                    # Записываем числа позиции через пробел, без квадратных скобок
                    file.write(' '.join(map(str, position)) + '\n')
            QMessageBox.information(self, "Сохранено", "Позиции сохранены.")

    def saveAll(self):
        self.saveCommands()
        self.savePositions()

    def loadCommands(self):
        filename, _ = QFileDialog.getOpenFileName(self, "Загрузите управляющую программу", "", "Text Files (*.txt)")
        if filename:
            self.commands = []
            with open(filename, 'r') as file:
                for line in file:
                    command = line.strip()
                    # Проверяем, содержит ли строка символ '('
                    if '(' in command:
                        parts = command.split('(')
                        parts[0] = parts[0].upper()
                        command = '('.join(parts)
                    else:
                        command = command.upper()
                    self.commands.append(command)
                self.updateCommandList()
                QMessageBox.information(self, "Загружено", "Управляющая программа загружена.")

    def loadPositions(self):
        filename, _ = QFileDialog.getOpenFileName(self, "Загрузите позиции", "", "Text Files (*.txt)")
        if filename:
            self.positions = []
            with open(filename, 'r') as file:
                for line in file:
                    data = line.strip().split(' ')
                    numbers = [float(num) for num in data]
                    self.positions.append(numbers)
            self.updatePositionList()
            QMessageBox.information(self, "Загружено", "Позиции загружены.")
        
    def deleteCommand(self):
        # Получаем текущий выбранный элемент
        current_item = self.com_widget.currentItem()
        if current_item:
            # Удаляем команду из массива commands
            command_to_delete = current_item.text()
            self.commands.remove(command_to_delete)
            # Обновляем список команд в виджете
            self.updateCommandList()
            QMessageBox.information(self, "Удалено", f"Команда '{command_to_delete}' удалена.")

    def deletePosition(self):
        # Получаем текущий выбранный элемент
        current_item = self.pos_widget.currentItem()
        if current_item:
            # Удаляем команду из массива commands
            position_to_delete = current_item.text()
            index_to_delete = self.pos_widget.currentRow()
            del self.positions[index_to_delete]
            # Обновляем список команд в виджете
            self.updatePositionList()
            QMessageBox.information(self, "Удалено", f"Позиция '{position_to_delete}' удалена.")

    def closeEvent(self, event): 
        reply = QMessageBox.question(self, 'Сообщение', "Вы уверены, что хотите выйти?", QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()
    
def main():
    app = QtWidgets.QApplication(sys.argv) 
    window = ScorbotApp()  
    window.show()  
    app.exec_() 

if __name__ == '__main__':
    main()