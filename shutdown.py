from gpiozero import Button
#import button library from the Pi GPIOZero library
import time
# import time library
import os
#imports OS library

shut_But = Button(26)
# declared GPIO 26 pin for input of button

while True:
# declared the infinite loop
     if shut_But.is_pressed:
     #Check to see if button is pressed
        time.sleep(1)
        # wait for the hold time
        if shut_But.is_pressed:
        #check to see if button is pressed
            os.system("shutdown now -h")
            #shut down the Pi
            time.sleep(1)
            # wait to loop again so we don’t use the processor too much.q
