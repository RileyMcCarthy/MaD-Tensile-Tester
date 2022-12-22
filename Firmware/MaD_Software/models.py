from flask_sqlalchemy import SQLAlchemy
from app import app, db
from ctypes import Structure, c_int, c_float, c_char
from definitions.StateMachineDefinition import MachineState


def generate_model(structure):
    # Create a new class that extends Flask-SQLAlchemy's Model class
    cls = type(structure.__name__, (db.Model,), {})

    # Add columns to the class for each field in the ctypes.Structure
    for field_name, field_type in structure._fields_:
        if issubclass(field_type, Structure):
            # If the field is a ctypes.Structure, generate a Flask-SQLAlchemy model for it and add it as a relationship
            inner_cls = generate_model(field_type)
            column = db.relationship(
                inner_cls, backref=field_name, uselist=False, cascade="all, delete-orphan")
        elif issubclass(field_type, c_int):
            column_type = db.Integer
        elif issubclass(field_type, c_float):
            column_type = db.Float
        elif issubclass(field_type, c_char):
            column_type = db.String(len(field_type))
        else:
            # Handle other ctypes types as needed
            continue

        column = db.Column(column_type, primary_key=field_name == 'id')
        setattr(cls, field_name, column)

    # Create a function that converts a ctypes.Structure to an instance of the generated Flask-SQLAlchemy model
    def from_ctypes(self, structure):
        for field_name, field_type in structure._fields_:
            value = getattr(structure, field_name)
            if issubclass(field_type, Structure):
                # If the field is a ctypes.Structure, convert it to a Flask-SQLAlchemy model and set it as the value
                inner_model = field_type()
                inner_cls = generate_model(field_type)
                inner_instance = inner_cls()
                inner_instance.from_ctypes(inner_model)
                value = inner_instance
            elif issubclass(field_type, c_char):
                value = value.decode('utf-8')
            setattr(self, field_name, value)

    # Add the from_ctypes function to the generated Flask-SQLAlchemy model
    setattr(cls, 'from_ctypes', from_ctypes)

    return cls


MachineStateModel = generate_model(MachineState)

with app.app_context():
    db.create_all()
