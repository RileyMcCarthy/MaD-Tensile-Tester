import ctypes
from struct import *
from definitions.JSON import *
from definitions.MonitorDefinition import *
from definitions.StateMachineDefinition import *
from definitions.CommunicationDefinition import *
from Helpers import *


class MaD_Serial:
    SYNC_BYTE = 0x55
    WRITE_BYTE = 128

    def __init__(self, rx, tx, lock):
        self.rx = rx
        self.tx = tx
        self.lock = lock

    @ staticmethod
    def convert_to_bytes(st):
        buffer = ctypes.create_string_buffer(ctypes.sizeof(st))
        ctypes.memmove(buffer, ctypes.addressof(st), ctypes.sizeof(st))
        return buffer.raw

    @ staticmethod
    def structEq(comp, other):
        return MaD_Serial.convert_to_bytes(comp) == MaD_Serial.convert_to_bytes(other)

    def send_write_cmd(self, cmd, data):
        with self.lock:
            try:
                self.tx.put(bytearray([self.SYNC_BYTE, cmd | self.WRITE_BYTE]))
                self.tx.put(data)
            except Exception as error:
                print("Exception while sending write command: " + str(error))

    def send_read_cmd(self, cmd):
        with self.lock:
            self.tx.put(bytearray([self.SYNC_BYTE, cmd]))

    def read_data(self, cmd):
        try:
            data = self.rx[cmd]
            self.rx[cmd] = None
            return data
        except KeyError:
            pass
        return None
