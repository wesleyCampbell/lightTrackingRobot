"""
@file serialComs.py

@brief A thrown-together python script that allows some basic serial data reading.

A rough draft that will read data coming in from the serial port and will display it on 
some pyplot graphs. Each sensor and robot state will have its own line graph, showing the 
current state of the robot

Part of the lightTrackingRobot project.

@author Wesley Campbell
@date   2026-01-29
@version 1.0.0
"""

import serial

from collections import deque
import matplotlib
matplotlib.use("QtAgg")
import matplotlib.pyplot as plt

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

GRAPH_WINDOW = 200

DATA_PACKET_HEADER = 0xF1
ACTION_PACKET_HEADER = 0xF2

HEADERS = {
        DATA_PACKET_HEADER: 3,
        ACTION_PACKET_HEADER: 3
        }

PLOT_INTERVAL = 0.05

GRAPH_AMPLITUDE = 0.4

###################################################################3

#                        GLOBALS

###################################################################3

lines_data = []
buffers_data = [deque(maxlen=GRAPH_WINDOW) for _ in range(SENSORS_LEN)]

lines_actions = []
buffers_actions = [deque(maxlen=GRAPH_WINDOW) for _ in range(ACTIONS_LEN)]

###################################################################3

#                        DATA READ METHODS

###################################################################3

def read_packet(serialPort):
    header = ser.read(1)

    # If there is no packet, break off
    if not header:
        return None

    header = header[0]

    try:
        packet_size = HEADERS[header]
        payload = ser.read(packet_size)

        # if payload is corrupted
        if not payload or len(payload) != packet_size:
            return None

        return (header, payload)
    except KeyError:
        return None

def unpack_light_data(byte):
    return [(byte >> i) & 1 for i in range(4)]


###################################################################3

#                        DATA PLOTTING METHODS

###################################################################3

def _init_data_plot(fig, ax_data):
    for i in range(SENSORS_LEN):
        line, = ax_data.plot([], [], drawstyle='steps-post')
        lines_data.append(line)

    ax_data.set_ylim(-1, SENSORS_LEN)
    ax_data.set_xlim(0, GRAPH_WINDOW)
    ax_data.set_yticks(range(SENSORS_LEN))
    ax_data.set_yticklabels([sensor for sensor in SENSORS])

def _init_action_plot(fig, ax_action):
    for i in range(ACTIONS_LEN):
        line, = ax_action.plot([], [], drawstyle='steps-post')
        lines_actions.append(line)

    ax_action.set_ylim(-1, ACTIONS_LEN)
    ax_action.set_xlim(0, GRAPH_WINDOW)
    ax_action.set_yticks(range(ACTIONS_LEN))
    ax_action.set_yticklabels([action for action in ACTIONS])

def init_plot():
    plt.ion()
    fig, (ax_data, ax_actions) = plt.subplots(
                nrows=1,
                ncols=2,
                sharex=True,
                figsize=(12,8)
            )

    _init_data_plot(fig, ax_data)
    _init_action_plot(fig, ax_actions)

    return fig, (ax_data, ax_actions) 

def _update_data_plot():
    for i, buf in enumerate(buffers_data):
        y = [val + i for val in buf]
        lines_data[i].set_data(range(len(y)), y)

def _update_action_plot():
    for i, buf in enumerate(buffers_actions):
        y = [val + i for val in buf]
        lines_actions[i].set_data(range(len(y)), y)

def update_plot():
    # for i, buf in enumerate(buffers):
    #     y = [v + i for v in buf]
    #     lines[i].set_data(range(len(y)), y)
    _update_data_plot()
    _update_action_plot()

    plt.draw()  
    plt.pause(0.01)

def update_data(data):
    for i, bit in enumerate(data):
        buffers_data[i].append(bit * GRAPH_AMPLITUDE)

def update_actions(action):
    for i, bit in enumerate(action):
        buffers_actions[i].append(bit * GRAPH_AMPLITUDE)

###################################################################3

#                        MAIN

###################################################################3

def shutdown(serialPort):
    serialPort.close()
    plt.close("all")

def parseDriveData(driveData):
    return [driveData & DRIVE_STATES['left'] != 0, 
            driveData & DRIVE_STATES['right'] != 0]

def parseServoData(servoData):
    return [servoData & SERVO_STATES['down'] != 0,
            servoData & SERVO_STATES['up'] != 0]

def readPayload(serialPort):
    reading = read_packet(serialPort)

    if not reading:
        return None
    
    header, payload = reading

    if (header == DATA_PACKET_HEADER):
        lightData = unpack_light_data(payload[0])

        data = lightData[0:2] + [payload[1]] + lightData[2:4] + [payload[2]]

        print(f"DATA: {data}")
        update_data(data)
    elif (header == ACTION_PACKET_HEADER):
        collision = payload[0]
        drive = parseDriveData(payload[1])
        servo = parseServoData(payload[2])

        # actions = collision + drive + servo
        actions = list([servo[0], drive[0], collision, drive[1], servo[1]])

        print(f"ACTIONS: {actions}")

        update_actions(actions)


if __name__ == "__main__":
    running = True

    fig, ax = init_plot()

    def interrupt(_no_idea):
        global running
        running = False

    fig.canvas.mpl_connect('close_event', interrupt)

    ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)

    lastPlot = 0
    
    while running:
        try:
            readPayload(ser)

            # Check to see if we need to plot again
            now = time.monotonic()
            if (now - lastPlot >= PLOT_INTERVAL):
                print("time to plot!")
                update_plot()
                lastPlot = now

        except Exception as e:
            print(f"Caught exception {e}")
            print("Exiting now...")
            running = False

    shutdown(ser)


