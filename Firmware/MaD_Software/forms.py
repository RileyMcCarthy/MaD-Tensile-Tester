from wtforms import Form, BooleanField, StringField, PasswordField, validators, SelectField, SubmitField, FormField, FieldList
from flask_wtf import FlaskForm
from Helpers import print_ctypes_obj
import re
import ctypes
from definitions.MotionPlanningDefinition import *
from definitions.StateMachineDefinition import *


def title(title):
    return re.sub('([A-Z])', r' \1', title).title()


def getEnum(prefix):
    options = []
    for var in globals():
        if prefix in var:
            options.append(var.replace(prefix, ""))
    return options


class ConnectForm(FlaskForm):
    port = SelectField('Serial Port', choices=[''], validate_choice=False)
    baud = SelectField('Baud Rate', choices=[
                       9600, 14400, 19200, 38400, 57600, 115200, 230400, 256000], validate_choice=False)


def FunctionForm(func, data):
    str = """
        <div class="row-xl">
            <div class="col">
        """.format(data)
    functions = getEnum('FUNC_MANUAL_')
    print(func)
    #print("seelected: "+str(func)+", data:",+str(data))
    for index, function in enumerate(functions):
        str += '<button type="button" class="btn m-2 {}" id="{}" onclick=func_manual()>{}</button>'.format("btn-success" if index !=
                                                                                                           func else "btn-primary", index, function)
    str += """
            </div>
        </div>"""
    return str


def structure_to_form(obj, callback=None):
    class StructureForm(FlaskForm):
        pass

    def getField(name, obj):
        if hasattr(obj, '_fields_'):
            return FormField(structure_to_form(obj))
        elif issubclass(type(obj), ctypes.Array):
            return FieldList(getField(name, obj[0]), default=obj)
        else:
            return StringField(title(fname), default=obj)

    # Print obj name as title here!
    for fdesc in obj._fields_:
        # Get the next field descriptor
        fname = fdesc[0]
        ftype = fdesc[1]
        if fname[0] == "_":  # Skip fields that begin with _*
            continue
        obj2 = getattr(obj, fname)
        print("name: "+fname + ",type"+str(type(obj2)))
        if callback is not None and fname in callback:
            print("using callback")
            setattr(StructureForm, fname, callback[fname](fname, obj, obj2))
        else:
            setattr(StructureForm, fname, getField(fname, obj2))
    return StructureForm


def MotionProfileForm(profile):
    # callbacks should replace getField()

    def getParameters(function, parameters):
        # Get params over serial on setup!!!

        lineInfo = FunctionInfo()
        lineInfo.name = b'Line'
        lineInfo.args[0] = ctypes.create_string_buffer(
            b'distance',  ctypes.sizeof(lineInfo.args[0]))
        lineInfo.args[1] = ctypes.create_string_buffer(
            b'strain rate', ctypes.sizeof(lineInfo.args[0]))

        sigmoidInfo = FunctionInfo()
        sigmoidInfo.name = b'Sigmoid'
        sigmoidInfo.args[0] = ctypes.create_string_buffer(
            b'distance', ctypes.sizeof(sigmoidInfo.args[0]))
        sigmoidInfo.args[1] = ctypes.create_string_buffer(
            b'strain rate', ctypes.sizeof(sigmoidInfo.args[1]))
        sigmoidInfo.args[2] = ctypes.create_string_buffer(
            b'error', ctypes.sizeof(sigmoidInfo.args[2]))
        functions = [lineInfo, sigmoidInfo]

        #fields = FieldList(FunctionInfo, default=functions)
        # print_ctypes_obj(functions[function])
        return StringField("Parameters", default="hi")
        return FieldList(StringField(), default=["arg1, 'arg2"])

    return structure_to_form(profile, {
        "sets": lambda name, profile, sets: FieldList(FormField(
            structure_to_form(sets[0], {
                "quartets": lambda name, set, quartets: FieldList(FormField(
                    structure_to_form(quartets[0], {
                        "function": lambda name, quartet, quartets: SelectField('Function', choices=getEnum("QUARTET_FUNC_")),
                        # "parameters": lambda name, quartet, parameters: StringField("Parameters", default="hi")
                    })
                ), default=quartets)
            })
        ), default=sets)
    })
