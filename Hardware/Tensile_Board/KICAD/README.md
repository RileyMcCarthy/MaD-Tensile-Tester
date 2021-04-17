# Tensile_Board

# AM26LS32
The datasheet for the DYN4 recommends a AM2632C or equivalent. The difference between the two IC's are the AM26LS32
has around the switching frequency of the AM26C32. This wont be an issue due to the max pulses per revolution 
the incremential encoder library can handle is 3332ppr at 5000RPM.
(https://forums.parallax.com/discussion/89954/quadrature-encodersam26%20l)
At 3332ppr and 5000RPM we get a switching frequency of 277KHz which is well in spec of the AM26LS32(around 20MHz max).

This limits the DYN4 line_num (Section 4.5 in DYN4 manual) which determines the ppm = 4* line_num = 3332. Giving
a line_num = 833.

# 6N137
The optocoupler used for isolation of the encoder lines needs to handle signals switching max 277KHz. The 6N137
can handle up to 20MHz (1/(t_r + t_f)) making it very capable for our speeds.

Next choosing the input resistor of 480ohms. This was determined using the V_f typical of 1.4V, with an input of 5V 
to achieve a forward current of 7.5mA (recommended) we will need a 480ohm resistor.

# Motor IO
The IO to the motor must be completely isolated. For recieving signals from the motor a simple optocoupler with a 
1k resistor is needed(YN4 manual section 2.3.2). To send signals to the motor we will use a line driver (AM26LS31CDR)
and optocoupler. The line driver will allow a high speed signal travel over longer distances to the motor. This setup
will be the reverse setup used to read the DYN4 encoder output.