"""
@file serialComs.py

@brief A thrown-together python script that allows some basic serial data reading.

A rough draft that will read data coming in from the serial port and will display it on 
some pyplot graphs. Each sensor and robot state will have its own line graph, showing the 
current state of the robot

Part of the lightTrackingRobot project.

@author Wesley Campbell
@date   2026-01-29
@version 1.0.1
"""

import serial
import struct

from collections import deque
import matplotlib
matplotlib.use("QtAgg")
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

import time

SENSORS = [
        "Light Down",
        "Light Left",
        "Collision",
        "Light Right",
        "Light Up",
        "Capacitive"
        ]

SENSORS_LEN = len(SENSORS)

ACTIONS = [
        "Servo Down",
        "Drive Left",
        "Collision!",
        "Drive Right",
        "Servo Up"
        ]

DRIVE_STATES = {
        'left': 0x01,
        'right': 0x10,
        }

SERVO_STATES = {
        'down': 0x10,
        'up' : 0x01,
        }

ACTIONS_LEN = len(ACTIONS)

PIN_VOLTAGE_MAX = 5

GRAPH_WINDOW = 200
X_AXIS = list(range(GRAPH_WINDOW))

DATA_PACKET_HEADER = 0xAA
ACTION_PACKET_HEADER = 0xBB
PIN_DATA_PACKET_HEADER = 0xCC

HEADERS = {
        DATA_PACKET_HEADER: 3,
        ACTION_PACKET_HEADER: 3,
        PIN_DATA_PACKET_HEADER: 5
        }

PLOT_INTERVAL = 0.09

GRAPH_AMPLITUDE = 0.4

###################################################################3

#                        GLOBALS

###################################################################3

lines_data = []
buffers_data = [deque(maxlen=GRAPH_WINDOW) for _ in range(SENSORS_LEN)]

lines_actions = []
buffers_actions = [deque(maxlen=GRAPH_WINDOW) for _ in range(ACTIONS_LEN)]

lines_pins = {}
buffers_pin_data = {}

###################################################################3

#                        DATA READ METHODS

###################################################################3

def read_packet(serialPort):

    header = 0
    while True:
        byte = serialPort.read(1)

        if not byte:
            continue

        header = byte[0]

        if header in HEADERS.keys():
            break

    packet_size = HEADERS[header]
    payload = serialPort.read(packet_size)

    if len(payload) != packet_size:
        return None

    return (header, payload)

def unpack_light_data(byte):
    return [(byte >> i) & 1 for i in range(4)]

def parseDriveData(driveData):
    return [driveData & DRIVE_STATES['left'] != 0, 
            driveData & DRIVE_STATES['right'] != 0]

def parseServoData(servoData):
    return [servoData & SERVO_STATES['down'] != 0,
            servoData & SERVO_STATES['up'] != 0]

def parsePinVoltageData(pinData):
    # voltage = pinData[1] << 8
    # voltage |= pinData[0]
    # return voltage
    voltage = struct.unpack('<f', pinData)
    return voltage

def readPayload(serialPort, ax):
    def handleDrivePacket(payload):
        lightData = unpack_light_data(payload[0])

        data = lightData[0:2] + [payload[1]] + lightData[2:4] + [payload[2]]

        update_data(data)

    def handleActionPacket(payload):
        collision = payload[0]
        drive = parseDriveData(payload[1])
        servo = parseServoData(payload[2])

        # actions = collision + drive + servo
        actions = list([servo[0], drive[0], collision, drive[1], servo[1]])

        update_actions(actions)

    def handlePinPacket(payload):
        pinNumber = payload[0]
        voltage = parsePinVoltageData(payload[1:5])

        if pinNumber not in buffers_pin_data.keys():
            # buffers_pin_data[pinNumber] = ax[2].plot([], [])
            buffers_pin_data[pinNumber] = deque(maxlen=GRAPH_WINDOW)

            line, = ax[2].plot([], [], label=f"Pin {pinNumber}")
            lines_pins[pinNumber] = line

        update_pin_data(pinNumber, voltage)
        ax[2].legend()

    ### Read the packet

    reading = read_packet(serialPort)

    if not reading:
        return None
    
    header, payload = reading

    if (header == DATA_PACKET_HEADER):
        handleDrivePacket(payload)
    elif (header == ACTION_PACKET_HEADER):
        handleActionPacket(payload)
    elif (header == PIN_DATA_PACKET_HEADER):
        handlePinPacket(payload)

###################################################################3

#                        DATA PLOTTING METHODS

###################################################################3

def _init_data_plot(fig, ax):
    for i in range(SENSORS_LEN):
        line, = ax.plot([], [], drawstyle='steps-post')
        lines_data.append(line)

    ax.set_ylim(-1, SENSORS_LEN)
    ax.set_xlim(0, GRAPH_WINDOW)
    ax.set_yticks(range(SENSORS_LEN))
    ax.set_yticklabels([sensor for sensor in SENSORS])

    ax.set_ylabel("Data")
    ax.set_title("Robot Sensor Data")

def _init_action_plot(fig, ax):
    for i in range(ACTIONS_LEN):
        line, = ax.plot([], [], drawstyle='steps-post')
        lines_actions.append(line)

    ax.set_ylim(-1, ACTIONS_LEN)
    ax.set_xlim(0, GRAPH_WINDOW)
    ax.set_yticks(range(ACTIONS_LEN))
    ax.set_yticklabels([action for action in ACTIONS])

    ax.set_title("Robot States")
    ax.set_xlabel("Time")

def _init_pin_plot(fig, ax):
    # lines_pins.append(ax.plot([], []))

    ax.set_ylim(-0.125, PIN_VOLTAGE_MAX + 1)
    ax.set_xlim(0, GRAPH_WINDOW)

    ax.yaxis.set_major_locator(MultipleLocator(0.5))
    ax.yaxis.set_minor_locator(MultipleLocator(0.1))

    ax.grid(which='major', linestyle='-', linewidth=0.8)
    ax.grid(which='minor', linestyle='--', linewidth=0.3)

    ax.set_ylabel("Volts")
    ax.set_title("Pin Voltages")

def init_plot():
    plt.ion()
    fig, (ax_data, ax_actions, ax_pin) = plt.subplots(
                nrows=1,
                ncols=3,
                sharex=True,
                figsize=(12,8)
            )

    _init_data_plot(fig, ax_data)
    _init_action_plot(fig, ax_actions)
    _init_pin_plot(fig, ax_pin)

    fig.canvas.draw()
    background = fig.canvas.copy_from_bbox(fig.bbox)

    return fig, (ax_data, ax_actions, ax_pin) 

def _update_data_plot():
    for i, buf in enumerate(buffers_data):
        y = [val + i for val in buf]
        lines_data[i].set_data(range(len(y)), y)


def _update_action_plot():
    for i, buf in enumerate(buffers_actions):
        y = [val + i for val in buf]
        lines_actions[i].set_data(range(len(y)), y)

def _update_pin_data_plot():
    for pin in buffers_pin_data.keys():
        # y = list(buffers_pin_data[pin])
        # lines_pins[pin].set_data(range(len(y)), y)

        buf = buffers_pin_data[pin]
        lines_pins[pin].set_ydata(buf)
        lines_pins[pin].set_xdata(X_AXIS[:len(buf)])

def update_plot():
    # for i, buf in enumerate(buffers):
    #     y = [v + i for v in buf]
    #     lines[i].set_data(range(len(y)), y)
    _update_data_plot()
    _update_action_plot()
    _update_pin_data_plot()

    # for line in lines_data + lines_actions + list(lines_pins.values()):
        # k

    plt.draw()  
    plt.pause(0.01)

def update_data(data):
    for i, bit in enumerate(data):
        buffers_data[i].append(bit * GRAPH_AMPLITUDE)

def update_actions(action):
    for i, bit in enumerate(action):
        buffers_actions[i].append(bit * GRAPH_AMPLITUDE)

def update_pin_data(pinNum, data):
    buffers_pin_data[pinNum].append(data)

###################################################################3

#                        MAIN

###################################################################3

def shutdown(serialPort):
    serialPort.close()
    plt.close("all")


EndException = None

if __name__ == "__main__":
    running = True

    fig, ax = init_plot()

    def interrupt(_no_idea):
        global running
        running = False

    fig.canvas.mpl_connect('close_event', interrupt)

    ser = serial.Serial("/dev/ttyACM1", 9600, timeout=1)

    lastPlot = 0
    
    while running:
        try:
            readPayload(ser, ax)

            # Check to see if we need to plot again
            now = time.monotonic()
            if (now - lastPlot >= PLOT_INTERVAL):
                update_plot()
                lastPlot = now

        except Exception as e:
            print(f"Caught exception {e}")
            print("Exiting now...")
            running = False
            EndException = e

    shutdown(ser)

    raise EndException



