from ctypes import *
class MachineConfiguration(Structure):
    _fields_ = [
        ("(model)", char motorType[MAX_CONFIGURATION_MOTOR_TYPE]; // Motor used for the machine),
        ("(Nm)", double maxMotorTorque; // Maximum torque of the motor),
        ("(RPM)", double maxMotorRPM; // Maximum RPM of the motor),
        ("(mm)", double gearDiameter; // Diameter of the gear),
        ("(mm)", double gearPitch; // Pitch of the gear),
        ("(kgm2)", double systemIntertia; // Total inertia of the system),
        ("(Nm)", double staticTorque; // Static torque of the motor),
        ("(N)", double load; // Estimated load of the system),
        ("scale)", char positionEncoderType[MAX_CONFIGURATION_ENCODER_TYPE]; // Encoder used for the position measurement (encoder or distance),
        ("(step/mm)", double positionEncoderStepsPerRev; // steps per mm of position encoder),
        ("(model)", char forceGauge[MAX_CONFIGURATION_FORCE_GAUGE]; // Force gauge used),
        ("(mN/step)", double forceGaugeScaleFactor; // Force scale factor),
        ("(steps)", int forceGaugeZeroFactor; // Force offset),
    ]

from ctypes import *
class MachinePerformance(Structure):
    _fields_ = [
        ("(mm)", double minPosition; // Minimum position of the motor),
        ("(mm)", double maxPosition; // Maximum position of the motor),
        ("(mm/s)", double maxVelocity; // Maximum velocity of the motor),
        ("(mm/s2)", double maxAcceleration; // Maximum acceleration of the motor),
        ("(N)", double maxForceTensile; // Maximum force tensile of the motor),
        ("(N)", double maxForceCompression; // Maximum force compression of the motor),
        ("(N)", double forceGaugeNeutralOffset; // Force gauge neutral offset),
    ]

from ctypes import *
class MachineProfile(Structure):
    _fields_ = [
        ("profile", char name[MAX_MACHINE_PROFILE_NAME]; // Name of the machine),
        ("configuration;", MachineConfiguration),
        ("performance;", MachinePerformance),
    ]

from ctypes import *
class FunctionInfo(Structure):
    _fields_ = [
        ("function;", Function),
        ("10)", double parameters[MAX_MOTION_QUARTET_PARAMETERS]; // Parameters of the motion quartet (max),
    ]

from ctypes import *
class MotionQuartet(Structure):
    _fields_ = [
        ("quartet", char name[MAX_MOTION_QUARTET_NAME]; // Filename of the motion),
        ("function;", FunctionInfo),
        ("(us)", double dwell; // Dwell of the motion quartet),
    ]

from ctypes import *
class MotionSet(Structure):
    _fields_ = [
        ("defined", char name[MAX_MOTION_PROFILE_SET_NAME]; // Motion set Filename, user),
        ("etc)", char type[MAX_MOTION_PROFILE_SET_TYPE]; // descriptive purpose of the set (ie. precondition, test,),
        ("set", int executions; // Number of times to execute the motion),
        ("set", int quartetCount; // Number of motion quartets in the),
        ("10)", MotionQuartet quartets[MAX_MOTION_QUARTETS]; // List of quartets to execute (max),
    ]

from ctypes import *
class MotionProfile(Structure):
    _fields_ = [
        ("profile", char name[MAX_MOTION_PROFILE_NAME]; // Filename of the motion),
        ("profile", int setCount; // Number of motion sets in the),
        ("sets", MotionSet sets[MAX_MOTION_PROFILE_SETS]; // Array of motion),
    ]

from ctypes import *
class SampleProfile(Structure):
    _fields_ = [
        ("profile", char name[MAX_SAMPLE_PROFILE_NAME]; // Name of the sample),
        ("(mm)", double length; // Length of the sample profile),
        ("length)", double stretchMax; // Maximum stretch of the sample profile (% of),
        ("(mm/s)", double maxVelocity; // Maximum velocity of the sample profile),
        ("(mm/s2)", double maxAcceleration; // Maximum acceleration of the sample profile),
        ("(mm/s3)", double maxJerk; // Maximum jerk of the sample profile),
        ("(N)", double maxForceTensile; // Maximum force tensile of the sample profile),
        ("(N)", double maxForceCompression; // Maximum force compression of the sample profile),
    ]

from ctypes import *
class TestProfile(Structure):
    _fields_ = [
        ("name", char name[MAX_TEST_PROFILE_NAME]; // Descriptive test),
        ("sample", int sampleSN; // Serial number of the),
        ("profile", MachineProfile machineProfile; // Name of the machine),
        ("profile", SampleProfile sampleProfile; // Name of the sample),
        ("profile", MotionProfile motionProfile; // Name of the motion),
        ("test", char comment[MAX_TEST_PROFILE_COMMENT]; // Comment for the),
    ]

