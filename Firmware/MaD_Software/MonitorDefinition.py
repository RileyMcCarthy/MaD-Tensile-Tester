r"""Wrapper for MonitorDefinition.h

Generated with:
/home/riley/.local/bin/ctypesgen MaD_Firmware/include/Utility/MonitorDefinition.h -o MaD_Software/MonitorDefinition.py --no-embed-preamble

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python

from ctypes_preamble import *
from ctypes_preamble import _variadic_function

# End preamble

_libs = {}
_libdirs = []

# Begin loader

from ctypes_loader import *

# End loader

add_library_search_dirs([])

# No libraries

# No modules

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 12
class struct_monitor_data_t(Structure):
    pass

struct_monitor_data_t.__slots__ = [
    'forceRaw',
    'encoderRaw',
    'force',
    'position',
    'timems',
    'timeus',
]
struct_monitor_data_t._fields_ = [
    ('forceRaw', c_int),
    ('encoderRaw', c_int),
    ('force', c_float),
    ('position', c_float),
    ('timems', c_int),
    ('timeus', c_int),
]

MonitorData = struct_monitor_data_t# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 12

monitor_data_t = struct_monitor_data_t# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/include/Utility/MonitorDefinition.h: 12

# No inserted files

# No prefix-stripping

