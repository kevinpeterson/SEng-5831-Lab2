import serial
import time
import matplotlib.pyplot as plt

ser = serial.Serial('/dev/tty.usbmodem00055903', 9600)

print("connected to: " + ser.portstr)

def read_serial(seconds):
    buffer = []
    end_time = time.time() + seconds
    while time.time() < end_time:
        buffer.append(ser.readline().strip())
    ser.write('l\r')
    return clean_serial_buffer(buffer)


def record_position_traverse(position):
    buffer = []
    ser.write('r0,\r')
    time.sleep(2)

    ser.write('l,\r')
    buffer += read_serial(2)
    ser.write('l,\r')
    ser.write('r'+str(position)+'\r')
    buffer += read_serial(6)
    ser.write('r0,\r')

    return buffer


def record_speed(speed):
    buffer = []
    ser.write('s0,\r')
    time.sleep(2)

    ser.write('l,\r')
    buffer += read_serial(2)
    ser.write('l,\r')
    ser.write('s'+str(speed)+'\r')
    buffer += read_serial(6)
    ser.write('s0,\r')

    return buffer


def clean_serial_buffer(buffer):
    clean_buffer = []
    for l in buffer:
        try:
            for token in l.split(","):
                float(token)
            clean_buffer.append(l)
        except ValueError:
            pass
    return clean_buffer


def plot_position_traverse(buffer):
    Pr = [l.split(",")[0] for l in buffer]
    Pm = [l.split(",")[1] for l in buffer]
    T = [l.split(",")[4] for l in buffer]

    plt.plot(Pr)
    plt.plot(Pm)
    plt.plot(T)

    plt.show()


def plot_speed(buffer):
    Pr = [l.split(",")[2] for l in buffer]
    Pm = [l.split(",")[3] for l in buffer]
    T = [l.split(",")[4] for l in buffer]

    plt.plot(Pr)
    plt.plot(Pm)
    plt.plot(T)

    plt.show()


plot_speed(record_speed(20))
plot_speed(record_speed(80))
plot_speed(record_speed(140))
plot_speed(record_speed(200))
plot_speed(record_speed(255))


ser.close()