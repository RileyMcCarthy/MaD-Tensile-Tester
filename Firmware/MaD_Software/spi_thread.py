# spitest.py
# A brief demonstration of the Raspberry Pi SPI interface, using the Sparkfun
# Pi Wedge breakout board and a SparkFun Serial 7 Segment display:
# https://www.sparkfun.com/products/11629

from definitions.MonitorDefinition import MonitorData
from Helpers import print_ctypes_obj
import spidev
import time
import RPi.GPIO as GPIO
import ctypes

monitorData = MonitorData()


def startThread():
    GPIO.setmode(GPIO.BCM)
    # GPIO 10 set up as input. It is pulled up to stop false signals
    GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    # We only have SPI bus 0 available to us on the Pi
    bus = 0

    # Device is the chip select pin. Set to 0 or 1, depending on the connections
    device = 0

    # Enable SPI
    spi = spidev.SpiDev()
    spi.close()

    # Open a connection to a specific bus and device (chip select pin)
    spi.open(bus, device)

    # Set SPI speed and modewwww
    spi.max_speed_hz = 1000000
    spi.mode = 0

    while (1):
        # wait for data to be ready
        if GPIO.input(22) == 0:
            monitorData = MonitorData.from_buffer_copy(
                bytearray(spi.readbytes(ctypes.sizeof(MonitorData))))
           # print_ctypes_obj(monitorData)
