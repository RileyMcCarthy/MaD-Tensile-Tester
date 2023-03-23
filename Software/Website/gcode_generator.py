import math

# Define the function to generate the curve
def curve_func(x):
    return math.sin(x)  # Example function, replace with your own

# Set up the G-code generator
x_start = 0      # Starting X position
x_end = 10       # Ending X position
x_step = 0.1     # Step size for X
z_height = 5     # Z position (height above work surface)
feed_rate_max = 1000   # Maximum feed rate
feed_rate_min = 100    # Minimum feed rate
time_max = 10           # Maximum time to complete the curve
time_min = 0.1          # Minimum time to complete the curve

# Generate the G-code
with open("curve.gcode", "w") as f:
    x = x_start
    time_elapsed = 0
    while x <= x_end:
        y = curve_func(x)
        feed_rate = feed_rate_min + (feed_rate_max - feed_rate_min) * (time_elapsed / time_max)
        f.write("G1 X{} F{}\n".format(x_step, feed_rate))
        x += x_step
        time_elapsed += (time_max - time_min) * x_step / (x_end - x_start)
