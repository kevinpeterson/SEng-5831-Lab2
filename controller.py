import serial
import time
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

ser = serial.Serial('/dev/tty.usbmodem00055903', 9600)

print("connected to: " + ser.portstr)

def flush_input():
    time.sleep(1.0)
    ser.flushInput()

def _send_command(command):
    ser.write(command+'\r')

def turn_off_logging():
    _send_command('l')

def turn_on_logging():
    _send_command('l,')

def set_position(pos):
    _send_command('r'+str(pos))

def execute_trajectory(trajectory):
    _send_command('i'+str(trajectory))

def set_speed(speed):
    _send_command('s'+str(speed))

def read_serial(seconds):
    buffer = []
    end_time = time.time() + seconds
    while time.time() < end_time:
        buffer.append(ser.readline().strip())
    return [l for l in buffer if l != '']


def record_position_traverse(position):
    buffer = []
    set_position(0)
    time.sleep(2)
    turn_on_logging()
    buffer += read_serial(2)
    set_position(position)
    buffer += read_serial(6)
    turn_off_logging()
    set_position(0)

    return buffer


def record_trajectory(trajectory):
    buffer = []
    set_speed(0)
    set_position(0)
    time.sleep(4)
    turn_on_logging()
    execute_trajectory(trajectory)
    buffer += read_serial(10)
    turn_off_logging()
    set_position(0)

    return buffer


def record_speed(speed):
    buffer = []
    set_speed(0)
    time.sleep(2)
    turn_on_logging()
    buffer += read_serial(2)
    set_speed(speed)
    buffer += read_serial(6)
    turn_off_logging()
    set_speed(0)

    return buffer

def plot_trajectory(buffer, params):
    print buffer
    Pr = [l.split(",")[0] for l in buffer]
    Pm = [l.split(",")[1] for l in buffer]
    T = [l.split(",")[4] for l in buffer]

    fig = plt.figure()
    plt.plot(Pr, label="Reference Position")
    plt.plot(Pm, label="Measured Position")
    plt.plot(T, label="Torque")
    plt.xlabel("Time (x5 ms)")
    plt.ylabel("Position (counts)")
    plt.legend()
    plt.title("Kp: %(Kp)s, Kd: %(Kd)s, Controller Period(ms): %(F)s" % params)
    plt.ylim([0,140])
    filename = "output/Kp%(Kp)sKd%(Kd)sF%(F)s" % params

    plt.savefig(filename + "T.png")

    plt.close(fig)


def plot_position_traverse(buffer, params, position):
    Pr = [l.split(",")[0] for l in buffer]
    Pm = [l.split(",")[1] for l in buffer]
    Vm = [l.split(",")[4] for l in buffer]

    fig = plt.figure()
    plt.plot(Pr, label="Reference Position")
    plt.plot(Pm, label="Measured Position")
    plt.plot(Vm, label="Velocity")
    plt.xlabel("Time (x5 ms)")
    plt.ylabel("Position (counts)")
    plt.legend()
    plt.title("Kp: %(Kp)s, Kd: %(Kd)s, Controller Period(ms): %(F)s" % params)
    plt.ylim([0,position+20])
    filename = "output/Kp%(Kp)sKd%(Kd)sF%(F)s" % params

    plt.savefig(filename + "P" + str(position) + ".png")

    plt.close(fig)


def plot_speed(buffer, params, speed):
    print buffer
    Sr = [l.split(",")[2] for l in buffer]
    Sm = [l.split(",")[3] for l in buffer]

    fig = plt.figure()
    plt.plot(Sr, label="Reference Speed")
    plt.plot(Sm, label="Measured Speed")
    plt.xlabel("Time (x5 ms)")
    plt.ylabel("Speed (counts/sec)")
    plt.legend()
    plt.title("Kp: %(Kp)s, Kd: %(Kd)s, Controller Period(ms): %(F)s" % params)
    filename = "output/Kp%(Kp)sKd%(Kd)sF%(F)s" % params

    plt.savefig(filename + "S" + str(speed) + ".png")

    plt.close(fig)

def set_params(Kp, Kd, F):
    _send_command('p'+str(Kp))
    _send_command('d'+str(Kd))
    _send_command('f'+str(F))


_send_command('e-')
flush_input()

"""
params = {"F": 50, "Kp": 5.0, "Kd": 5.0}
set_params(**params)
plot_speed(record_speed(255), params, 255)

params = {"F": 50, "Kp": 5.0, "Kd": 5.0}
set_params(**params)
plot_speed(record_speed(20), params, 20)

params = {"F": 50, "Kp": 0.1, "Kd": 0.1}
set_params(**params)
plot_speed(record_speed(20), params, 20)

params = {"F": 50, "Kp": 0.5, "Kd": 0.5}
set_params(**params)
plot_speed(record_speed(120), params, 120)

params = {"F": 50, "Kp": 0.2, "Kd": 0.2}
set_params(**params)
plot_speed(record_speed(120), params, 120)

params = {"F": 50, "Kp": 0.2, "Kd": 0.2}
set_params(**params)
plot_position_traverse(record_position_traverse(50), params, 50)

params = {"F": 50, "Kp": 0.2, "Kd": 0.2}
set_params(**params)
plot_position_traverse(record_position_traverse(50), params, 50)

params = {"F": 20, "Kp": 0.4, "Kd": 0.1}
set_params(**params)
plot_position_traverse(record_position_traverse(50), params, 50)

params = {"F": 20, "Kp": 0.5, "Kd": 0.1}
set_params(**params)
plot_position_traverse(record_position_traverse(50), params, 50)

params = {"F": 20, "Kp": 0.15, "Kd": 0.15}
set_params(**params)
plot_position_traverse(record_position_traverse(50), params, 50)
"""

params = {"F": 20, "Kp": 0.15, "Kd": 0.25}
set_params(**params)
print plot_trajectory(record_trajectory("r120,w23,r0,w23,r20"), params)

ser.close()