import re
import argparse

# a mapping of C types to ctypes data types
ctype_map = {
    'char': 'c_char',
    'short': 'c_short',
    'int': 'c_int',
    'long': 'c_long',
    'long long': 'c_longlong',
    'unsigned char': 'c_ubyte',
    'unsigned short': 'c_ushort',
    'unsigned int': 'c_uint',
    'unsigned long': 'c_ulong',
    'unsigned long long': 'c_ulonglong',
    'float': 'c_float',
    'double': 'c_float',
    'void': 'c_void_p',
    'bool': 'c_bool',
}


def parse_defines(data):
    # the regular expression to match #define statements with a non-empty value
    define_regex = re.compile(
        r'#define\s+([\w_]+)\s+(.+?)\s*(?:/\*(.*?)\*/)?\s*$',
        re.MULTILINE
    )

    # find all #define statements in the input file
    matches = define_regex.finditer(data)

    # the output string
    output_string = ''

    # loop over the matches
    for match in matches:
        # the name of the define
        define_name = match.group(1)
        # the value of the define
        define_value = match.group(2)

        # skip this define if it does not have a value
        if not define_value:
            continue

        define_value = define_value.replace("//", "#")

        # add the define to the output string
        output_string += f'{define_name} = {define_value}\n'

    return output_string


def parse_enums(data):
    # the regular expression to match comments
    comment_regex = re.compile(
        r'//.*?$|/\*.*?\*/',
        re.DOTALL | re.MULTILINE
    )

    # remove all comments from the input string
    data = comment_regex.sub('', data)

    # the regular expression to match enum statements
    enum_regex = re.compile(
        r'enum\s+([\w_]+)?\s*{(.*?)}',
        re.DOTALL
    )

    # the regular expression to match values in an enum statement
    value_regex = re.compile(
        r'([\w_]+)\s*(?:=\s*([^,]*))?,?',
        re.DOTALL
    )

    # find all enum statements in the input file
    matches = enum_regex.finditer(data)

    # the output string
    output_string = ''

    # loop over the matches
    for match in matches:
        # the name of the enum (if any)
        enum_name = match.group(1)
        # the values of the enum
        enum_values = match.group(2)

        # initialize the counter variable to 0
        counter = 0

        # generate the class definition for the enum
        if enum_name:
            output_string += f'class {enum_name}(c_int):\n'

            # find all values in the enum statement
            values = value_regex.finditer(enum_values)
            
            output_string += '\tenum = {\n'
            # loop over the values
            for value in values:
                # the name of the value
                value_name = value.group(1)
                # the value of the value (if any)
                value_value = value.group(2)

                # remove the enum name prefix from the value name
                if value_name.lower().startswith(enum_name.lower() + '_'):
                    value_name = value_name[len(enum_name) + 1:]

                # add the value to the output string
                if value_value:
                    counter = value_value
                output_string += f'\t\t{counter}: "{value_name}",\n'
                counter += 1
            # add a newline to the output string
            output_string += '\t}\n'

    return output_string


def parse_structs(data):
    # the regular expression to match structure definitions
    struct_regex = re.compile(
        r'typedef\s+struct\s+([\w_]+)\s*{(.*?)}(\s+[\w_]+);',
        re.DOTALL
    )

    # the regular expression to match fields in a structure definition
    field_regex = re.compile(
        r'([\w_\[\]*]+)\s+([\w_]+)\s*(?:\[([^\]]*)\])?\s*(?:=\s*([^;]*))?;',
        re.DOTALL
    )
    # find all structure definitions in the input file
    matches = struct_regex.finditer(data)

    # a set of defined structures
    defined_structs = set()

    # the output string
    output_string = ''

    # loop over the matches
    for match in matches:
        # the name of the structure
        struct_name = match.group(1)
        # the definition of the structure
        struct_def = match.group(2)

        # skip this structure if it has already been defined
        if struct_name in defined_structs:
            continue

        # generate the class definition for the structure
        output_string += f'class {struct_name}(Structure):\n'
        output_string += '    _fields_ = [\n'

        # find all fields in the structure definition
        fields = field_regex.finditer(struct_def)

        # loop over the fields
        for field in fields:
            # the type of the field
            field_type = field.group(1)
            # the name of the field
            field_name = field.group(2)
            # the array dimensions of the field (if any)
            field_dims = field.group(3)
            # the default value of the field
            field_default = field.group(4)

            # map the C type to a ctypes data type
            ctype = ctype_map.get(field_type, field_type)

            # if the field type is a structure, add the structure to the set of defined structures
            if ctype in struct_regex.findall(data):
                defined_structs.add(ctype)

            # add the array dimensions to the ctype
            if field_dims:
                ctype += f' * {field_dims}'

            # add the field definition to the output string
            if field_default:
                output_string += f'        ("{field_name}", {ctype}, {field_default}),\n'
            else:
                output_string += f'        ("{field_name}", {ctype}),\n'

        # add the closing bracket and newline to the output string
        output_string += '    ]\n\n'
    return output_string


# Create the parser
parser = argparse.ArgumentParser()

# Define a command line argument
parser.add_argument('header', type=str, help='header file name')
parser.add_argument('-o', '--output', type=str,
                    help='The output name for the script')


# Parse the command line arguments
args = parser.parse_args()

# read the input file
with open(args.header, 'r') as f:
    data = f.read()

filestr = 'from ctypes import Structure, c_int, c_float, c_char, c_bool\n'
filestr += parse_defines(data)
filestr += parse_enums(data)
filestr += parse_structs(data)
# write the output string to the output file
with open(args.output, 'w') as f:
    f.write(filestr)
