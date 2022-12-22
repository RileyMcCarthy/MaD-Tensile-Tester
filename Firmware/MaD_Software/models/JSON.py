from flask_sqlalchemy import Column, Integer, Float, String, ForeignKey, Base
from flask_sqlalchemy.orm import relationship
class Model_FunctionInfo(Base):
    __tablename__ = "FunctionInfo"
    id = Column(Integer, primary_key=True)
    function = Column("function", Integer)
class Model_MachineConfiguration(Base):
    __tablename__ = "MachineConfiguration"
    id = Column(Integer, primary_key=True)
    maxMotorTorque = Column("maxMotorTorque", Float)
    maxMotorRPM = Column("maxMotorRPM", Float)
    gearDiameter = Column("gearDiameter", Float)
    gearPitch = Column("gearPitch", Float)
    systemIntertia = Column("systemIntertia", Float)
    staticTorque = Column("staticTorque", Float)
    load = Column("load", Float)
    positionEncoderStepsPerRev = Column("positionEncoderStepsPerRev", Float)
    forceGaugeScaleFactor = Column("forceGaugeScaleFactor", Float)
    forceGaugeZeroFactor = Column("forceGaugeZeroFactor", Integer)
class Model_MachinePerformance(Base):
    __tablename__ = "MachinePerformance"
    id = Column(Integer, primary_key=True)
    minPosition = Column("minPosition", Float)
    maxPosition = Column("maxPosition", Float)
    maxVelocity = Column("maxVelocity", Float)
    maxAcceleration = Column("maxAcceleration", Float)
    maxForceTensile = Column("maxForceTensile", Float)
    maxForceCompression = Column("maxForceCompression", Float)
    forceGaugeNeutralOffset = Column("forceGaugeNeutralOffset", Float)
class Model_MachineProfile(Base):
    __tablename__ = "MachineProfile"
    id = Column(Integer, primary_key=True)
    configuration = relationship(Model_MachineConfiguration, backref='machineprofile')
    performance = relationship(Model_MachinePerformance, backref='machineprofile')
class Model_MotionProfile(Base):
    __tablename__ = "MotionProfile"
    id = Column(Integer, primary_key=True)
    setCount = Column("setCount", Integer)
class Model_MotionQuartet(Base):
    __tablename__ = "MotionQuartet"
    id = Column(Integer, primary_key=True)
    function = relationship(Model_FunctionInfo, backref='motionquartet')
    dwell = Column("dwell", Float)
class Model_MotionSet(Base):
    __tablename__ = "MotionSet"
    id = Column(Integer, primary_key=True)
    executions = Column("executions", Integer)
    quartetCount = Column("quartetCount", Integer)
class Model_SampleProfile(Base):
    __tablename__ = "SampleProfile"
    id = Column(Integer, primary_key=True)
    length = Column("length", Float)
    stretchMax = Column("stretchMax", Float)
    maxVelocity = Column("maxVelocity", Float)
    maxAcceleration = Column("maxAcceleration", Float)
    maxJerk = Column("maxJerk", Float)
    maxForceTensile = Column("maxForceTensile", Float)
    maxForceCompression = Column("maxForceCompression", Float)
class Model_TestProfile(Base):
    __tablename__ = "TestProfile"
    id = Column(Integer, primary_key=True)
    sampleSN = Column("sampleSN", Integer)
    machineProfile = relationship(Model_MachineProfile, backref='testprofile')
    sampleProfile = relationship(Model_SampleProfile, backref='testprofile')
    motionProfile = relationship(Model_MotionProfile, backref='testprofile')
