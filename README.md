# Low Modulus High Speed Tensile Testing Machine

## Purpose

Function as a relatively low cost (< $4,000), portable, spray resistant, user modifiable, open-source uniaxial tensile testing machine for low modulus elastomeric and biologic materials. 

## Physical Specifications

- Strain Rate 0 – 2 m/s

- Force Measurement Range 1 – 10 N

- Gauge Length 10 – 150 mm

- Strain Length 0 – 500 mm

- Data generation rate 1000 sps

## Power Up Sequence

1. Turning on the mains inlet power switch will power the motion control electronics including the “inboard” power on the tensile board 

2. Depressing the on button of the momentary on-off button pair will apply DC power to the machine status switches and ESD chain circuit. 

3. Release of the ESD switch which will apply AC power to the servo controller. 

4. When machine operational and safety criteria are met, motion is affected through operator actuation of the reset function with a control panel and touch screen. 

5. The power down sequence is the reverse of the power up sequence with the off button of the momentary on-off button pair being pressed to remove DC power.

## Operation

The machine state machine operates in three main states with corresponding parameters; 

1. Self Check
- Charge Pump Activated (**ON**/OFF)
2. Machine Check
- Switched Power (**ON**/OFF)
- ESD Travel Limits (**OK**/UPPER FAULT/LOWER FAULT)
- ESD Switch (**ON**/OFF)
- Servo Ready (**YES**/NO)
- Servo Communicating (**YES**/NO)
- Force Gauge Ready (**YES**/NO)
3. Motion
- Mode (Manual or Test)
- Status (DISABLED, ENABLED, SAMPLE LIMIT, MACHINE LIMIT, FAULT)
- Condition (Length, Force, Tension, Compression, Upper Limit, Lower Limit, Door, Stopped, Moving)

## Manual Mode

Manual Operation Mode controls the motor based on user input. The user can selected from the following functions: Off, Incremental Jog, Continuous Jog, Move to Position, Handwheel Jog, Move to Home Position, Set or Move to a Gauge Length, Set or Move to a Force Level and Hold. 

## Test Mode 
Test Mode controls the motor automatically using the functions: Load, Run, Stop Test (Stop Tst) and Hold/Resume.  Motion Profiles are executed by the Run Function. 

** Documentation is a WIP **
