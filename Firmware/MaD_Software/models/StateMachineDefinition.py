from flask_sqlalchemy import Column, Integer, Float, String, ForeignKey, Base
from flask_sqlalchemy.orm import relationship
class Model_MachineCheckParameters(Base):
    __tablename__ = "MachineCheckParameters"
    id = Column(Integer, primary_key=True)
    esdTravelLimit = Column("esdTravelLimit", Integer)
class Model_MachineState(Base):
    __tablename__ = "MachineState"
    id = Column(Integer, primary_key=True)
    state = Column("state", Integer)
    selfCheckParameters = relationship(Model_SelfCheckParameters, backref='machinestate')
    machineCheckParameters = relationship(Model_MachineCheckParameters, backref='machinestate')
    motionParameters = relationship(Model_MotionParameters, backref='machinestate')
    _function = Column("_function", Integer)
    _functionData = Column("_functionData", Integer)
    _lock = Column("_lock", Integer)
class Model_MotionParameters(Base):
    __tablename__ = "MotionParameters"
    id = Column(Integer, primary_key=True)
    status = Column("status", Integer)
    condition = Column("condition", Integer)
    mode = Column("mode", Integer)
class Model_SelfCheckParameters(Base):
    __tablename__ = "SelfCheckParameters"
    id = Column(Integer, primary_key=True)
