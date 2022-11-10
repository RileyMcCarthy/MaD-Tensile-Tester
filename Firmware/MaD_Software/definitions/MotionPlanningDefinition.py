r"""Wrapper for MotionPlanningDefinition.h

Generated with:
/home/riley/.local/bin/ctypesgen MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h -o MaD_Software/definitions/MotionPlanningDefinition.py --no-embed-preamble

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

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 9
for _lib in _libs.values():
    if not _lib.has("line", "cdecl"):
        continue
    line = _lib.get("line", "cdecl")
    line.argtypes = [c_float, POINTER(c_float)]
    line.restype = c_float
    break

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 10
for _lib in _libs.values():
    if not _lib.has("sigmoid", "cdecl"):
        continue
    sigmoid = _lib.get("sigmoid", "cdecl")
    sigmoid.argtypes = [c_float, POINTER(c_float)]
    sigmoid.restype = c_float
    break

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 18
class struct_functioninfo_s(Structure):
    pass

struct_functioninfo_s.__slots__ = [
    'name',
    'func',
    'args_count',
    'args',
]
struct_functioninfo_s._fields_ = [
    ('name', c_char * int(20)),
    ('func', CFUNCTYPE(UNCHECKED(c_float), c_float, POINTER(c_float))),
    ('args_count', c_int),
    ('args', (c_char * int(20)) * int(10)),
]

FunctionInfo = struct_functioninfo_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 18

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 21
for _lib in _libs.values():
    try:
        functions = (POINTER(FunctionInfo)).in_dll(_lib, "functions")
        break
    except:
        pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 1
try:
    FUNCTION_COUNT = 2
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 2
try:
    QUARTET_FUNC_LINE = 0
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 3
try:
    QUARTET_FUNC_SIGMOIDAL = 1
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 5
try:
    FUNCTION_MAX_NAME_LENGTH = 10
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 6
try:
    FUNCTION_MAX_ARGS = 10
except:
    pass

# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 7
try:
    FUNCTION_MAX_ARG_LENGTH = 20
except:
    pass

functioninfo_s = struct_functioninfo_s# /home/riley/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 18

# No inserted files

# No prefix-stripping

