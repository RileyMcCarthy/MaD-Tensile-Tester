r"""Wrapper for MotionPlanningDefinition.h

Generated with:
/home/mad/.local/bin/ctypesgen MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h -o MaD_Software/definitions/MotionPlanningDefinition.py --no-embed-preamble

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

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 5
for _lib in _libs.values():
    if not _lib.has("line", "cdecl"):
        continue
    line = _lib.get("line", "cdecl")
    line.argtypes = [c_float, POINTER(c_float)]
    line.restype = c_float
    break

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 6
for _lib in _libs.values():
    if not _lib.has("sigmoid", "cdecl"):
        continue
    sigmoid = _lib.get("sigmoid", "cdecl")
    sigmoid.argtypes = [c_float, POINTER(c_float)]
    sigmoid.restype = c_float
    break

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 1
try:
    FUNCTION_COUNT = 2
except:
    pass

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 2
try:
    QUARTET_FUNC_LINE = 0
except:
    pass

# /home/mad/Tensile-Testing-Machine/Firmware/MaD_Firmware/Librarys/MotionPlanning/MotionPlanningDefinition.h: 3
try:
    QUARTET_FUNC_SIGMOIDAL = 1
except:
    pass

# No inserted files

# No prefix-stripping

