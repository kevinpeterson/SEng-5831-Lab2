import serial
import time
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

ser = serial.Serial('/dev/tty.usbmodem00055903', 9600)

print("connected to: " + ser.portstr)

def flush_input():
    time.sleep(1)
    ser.flushInput()

def _send_command(command):
    ser.write(command+'\r')

def turn_off_logging():
    _send_command('l')

def turn_on_logging():
    _send_command('l,')

def set_position(pos):
    _send_command('r'+str(pos))

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


def plot_position_traverse(buffer, params):
    Pr = [l.split(",")[0] for l in buffer]
    Pm = [l.split(",")[1] for l in buffer]
    T = [l.split(",")[4] for l in buffer]

    fig = plt.figure()
    plt.plot(Pr)
    plt.plot(Pm)
    plt.plot(T)
    plt.xlabel("Time (x5 ms)")
    plt.ylabel("Position (counts)")
    plt.title("Kp: %(Kp)s, Kd: %(Kd)s, Controller Period(ms): %(F)s" % params)

    pp.savefig(fig)


def plot_speed(buffer, params):
    print buffer
    Pr = [l.split(",")[2] for l in buffer]
    Pm = [l.split(",")[3] for l in buffer]
    T = [l.split(",")[4] for l in buffer]

    fig = plt.figure()
    plt.plot(Pr)
    plt.plot(Pm)
    plt.plot(T)
    plt.xlabel("Time (x5 ms)")
    plt.ylabel("Speed (counts/sec)")
    plt.title("Kp: %(Kp)s, Kd: %(Kd)s, Controller Period(ms): %(F)s" % params)

    pp.savefig(fig)
    fig.close()

def set_params(Kp, Kd, F):
    _send_command('p'+str(Kp))
    _send_command('d'+str(Kd))
    _send_command('f'+str(F))


_send_command('e-')
flush_input()

pp = PdfPages('figures.pdf')


for k in [0.1,0.15,0.2,0.25,0.3]:
    params = {
        "F": 20,
        "Kp": k,
        "Kd": k
    }
    set_params(**params)
    plot_speed(record_speed(20), params)
    plot_speed(record_speed(80), params)
    plot_speed(record_speed(140), params)
    plot_speed(record_speed(200), params)
    plot_speed(record_speed(255), params)


pp.close()

ser.close()