r"""Wrapper for MonitorDefinition.h

Generated with:
/home/mad/.local/bin/ctypesgen MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/definitions/MonitorDefinition.py --no-embed-preamble

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python

from .ctypes_preamble import *
from .ctypes_preamble import _variadic_function

# End preamble

_libs = {}
_libdirs = []

# Begin loader

from .ctypes_loader import *

# End loader

add_library_search_dirs([])

# No libraries

# No modules

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 16
class struct_monitor_data_t(Structure):
    pass

struct_monitor_data_t.__slots__ = [
    'forceRaw',
    'encoderRaw',
    'forcemN',
    'encoderum',
    'force',
    'position',
    'setpoint',
    'timems',
    'timeus',
    'log',
]
struct_monitor_data_t._fields_ = [
    ('forceRaw', c_int),
    ('encoderRaw', c_int),
    ('forcemN', c_int),
    ('encoderum', c_int),
    ('force', c_float),
    ('position', c_float),
    ('setpoint', c_int),
    ('timems', c_uint),
    ('timeus', c_uint),
    ('log', c_int),
]

MonitorData = struct_monitor_data_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 16

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 26
class struct_DataPacket_s(Structure):
    pass

struct_DataPacket_s.__slots__ = [
    'forcemN',
    'encoderum',
    'setpointum',
    'timeus',
    'log',
]
struct_DataPacket_s._fields_ = [
    ('forcemN', c_int),
    ('encoderum', c_int),
    ('setpointum', c_int),
    ('timeus', c_uint),
    ('log', c_int),
]

MonitorDataPacket = struct_DataPacket_s# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 26

monitor_data_t = struct_monitor_data_t# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 16

DataPacket_s = struct_DataPacket_s# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 26

# No inserted files

# No prefix-stripping

