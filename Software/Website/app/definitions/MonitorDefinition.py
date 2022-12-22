from ctypes import Structure, c_int, c_float, c_char, c_bool
class monitor_data_t(Structure):
    _fields_ = [
        ("forceRaw", c_int),
        ("encoderRaw", c_int),
        ("forcemN", c_int),
        ("encoderum", c_int),
        ("force", c_float),
        ("position", c_float),
        ("setpoint", c_int),
        ("timems", c_int),
        ("timeus", c_int),
        ("log", c_int),
    ]

class MonitorDataPacket(Structure):
    _fields_ = [
        ("forcemN", c_int),
        ("encoderum", c_int),
        ("setpointum", c_int),
        ("timeus", c_int),
        ("log", c_int),
    ]

