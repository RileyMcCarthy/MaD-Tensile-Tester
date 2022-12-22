from flask_sqlalchemy import Column, Integer, Float, String, ForeignKey, Base
from flask_sqlalchemy.orm import relationship
class Model_MonitorDataPacket(Base):
    __tablename__ = "MonitorDataPacket"
    id = Column(Integer, primary_key=True)
    forcemN = Column("forcemN", Integer)
    encoderum = Column("encoderum", Integer)
    setpointum = Column("setpointum", Integer)
    timeus = Column("timeus", Integer)
    log = Column("log", Integer)
class Model_monitor_data_t(Base):
    __tablename__ = "monitor_data_t"
    id = Column(Integer, primary_key=True)
    forceRaw = Column("forceRaw", Integer)
    encoderRaw = Column("encoderRaw", Integer)
    forcemN = Column("forcemN", Integer)
    encoderum = Column("encoderum", Integer)
    force = Column("force", Float)
    position = Column("position", Float)
    setpoint = Column("setpoint", Integer)
    timems = Column("timems", Integer)
    timeus = Column("timeus", Integer)
    log = Column("log", Integer)
