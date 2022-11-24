import time
import pigpio
I2C_ADDR = 0x13
SDA = 18
SCL = 19


def i2c(id, tick):
    global pi
    print("i2c callback")
    status, n, bytes = pi.bsc_i2c(I2C_ADDR)
    print("got bytres:"+str(bytes))


pi = pigpio.pi()
if not pi.connected:
    exit()
pi.set_pull_up_down(SDA, pigpio.PUD_UP)
pi.set_pull_up_down(SCL, pigpio.PUD_UP)
# Respond to BSC slave activity
e = pi.event_callback(pigpio.EVENT_BSC, i2c)
pi.bsc_i2c(I2C_ADDR)  # Configure BSC as I2C slave

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    e.cancel()
    pi.bsc_i2c(0)  # Disable BSC peripheral
    pi.stop()
