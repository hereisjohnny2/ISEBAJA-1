'''
TeleMetria - ISE BAJA versao 0.1 Beta
Escrito por: Gabriel Rodrigues de Azeredo
Data: 11/10/2016
'''

# -*- coding: latin1 -*-
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.properties import StringProperty,NumericProperty,BooleanProperty,ObjectProperty

import json
import requests


class principal(BoxLayout):
    A0 = StringProperty()
    A1 = StringProperty()
    A2 = StringProperty()
    A3 = StringProperty()
    A4 = StringProperty()
    A5 = StringProperty()
    status_carro = StringProperty()
    rpm = StringProperty()

    def recebe_valores(self):
         r = requests.get('http://10.0.0.177/json')
         if r.status_code == 200:
            telemetria = json.loads(r.content)
            return telemetria

    def exibe_valores(self, *args):
        telemetria = self.recebe_valores()
        self.A0 = str(telemetria['Analogico0'])
        self.A1 = str(telemetria['Analogico1'])
        self.A2 = str(telemetria['Analogico2'])
        self.A3 = str(telemetria['Analogico3'])
        self.A4 = str(telemetria['Analogico4'])
        self.A5 = str(telemetria['Analogico5'])
        self.rpm = str(telemetria['RPM'])

        if telemetria['Motor_Ligado'] == '0':
            self.status_carro = 'Ligar Carro'
        if telemetria['Motor_Ligado'] == '1':
            self.status_carro = 'Desligar Carro'

    def ligar_carro(self):
        if self.status_carro == 'Ligar Carro':
            r = requests.get('http://10.0.0.177/ligarmotor')
        if self.status_carro == 'Desligar Carro':
            r = requests.get('http://10.0.0.177/desligarmotor')




class Gui(App):
    def build(self):
        self.title="Telemetria ISE BAJA"
        plataforma = principal()
        Clock.schedule_interval(plataforma.exibe_valores,1)
        return plataforma

Gui().run()
