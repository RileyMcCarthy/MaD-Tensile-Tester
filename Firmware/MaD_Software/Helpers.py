#!/usr/bin/env python3
import serial.tools.list_ports
import re
from SerialHelpers import *

# This method of finding ports may break some serial ports


def list_ports():
    ports = serial.tools.list_ports.comports()
    result = ["/dev/ttyS0", "/dev/serial0"]
    for port in ports:
        result.append("/dev/"+port.name)
    return result


def crc8(data):
    if (data == None):
        raise ValueError('Data is not valid: '+str(data))
    crc = 0
    for i in range(len(data)):
        byte = data[i]
        for b in range(8):
            fb_bit = (crc ^ byte) & 0x01
            if fb_bit == 0x01:
                crc = crc ^ 0x18
            crc = (crc >> 1) & 0x7f
            if fb_bit == 0x01:
                crc = crc | 0x80
            byte = byte >> 1
    return crc


def print_ctypes_obj(obj, level=0):

    delta_indent = "    "
    indent = delta_indent*level

    # Assess wether the object is an array, a structure or an elementary type
    if issubclass(type(obj), ctypes.Array):
        print('{}ARRAY {}'.format(indent, obj))
        for obj2 in obj:
            print_ctypes_obj(obj2, level+1)

    elif hasattr(obj, '_fields_'):
        print('{}STRUCTURE {}'.format(indent, obj))
        for fdesc in obj._fields_:
            # Get the next field descriptor
            fname = fdesc[0]
            ftype = fdesc[1]
            if len(fdesc) == 3:
                fbitlen = fdesc[2]
            else:
                fbitlen = 8*ctypes.sizeof(ftype)
            obj2 = getattr(obj, fname)
            print('{}FIELD {} (type={}, bitlen={})'.format(
                indent+delta_indent, fname, ftype, fbitlen))
            print_ctypes_obj(obj2, level+2)

    else:
        print('{}VALUE = {} (type={})'.format(indent, obj, type(obj)))


def loadMotionProfile():
    motionProfile = MotionProfile()
    motionProfile.name = b'Test1.mp'
    motionProfile.number = 5
    motionProfile.setCount = 1
    motionProfile.sets[0].name = b'Test set 1'
    motionProfile.sets[0].number = 1
    motionProfile.sets[0].executions = 2
    motionProfile.sets[0].quartetCount = 2

    motionProfile.sets[0].quartets[0].name = b'qrt1.qrt'
    motionProfile.sets[0].quartets[0].function = 0
    motionProfile.sets[0].quartets[0].parameters[0] = 20
    motionProfile.sets[0].quartets[0].parameters[1] = 20
    motionProfile.sets[0].quartets[0].dwell = 500

    motionProfile.sets[0].quartets[1].name = b'qrt2.qrt'
    motionProfile.sets[0].quartets[1].function = 0
    motionProfile.sets[0].quartets[1].parameters[0] = -20
    motionProfile.sets[0].quartets[1].parameters[1] = 10
    motionProfile.sets[0].quartets[1].dwell = 500
    return motionProfile


def loadTestProfile():
    testProfile = TestProfile()
    testProfile.name = b'Test_Profile_1'
    testProfile.machineProfile = loadMachineProfile()
    testProfile.motionProfile = loadMotionProfile()
    testProfile.comment = "Demo test profile"
    return testProfile


def loadMachineProfile():
    machineProfile = MachineProfile()
    machineProfile.name = b'Tensile_Test_1'

    machineProfile.number = 1

    # MachineConfiguration
    machineProfile.configuration.motorType = b"640-DST"

    machineProfile.configuration.maxMotorTorque = 3.82
    machineProfile.configuration.maxMotorRPM = 5000
    machineProfile.configuration.gearDiameter = 40.0
    machineProfile.configuration.gearPitch = 2
    machineProfile.configuration.systemIntertia = 0.00121
    machineProfile.configuration.staticTorque = 0.558
    machineProfile.configuration.load = 37.8

    machineProfile.configuration.positionEncoderType = b"QuadEncoder"

    machineProfile.configuration.positionEncoderStepsPerRev = 4 * 2048

    machineProfile.configuration.forceGauge = b"DS2-5N"

    machineProfile.configuration.forceGaugeScaleFactor = -657.959
    machineProfile.configuration.forceGaugeZeroFactor = 16119601

    # MachinePerformance
    machineProfile.performance.minPosition = 0.01
    machineProfile.performance.maxPosition = 50.5
    machineProfile.performance.maxVelocity = 200.5
    machineProfile.performance.maxAcceleration = 100.5
    machineProfile.performance.maxForceTensile = 5.0
    machineProfile.performance.maxForceCompression = 5.0
    machineProfile.performance.forceGaugeNeutralOffset = 0.9

    return machineProfile


inputHTMLPre = """
<div class="mb-1 row {}"">
    <div class="col-auto">
        <span class="text-center">{}</span>
    </div>"""

inputHTMLTitlePre = """
<div class="mb-3 row rounded {}">
    <div class="col-auto">
        <h5 class="text-center">{}</h5>
    </div>"""

inputHTMLPost = """</div>"""


def formatTitle(title):
    return re.sub('([A-Z])', r' \1', title).title()


def formatField(field, readonly):
    if readonly == True:
        if (type(field) == bool):
            if field == True:
                return '<i class="bi-check-square-fill"></i>'
            else:
                return '<i class="bi-check-square"></i>'
        else:
            return '<span type="text" class="col">{}</span>'.format(field)
    else:
        return '<input type="text" class="col value" aria-label="Default" aria-describedby="inputGroup-sizing-default" value="{}">'.format(field)


def test_structure(obj):
    for field in obj._fields_:
        print(field)


def structure_to_html(obj, readonly=True, callback=None, str=""):
    if hasattr(obj, '_fields_'):
        str += '<div class="container-fluid">'
        for fdesc in obj._fields_:
            # Get the next field descriptor
            fname = fdesc[0]
            ftype = fdesc[1]
            if fname[0] == "_":
                continue
            obj2 = getattr(obj, fname)
            if hasattr(obj2, '_fields_'):
                str += inputHTMLTitlePre.format(fname, formatTitle(fname))
                str = structure_to_html(obj2, readonly, callback, str)
            elif callback is not None and fname in callback:
                str += inputHTMLPre.format(fname, formatTitle(fname))
                str += callback[fname](obj2, readonly)
            else:
                str += inputHTMLPre.format(fname, formatTitle(fname))
                str += formatField(obj2, readonly)  # Field
            str += inputHTMLPost
        str += '</div>'
    else:
        return ""
    return str


def machine_profile_to_html(profile):
    return structure_to_html(profile, False)


def enum_to_html(prefix, selected, readonly):
    options = []
    for var in globals():
        if prefix in var:
            options.append(var.replace(prefix, ""))
    if selected > len(options):
        print("selected value not valid:"+str(selected))
        selected = 0

    if readonly == True:
        str = '<span type="text" class="col value">{}</span>'.format(
            options[selected])
    else:
        str = """<select class="col" id="{}">""".format(prefix)
        for option in options:
            if options[selected] == option:
                str += """<option selected>{}</option>""".format(option)
            else:
                str += """<option>{}</option>""".format(option)
        str += """</select>"""
    return str


def structure_find(obj, field):
    if hasattr(obj, '_fields_'):
        for fdesc in obj._fields_:
            # Get the next field descriptor
            fname = fdesc[0]
            ftype = fdesc[1]
            if field != fname:
                continue
            obj2 = getattr(obj, fname)
            print(obj2)
            return "{}".format(obj2)
    return None


def machine_status_to_html(status):
    return structure_to_html(status, True, {
        "state": lambda sel, readonly: enum_to_html("STATE_", sel, readonly),
        "esdTravelLimit": lambda sel, readonly: enum_to_html("MOTION_LIMIT_", sel, readonly),
        "status": lambda sel, readonly: enum_to_html("MOTIONSTATUS_", sel, readonly),
        "condition": lambda sel, readonly: enum_to_html("CONDITION_", sel, readonly),
        "mode": lambda sel, readonly: enum_to_html("MODE_", sel, readonly),
    })
