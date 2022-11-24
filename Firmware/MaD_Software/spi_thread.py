from definitions.MonitorDefinition import MonitorData
from Helpers import print_ctypes_obj
import ctypes
import json
import time
import pigpio

pi = pigpio.pi()
if not pi.connected:
    print("Can't connected to pigpio daemon")
    exit()
# GPIO 22 set up as input. It is pulled up to stop false signals
pi.set_mode(22, pigpio.INPUT)
pi.set_pull_up_down(22, pigpio.PUD_UP)

# Enable SPI
spi_handler = pi.spi_open(0, 50000, 0) # Channel, baud, flags


#f = open("data_cache.csv", "w")
#f.write("time,position,force,setpoint\n")
#f.close()

def read_spi(gpio, level, tick):
    num_bytes, bytes_in = pi.spi_read(spi_handler, ctypes.sizeof(MonitorData))
    if num_bytes == ctypes.sizeof(MonitorData):
        monitorData = MonitorData.from_buffer_copy(bytearray(bytes_in))
        data = json.dumps({"time": monitorData.timems/1000.0, "position": monitorData.position,"force": monitorData.force, "setpoint": monitorData.setpoint})
        print(data)
    else:
        print("Error reading SPI, bytes to not match. Got "+str(num_bytes)+" bytes, expected "+str(ctypes.sizeof(MonitorData)))

pi.callback(22, pigpio.FALLING_EDGE, read_spi)

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    pi.spi_close(spi_handler)        