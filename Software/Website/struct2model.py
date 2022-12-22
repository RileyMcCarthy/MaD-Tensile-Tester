import inspect
import importlib
import argparse

from ctypes import Structure, c_int, c_float, c_char


def generate_model(cls, foreign_key=None):
    # Check if the class is a ctypes.Structure
    if issubclass(cls, Structure):
        # Create a string to hold the class definition
        class_def = ""

        # Add the class header to the string
        class_name = f"Model_{cls.__name__}"
        class_def += f"class {class_name}(Base):\n"
        class_def += f"    __tablename__ = \"{cls.__name__}\"\n"
        class_def += f"    id = Column(Integer, primary_key=True)\n"

        # Iterate over the fields of the ctypes.Structure
        for field_name, field_type in cls._fields_:
            # Check if the field is another ctypes.Structure
            if issubclass(field_type, Structure):
                # Generate a model for the nested structure
                nested_model = generate_model(
                    field_type, foreign_key=field_name)
                # Add the relationship to the class definition string
                class_def += (
                    f"    {field_name} = relationship("
                    f"{nested_model[0]}, backref='{cls.__name__.lower()}')\n")
            else:
                if issubclass(field_type, c_int):
                    column_type = "Integer"
                elif issubclass(field_type, c_float):
                    column_type = "Float"
                elif issubclass(field_type, c_char):
                    column_type = "String"
                else:
                    # Handle other ctypes types as needed
                    continue
                # Check if the field is a foreign key
                if foreign_key:
                    class_def += (
                        f"    {field_name} = Column(\"{field_name}\", {column_type})"
                        f".ForeignKey({foreign_key})\n")
                else:
                    class_def += (
                        f"    {field_name} = Column(\"{field_name}\", {column_type})\n")

        # Return the class name and class definition as a tuple
        return (class_name, class_def)


# Create the parser
parser = argparse.ArgumentParser()

# Define a command line argument
parser.add_argument('module', type=str, help='Output file name')
parser.add_argument('-o', '--output', type=str,
                    help='The output name for the script')


# Parse the command line arguments
args = parser.parse_args()

# Import the module containing the classes
definitions = importlib.import_module("{}".format(args.module))

with open('{}'.format(args.output), 'w') as f:
    f.write("from flask_sqlalchemy import Column, Integer, Float, String, ForeignKey, Base\nfrom flask_sqlalchemy.orm import relationship\n")
    # Iterate over all classes defined in the file
    for name, cls in inspect.getmembers(definitions, inspect.isclass):
        # Check if the class is a ctypes.Structure
        if hasattr(cls, '_fields_') and issubclass(cls, Structure):
            # Generate a model for the structure
            gen = generate_model(cls)
            # Print the generated model class to a new Python file
            print("Writing model for", gen[0])
            f.write(gen[1])
